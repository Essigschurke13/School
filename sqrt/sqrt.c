/**
 *
 * Program: My_SQRT
 * Author: Benedikt Stonig 5AHEL
 * Date: 30.09.2020
 * Description: This Program calculates the square root of the user input
 *
 */

#include <stdio.h>

//This function squares the number by adding the input by itself
int square(int a)
{
    int i, j;
    int result = 0;
    for (i = 0; i != a; i++)
    {
        for (j = 0; j != a; j++)
        {
            result++;
        }
    }
    return result;
}

//The following function calculates the square root be squaring the loop control variable and then comparing it to the input
int my_sqrt(int a)
{
    int i, j;
    int squared;
    int y = -1;
    for (i = 0; squared <= a; i++)
    {
        squared = square(i);
        if (squared != a)
        {
            if (squared <= a)
            {
                continue;
            }
            else
            {
                //for results bigger than the input the result is reduced by 1
                for (j = 0; j != i; j++)
                {
                    y++;
                }
                return y;
            }
        }
        else
        {
            return i;
        }
    }
}

int main()
{
    int input, output;
    printf("Input: ");
    scanf("%i", &input);
    output = my_sqrt(input);
    printf("Result: %i", output);
    return 0;
}