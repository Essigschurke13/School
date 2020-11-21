/**
 *
 * Program: Sieve of Erastosthenes
 * Author: Benedikt Stonig 5AHEL
 * Date: 11.10.2020
 * Description: This Program calculates prime numbers using the sieve of erastosthenes.
 * git-repo: https://github.com/Essigschurke13/School.git
 *
 */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

int main()
{
    //initalize bool array for primes
    bool primeNumbers[500];
    for (int i = 2; i <= 500; i++)
    {
        primeNumbers[i]=true;
    }

    //calculate primes with the sieve of erastosthenes
    for (int i = 2; i <= 23; i++)
    {
        if (primeNumbers[i])
        {
            for (int j = i*i; j <= 500; j += i)
            {
                primeNumbers[j] = false;
            }
        }
    }

    //print the result
    for (int i = 2; i < 500; i++)
    {
        if (primeNumbers[i])
        {
            printf("%i ", i);
        }
    }

    return 0;
}
