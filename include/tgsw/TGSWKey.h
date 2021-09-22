#include "TGSWParams.h"
#include "../general/polynomials.h"
#include "../tlwe/TLWEKey.h"

class TGSWKey
{
private:
    TGSWParams* tgswParams;
    TLWEParams* tlweParams;
    IntPolynomial* key;
    TLWEKey* tlwekey; 
public:
    TGSWKey(TGSWParams* params);
    ~TGSWKey();

    TLWEKey* getTLWEKey();
    TGSWParams* getTGSWparams();
    
    void setTLWEKey(TLWEKey* key);
};
