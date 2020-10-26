
/**
 *
 * Program: rle-encode
 * Author: Benedikt Stonig
 * Date: 26.10.2020
 * Description: BBOOOO -> B2O4
 *              OOOOOOOOOOOO -> O9O3
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RLEN 128

//encode function: encodes given string to required output
char* encode(char* input)
{
    char* result = malloc(2 * MAX_RLEN * sizeof(char));
    int count;
    int i = 0;
    int j = 0;

    while (i < strlen(input))
    {
        result[j] = input[i];
        count = 0;

        // count identical chars
        while (result[j] == input[i] && count < 9)
        {
            i++;
            count++;
        }
        
        // append char count to result string as `char`
        result[++j] = count + '0';
        j++;
    }

    // terminate string
    result[++j] = '\0';

    return result;
}


// given main
int main(int argc, char **argv)
{
	char str[MAX_RLEN];
      
       	do {	
		if (fgets(str, MAX_RLEN, stdin) == NULL) break;
		str[MAX_RLEN] = 0;
		str[strlen(str)-1] = 0;
		char* res = encode(str);
		printf(">%s\n", res);
		free(res);
	} while (strlen(str) > 1);
}

