#include "../../include/tlwe/TLWEKey.h"

IntPolynomial* init_IntPolynomial_array(int32_t arrayLength, int32_t N) {
    IntPolynomial* arr = (IntPolynomial*) malloc(arrayLength*sizeof(IntPolynomial));    // Allocate space for polynomials
    for (int i = 0; i < arrayLength; i++)
       new(arr+i) IntPolynomial(N);                                                     // Initiate it
    return arr;                                                                         // Return address to the first element
}

TLWEKey::TLWEKey(TLWEParams* params):
    params(params) {
        key = init_IntPolynomial_array(params->getPolyAmount(), params->getDegree());
    }

TLWEKey::~TLWEKey() {}

TLWEParams* TLWEKey::getTLWEParams() { return (this->params); }

IntPolynomial* TLWEKey::getKey(){
    return this->key;
}

IntPolynomial* TLWEKey::getKey(int i){
    return &(this->key)[i];
}
