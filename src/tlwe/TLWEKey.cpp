#include "../../include/tlwe/TLWEKey.h"
#include <array>

TLWEKey::TLWEKey(TLWEParams *params) : params(params)
{
    for (size_t i = 0; i < this->key.size(); i++)
    {
        key.at(i) = new IntPolynomial{params->getDegree()};
    }
}

TLWEKey::~TLWEKey() {}

TLWEParams *TLWEKey::getTLWEParams() { return (this->params); }

IntPolynomial *TLWEKey::getKey()
{
    return *(this->key.data());
}

IntPolynomial *TLWEKey::getKey(int i)
{
    return this->key.at(i);
}
