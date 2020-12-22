/**
 *
 * Program: Binary search
 * Author: Benedikt Stonig
 * Date: 01.12.2020
 * Description:
 * git-repo: https://github.com/Essigschurke13/School.git
 * 
 * Comment: The Program does not work. It exits with the error: Segmentation fault
 *          Couldn't find the problem yet. Probably it is something with the searchindex building.
 *          Didn't have time to take a closer look yet
 *
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>


void *search(const void *key, const void *base, size_t size, size_t start, size_t end)
{
    if (end < start) return NULL;
    
    size_t mid = (start + end) / 2;
    const void *p = (void *)(((const char *)base) + (mid * size)); 
    int comparison = strcmp((char *)key, *(char **)p);

    if (comparison > 0)
        return search(key, base, size, mid + 1, end);
    else if (comparison < 0)
        return search(key, base, size, start, mid - 1);
    else
        return (void *)p;
}

int main(int argc, char **argv)
{
    // open input file
    int input_file = open(argv[1], O_RDONLY);
    if (input_file < 0)
    {
        perror("Couldn't open source file");
        return EXIT_FAILURE;
    }

    struct stat input_stat;
    fstat(input_file, &input_stat);

    // allocating buffer
    char *buffer = malloc(input_stat.st_size);
    if (errno)
    {
        perror("buffer allocation");
        return EXIT_FAILURE;
    }
  
    // read input file to buffer
    ssize_t read_status;
    read_status = read(input_file, buffer, input_stat.st_size);
    if (read_status < 0)
    {
        perror("Couldn't read source file");
        return EXIT_FAILURE;
    }

    //building searchindex
    size_t word_count = 0;
    unsigned long iterator_input;
    char **search_index = malloc(input_stat.st_size);

    while (iterator_input < input_stat.st_size)
    {
        if (word_count * sizeof(search_index) >= (input_stat.st_size / sizeof(search_index)))
        {
            search_index = reallocarray(search_index, word_count + 1, sizeof(search_index));
        }

        search_index[word_count++] = &buffer[iterator_input];

        while (buffer[iterator_input++] != '\0');
    }

    for (;;) 
    {
        char input[100];

        fgets(input, sizeof(input), stdin);
        input[strlen(input)-1] = 0;

        if (!strlen(input)) break;

        struct timeval tv_begin, tv_end, tv_diff;

        gettimeofday(&tv_begin, NULL);
        void *res = search(input, search_index, sizeof(search_index), 0, word_count);// wie auch immer
        gettimeofday(&tv_end, NULL);

        timersub(&tv_end, &tv_begin, &tv_diff);

        if (res != NULL) 
        {
                printf("found");
        } 
        else 
        {
                printf("not found");
        }
        printf(" in (%ld seconds %ld microseconds)\n", tv_diff.tv_sec, tv_diff.tv_usec);
    }

    free(buffer);
    free(search_index);

    return 0;
}