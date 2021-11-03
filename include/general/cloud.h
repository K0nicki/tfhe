#include "secparams.h"
#include "../tgsw/functions.h"
#include "../lwe/functions.h"

class LweKeySwitch
{
private:
    int32_t N;
    int32_t tt;
    int32_t base;
    std::array<std::array<std::array<LWESample, (1U << DEF_basebit) - 1>, DEF_tt>, DEF_N> keySwitchingKey; // N x t x 3 x 2 x N
public:
    LweKeySwitch();
    LweKeySwitch(LWEKey *key);
    ~LweKeySwitch();
};

class BootstrappingKey
{
private:
    TGSWParams *tgswParams;
    std::array<TGSWSample*, DEF_n> bootstrappingKey;

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
