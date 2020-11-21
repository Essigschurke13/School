/**
 *
 * Program: file-copy
 * Author: Benedikt Stonig
 * Date 28.10.2020
 * Description: Copies a file like the cp command in Linux
 * Usage: Outputfile from compiler with source arg and then destination arg
 * git-repo: https://github.com/Essigschurke13/School.git
 * 
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    size_t buffer_size;

    buffer_size = 20;

    // allocating buffer
    char *buffer = malloc(buffer_size);
    if (errno)
    {
        perror("buffer allocation");
        return EXIT_FAILURE;
    }

    // open input file
    int input_file = open(argv[1], O_RDONLY);
    if (input_file < 0)
    {
        perror("Couldn't open source file");
        return EXIT_FAILURE;
    }

    // open or create output file
    int output_file = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT);
    if (output_file < 0)
    {
        perror("Couldn't open destination file");
        return EXIT_FAILURE;
    }

    // copy process
    ssize_t read_status, write_status;
    do
    {
        // read input file to buffer
        read_status = read(input_file, buffer, buffer_size);
        if (read_status < 0)
        {
            perror("Couldn't read source file");
            return EXIT_FAILURE;
        }

        // write buffer to output file
        write_status = write(output_file, buffer, read_status);
        if (write_status < 0)
        {
            perror("Couldn't write to destination file");
            return EXIT_FAILURE;
        }
    } while (read_status != 0);

    close(input_file);
    close(output_file);
    free(buffer);

    return 0;
}