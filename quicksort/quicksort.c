/**
 *
 * Program: quicksort
 * Author: Benedikt Stonig
 * Date 19.01.2021
 * Description: Generates a list of random numbers and sorts
 * git-repo: https://github.com/Essigschurke13/School.git
 * 
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define MY_SIZE 100

//partitioning
int p(int *a, int lower, int higher)
{
    int pivot = a[higher];
    int buffer;
    int i = lower;
    int j = higher;

    while (i < j)
    {
        //get number greater or equual than pivot on lower end
        while (i < higher && a[i] < pivot)
        {
            i++;
        }


        //get number smaller than pivot on higher end
        while (j > lower && a[j] >= pivot)
        {
            j--;
        }

        if (i < j)
        {
            //swap previously obtained numbers
            buffer = a[i];
            a[i] = a[j];
            a[j] = buffer;
        }
    }

    if (a[i] > pivot)
    {
        //swap pivot with its new position
        buffer = a[i];
        a[i] = a[higher];
        a[higher] = buffer;
    }

    return i;
}

//quicksort
void qs(int *a, int lower, int higher)
{
	if (lower < higher)
    {
        int pivot = p(a, lower, higher);
        qs(a, lower, pivot - 1);
        qs(a, pivot + 1, higher);
    }
}

// creates a array of size size and fills it with random ints in range 0 to max_int
int *create_array(int size, int max_int)
{
	int *b = (int*)malloc(size * sizeof(int));

	for (int i=0; i<size; i++) 
    {
		b[i] = rand() % max_int;
	}

	return b;
}

int main(int argc, char **argv)
{
    struct timespec t_start, t_end, t_diff;

//set clock type (Thanks to Kasticky for showing me this way to measure time)
#ifdef CLOCK_THREAD_CPUTIME_ID
    clockid_t clockid = CLOCK_THREAD_CPUTIME_ID;
#else
    clockid_t clockid = CLOCK_REALTIME;
#endif

	// create random ints based in current time
	srand(time(NULL));
    int *a = create_array(MY_SIZE, 100);
    
    //get start time
    clock_gettime(clockid, &t_start);
	qs(a, 0, MY_SIZE);
    //get end time
    clock_gettime(clockid, &t_end);

	int old = -1;
	for (int i=0; i<MY_SIZE; ++i)
    {
		if (old != -1) assert(old <= a[i]);
		printf("%d ", a[i]);
		old = a[i];
	}
	printf("\n");

    //print time
    printf("Time: %lins\n", t_end.tv_nsec - t_start.tv_nsec);

    return 0;
}