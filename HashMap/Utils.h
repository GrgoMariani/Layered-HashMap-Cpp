#pragma once
/*
 * Heavily based on https://stackoverflow.com/questions/30052316/find-next-prime-number-algorithm
 * Really simple and fast
 */
namespace CRT{

    bool isPrime(const unsigned int number);
    int nextPrime(unsigned int a);
    int previousPrime(unsigned int a, const unsigned int minPrime=13);

}
