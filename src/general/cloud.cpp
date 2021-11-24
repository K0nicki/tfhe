#include "../../include/general/blocks.h"

// TODO:
// validation
LweKeySwitch::LweKeySwitch() {}

LweKeySwitch::LweKeySwitch(LWEKey *key)
{
    this->N = DEF_N;
    this->tt = DEF_tt;
    this->base = (1 << DEF_basebit);

    for (int i = 0; i < N; i++)
        for (size_t j = 0; j < tt; j++)
            for (size_t k = 0; k < base - 1; k++)
            {
                keySwitchingKey[i][j][k] = new LWESample;
                int32_t in_M = (k + 1) * key->getLWEKey(i) * (1U << (32 - DEF_basebit * (j + 1)));
                LWESample result = lweEncrypt(&in_M, DEF_TLWE_ALPHA, key);
                this->keySwitchingKey[i][j][k] = &result;
            }
}

LweKeySwitch::~LweKeySwitch() {}

BootstrappingKey::BootstrappingKey() {}

BootstrappingKey::BootstrappingKey(TGSWKey *tgswKey, LWEKey *lweKey)
{
    this->tgswParams = tgswKey->getTGSWparams();

    for (int i = 0; i < DEF_N; i++)
    {
        bootstrappingKey[i] = new TGSWSample{tgswKey->getTGSWparams()};
        tgswEncryptInt(bootstrappingKey[i], lweKey->getLWEKey(i), DEF_TRLWE_ALPHA, tgswKey);
    }
}

BootstrappingKey::~BootstrappingKey() {}

TGSWParams *BootstrappingKey::getTGSWParams()
{
    return this->tgswParams;
}

TGSWSample *BootstrappingKey::getSampleAt(int i)
{
    return bootstrappingKey.at(i);
}

GateKey::GateKey(TGSWKey *tgswKey, LWEKey *lweKey)
{
    this->bootstrappingkey = BootstrappingKey{tgswKey, lweKey};
    this->switchKey = LweKeySwitch{lweKey};
}

GateKey::~GateKey() {}

TGSWParams *GateKey::getTGSWParams()
{
    return this->bootstrappingkey.getTGSWParams();
}

BootstrappingKey *GateKey::getBootstrappingKey()
{
    return &bootstrappingkey;
}

LweKeySwitch *GateKey::getSwitchKey()
{
    return &switchKey;
}

std::array<std::array<std::array<LWESample*, (1U << DEF_basebit) - 1>, DEF_tt>, DEF_N> LweKeySwitch::getSwitchKey() {
    return keySwitchingKey;
}
