
/**
 *
 * Program: rle-encode
 * Author: Benedikt Stonig
 * Date: 26.10.2020
 * Description: BBOOOO -> B2O4
 *              OOOOOOOOOOOO -> O12
 * git-repo: https://github.com/Essigschurke13/School.git
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RLEN 42

//encode function: encodes given string to required output
char* encode(char* input)
{
    int n = strlen(input);
    char* result = malloc(2 * MAX_RLEN * sizeof(char));
    int count;
    int i = 0;
    int j = 0;

    for (i; i < n; i++)
    {
        result[j] = input[i];
        count = 1;

        /**counts identical characters (count < 9 because if the count is greater than 9
         * the count isn't appended correctly to the result, i couln't find a solution yet, 
         * but i will search for one if i have time.
         */
		while (i < n - 1 && input[i] == input[i + 1] && count < 9) {
			count++;
			i++;
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

