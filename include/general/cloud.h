#include "secparams.h"
#include "../tgsw/functions.h"
#include "../lwe/functions.h"

class LweKeySwitch
{
private:
    int32_t N;
    int32_t tt;
    int32_t base;
    std::array<std::array<std::array<LWESample*, (1U << DEF_basebit)>, DEF_tt>, DEF_N> keySwitchingKey; // N x t x 3 x 2 x N
public:
    LweKeySwitch();
    LweKeySwitch(TGSWKey *tgswKey, LWEKey *key);

    std::array<std::array<std::array<LWESample*, (1U << DEF_basebit)>, DEF_tt>, DEF_N> getSwitchKey();
    ~LweKeySwitch();
};

class BootstrappingKey
{
private:
    TGSWParams *tgswParams;
    std::array<TGSWSample*, DEF_N> bootstrappingKey;

public:
    TGSWParams* getTGSWParams();
    TGSWSample* getSampleAt(int i);

    BootstrappingKey();
    BootstrappingKey(TGSWKey *tgswKey, LWEKey* lweKey);
    ~BootstrappingKey();
};

class GateKey
{
private:
    BootstrappingKey bootstrappingkey;
    LweKeySwitch switchKey;

public:
    BootstrappingKey* getBootstrappingKey();
    LweKeySwitch* getSwitchKey();
    TGSWParams* getTGSWParams();

    GateKey(TGSWKey *tgswKey, LWEKey *lweKey);
    ~GateKey();
};


/**
 * Functions needed for presentation
 */
LWEKey generatePrivateLWEKey(LWEParams* params);
TGSWKey generatePrivateTGSWKey(TGSWParams* params);
GateKey generateGateKey(LWEKey* lwekey, TGSWKey* tgswkey);
Torus32 generateMessage();
int32_t testAdditionTillErrorOccurs(LWESample& s1, Torus32& plaintext1, LWEKey& key);
int32_t testAdditionTillErrorOccurs(LWESample& s1, Torus32& plaintext1, TLWEKey* key);
void bootstrapping(LWESample& victim, GateKey& gk);
