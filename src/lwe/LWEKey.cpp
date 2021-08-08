#include "../../include/lwe/LWEKey.h"

LWEKey::LWEKey(LWEParams *params)
{
    this->params = params;
    this->key = new int32_t[params->getLength()];
}

LWEKey::~LWEKey()
{
    delete[] key;
}

void LWEKey::setKey(int32_t val, int i)
{
    this->key[i] = val;
}

LWEParams *LWEKey::getParams()
{
    return this->params;
}

int32_t *LWEKey::getLWEKey()
{
    return this->key;
}
