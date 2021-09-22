#include "../../include/tgsw/TGSWKey.h"

TGSWKey::TGSWKey(TGSWParams* params): tgswParams(params), tlweParams(params->getTLWEParams())
{
    tlwekey = &TLWEKey{tlweParams};
    key = tlwekey->getKey();
}

TGSWKey::~TGSWKey()
{
}

TLWEKey* TGSWKey::getTLWEKey() {
    return this->tlwekey;
}

void TGSWKey::setTLWEKey(TLWEKey* key) {
    this->tlwekey = key;
}


TGSWParams* TGSWKey::getTGSWparams() {
    return this->tgswParams;
}
