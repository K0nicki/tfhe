#include "../include/tfhe.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    const LWEParams params( 15, 0.1, 0.3);
    LWEKey key (&params);

    lweKeyGen(&key);
    for (int i = 0; i < (&params)->n; i++)
    {
        std::cout << ((&key)->getLWEKey())[i];
    }
    
    return 0;
}