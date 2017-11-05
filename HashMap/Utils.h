#pragma once
/*
 * Heavily based on https://stackoverflow.com/questions/30052316/find-next-prime-number-algorithm
 * Really simple and fast
 */
namespace CRT{

bool isPrime(const unsigned int& number)
{
    if (number == 2 || number == 3)
        return true;
    if (number%2 == 0 || number%3 == 0)
        return false;
    unsigned int divisor=5;
    while ( divisor*divisor <= number)
    {
        if (number%divisor == 0 || number%(divisor+2) == 0)
            return false;
        divisor += 6;
    }
    return true;
}
int nextPrime(unsigned int a)
{
    while (!isPrime(++a)){
    }
    return a;
}
int previousPrime(unsigned int a, unsigned int minPrime=13)
{
    if(a<=minPrime)
        return minPrime;
    while (!isPrime(--a)){
    }
    return a;
}

}
