#include "../../include/tgsw/TGSWKey.h"

TGSWKey::TGSWKey(TGSWParams *params)
{
    this->tgswParams = params;
    this->tlwekey = new TLWEKey{params->getTLWEParams()};
}

TGSWKey::~TGSWKey()
{
}

TLWEKey *TGSWKey::getTLWEKey()
{
    return this->tlwekey;
}

void TGSWKey::setTLWEKey(TLWEKey *key)
{
    this->tlwekey = key;
}

TGSWParams *TGSWKey::getTGSWparams()
{
    return tgswParams;
}

IntPolynomial *TGSWKey::getKey()
{
    return this->tlwekey->getIntKey();
}
