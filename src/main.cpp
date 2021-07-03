#include "../include/tfhe.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    LWEParams params(32, 0.1, 0.3);
    LWEKey key(&params);

    lweKeyGen(&key);
    for (int i = 0; i < (&params)->getLength(); i++)
    {
        std::cout << ((&key)->getLWEKey())[i];
    }
    std::cout << std::endl
              << (&params)->getLength();
    return 0;
}