#include "../../include/tlwe/TLWEKey.h"
#include <array>

TLWEKey::TLWEKey(TLWEParams *params) : params(params)
{
    for (size_t i = 0; i < this->key.size(); i++)
        key.at(i) = new IntPolynomial;
}

TLWEKey::~TLWEKey() {}

TLWEKey &TLWEKey::operator=(TLWEKey &other)
{
    for (int i = 0; i < key.size(); i++)
        this->key[i] = other.key[i];

    this->params = other.params;

    return *this;
}

TLWEParams *TLWEKey::getTLWEParams()
{
    return this->params;
}

IntPolynomial *TLWEKey::getIntKey()
{
    return key.at(0);
}

IntPolynomial *TLWEKey::getIntKey(int i)
{
    return key.at(i);
}
