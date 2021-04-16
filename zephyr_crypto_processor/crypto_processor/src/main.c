/**
 *
 * Project: Zephyr crypto processor
 * Author: Benedikt Stonig
 * git-repo: https://github.com/Essigschurke13/School.git
 * 
 * Thanks goes to Lukas K. for helping me understand how to 
 * implement threads and message queues.
 *
 */

#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <drivers/uart.h>

//default sizes
#define STACKSIZE 128
#define MSGQSIZE 64
#define MAXMESSAGESIZE 64

//Thread names
#define TH_NAME_MAIN "MAIN"
#define TH_NAME_PROCESSING "PROCESSING"
#define TH_NAME_UART_IN "UART IN"
#define TH_NAME_UART_OUT "UART OUT"


/**=============================== *
 * ==========  threads  ========== *
 * =============================== */

// processing thread
K_THREAD_STACK_DEFINE(processing_stack_area, STACKSIZE); // define Stack for processing thread
struct k_thread processing_thread_data;
void processing(void *msgq_in, void *msgq_out, void *n)
{
	char data;

	// below a the messages that are send as answer over uart for test_00 and test_01
	char *message_00 = ".\n";
	char *message_01 = "PROCESSING AVAILABLE\n";

	printk("PROCESSING_THREAD started!\n"); // print that thread is started

	while (1)
	{
		if (k_msgq_get(msgq_in, &data, K_NO_WAIT) == 0) // get data from msgq
		{
			printk("Processing Thread: Recieved %c \n", data); // print recieved data
			switch (data)
			{
			case '.': // if recieved data == '.' echo it
				printk("Processing Thread: Sending %s", message_00);
				for (int i = 0; i < 2; i++)
				{
					while (k_msgq_put(msgq_out, &message_00[i], K_NO_WAIT) != 0) // put message in msgq
					{
						k_msleep(100);
					}
				}
				break;
			case 'P': // if recieved data == 'P' send PROCESSING AVAILABLE as reply
				printk("Processing Thread: Sending %s", message_01);
				for(int i = 0; i < 21; i++)
				{
					while (k_msgq_put(msgq_out, &message_01[i], K_NO_WAIT) != 0) // put message in msgq
					{
						k_msleep(100);
					}
				}
			default:
				break;
			}
			/**
			 *
			 * The rest of the program is still missing. It only works
			 * for test_00 and test_01. I ran out of time. Should have 
			 * managed my time better. :(
			 *
			 */
		}
		else
		{
			k_msleep(10);
		}
	}
}

// uart in thread
K_THREAD_STACK_DEFINE(uart_in_stack_area, STACKSIZE); // define Stack for uart in thread
struct k_thread uart_in_thread_data;
void uart_in(void *msgq, void *uart_dev, void *n)
{
	char data;
    const struct device *dev = uart_dev;

	printk("UART_IN_THREAD started!\n"); // print that thread is started

	while (1)
	{
		if (uart_poll_in(dev, &data) == 0) // get data from uart and if 0 send to msgq
		{
			while (k_msgq_put(msgq, &data, K_NO_WAIT) != 0) // put data in msgq
			{
				printk("Message queues is full!");
				k_msleep(100);
			}
		}
		else // if the retrun of uart_poll_in is 1 sleep for 1ms
		{
			k_msleep(1);
		}
	}
	
}

// uart out thread
K_THREAD_STACK_DEFINE(uart_out_stack_area, STACKSIZE); // define Stack for uart out thread
struct k_thread uart_out_thread_data;
void uart_out(void *msgq, void *uart_dev, void *n)
{
	char data;
    const struct device *dev = uart_dev;

	printk("UART_OUT_THREAD started!\n"); // print that thread is started

	while (1)
	{
		while (k_msgq_get(msgq, &data, K_FOREVER) != 0); // get data from message queue
		uart_poll_out(dev, data); // send data over uart
	}
	
}

/* =======  End of threads  ======= */

/**====================================== *
 * ==========  message queues  ========== *
 * ====================================== */

char msgq_in_char;
char msgq_out_char;

/* =======  End of message queues  ====== */

void main(void)
{
	// get the UART binding
    const struct device *dev =
        device_get_binding(CONFIG_UART_CONSOLE_ON_DEV_NAME);
	
	/* Tried it with K_MSGQ_DEFINE, but did't work. Below is the working code. 
	K_MSGQ_DEFINE(msgq_in, sizeof(msgq_in_char), MAXMSGQ, 8);
	K_MSGQ_DEFINE(msgq_out, sizeof(msgq_out_char), MAXMSGQ, 8); 
	*/

	char __aligned(8) my_msgq_in_buffer[MSGQSIZE * sizeof(msgq_in_char)];
	char __aligned(8) my_msgq_out_buffer[MSGQSIZE * sizeof(msgq_out_char)];

	struct k_msgq msgq_in;
	struct k_msgq msgq_out;

	k_msgq_init(&msgq_in, my_msgq_in_buffer, sizeof(msgq_in_char), MSGQSIZE);
	k_msgq_init(&msgq_out, my_msgq_out_buffer, sizeof(msgq_out_char), MSGQSIZE);

	/* Tried it with K_THREAD_DEFINE, but didn't work. Below is the working code.
	K_THREAD_DEFINE(th_processing, STACKSIZE, 
					processing, (void *)&msgq_in, (void *)&msgq_out, NULL,
					1, 0, 0);

	K_THREAD_DEFINE(th_uart_in, STACKSIZE, 
					uart_in, (void *)&msgq_in, (void *)&dev, NULL,
					1, 0, 0);
	
	K_THREAD_DEFINE(th_uart_out, STACKSIZE, 
					uart_out, (void *)&msgq_out, (void *)&dev, NULL,
					1, 0, 0);
	*/

	// thread creation and start
	k_thread_create(&processing_thread_data, processing_stack_area,
                    K_THREAD_STACK_SIZEOF(processing_stack_area),
                    processing,
                    (void *)&msgq_in, (void *)&msgq_out, NULL,
                    1, 0, K_NO_WAIT);

	k_thread_create(&uart_in_thread_data, uart_in_stack_area,
                    K_THREAD_STACK_SIZEOF(uart_in_stack_area),
                    uart_in,
                    (void *)&msgq_in, (void *)dev, NULL,
                    1, 0, K_NO_WAIT);

	k_thread_create(&uart_out_thread_data, uart_out_stack_area,
                    K_THREAD_STACK_SIZEOF(uart_out_stack_area),
                    uart_out,
                    (void *)&msgq_out, (void *)dev, NULL,
                    1, 0, K_NO_WAIT);

	// set the thread names
	k_thread_name_set(k_current_get(), TH_NAME_MAIN);
    k_thread_name_set(&processing_thread_data, TH_NAME_PROCESSING);
    k_thread_name_set(&uart_in_thread_data, TH_NAME_UART_IN);
    k_thread_name_set(&uart_out_thread_data, TH_NAME_UART_OUT); 

	// Show that MAIN is alive
	while(1)
	{
		printk("Main Thread is alive!\n");
		k_sleep(K_SECONDS(10));
	}
}
