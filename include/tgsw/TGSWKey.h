#pragma once

#include "TGSWParams.h"
#include "../general/polynomials.h"
#include "../tlwe/TLWEKey.h"

class TGSWKey
{
private:
    TGSWParams *tgswParams;
    TLWEKey *tlwekey;

public:
    TGSWKey(TGSWParams *params);
    ~TGSWKey();

    TLWEKey *getTLWEKey();
    IntPolynomial *getKey();
    TGSWParams *getTGSWparams();

    void setTLWEKey(TLWEKey *key);
};
