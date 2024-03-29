#include "../../include/general/blocks.h"
#include <vector>

LweKeySwitch::LweKeySwitch()
{
    // this->N = DEF_N;
    // this->tt = DEF_tt;
    // this->base = (1 << DEF_basebit);

    // for (int i = 0; i < N; i++)
    //     for (int j = 0; j < tt; j++)
    //         for (int k = 0; k < base; k++)
    //             keySwitchingKey[i][j][k] = new LWESample;
}

// LweKeySwitch::LweKeySwitch(TGSWKey *tgswKey, LWEKey *key)
// {
//     this->N = DEF_N;
//     this->tt = DEF_tt;
//     this->base = (1 << DEF_basebit);
//     TLWEKey * tlweKey = tgswKey->getTLWEKey();

//     for (int i = 0; i < N; i++)
//         for (int j = 0; j < tt; j++)
//             for (int k = 0; k < base - 1; k++)
//             {
//                 keySwitchingKey[i][j][k] = new LWESample{key->getParams()};
//                 Torus32 in_M = ((k + 1) * tlweKey->getIntKey()->getCoef(i)) * (1 << (32 - DEF_basebit * (j + 1)));
//                 LWESample result = lweEncrypt(&in_M, DEF_TLWE_ALPHA, key);
//                 this->keySwitchingKey[i][j][k] = &result;
//                 printf("i,j,k,ki,x,phase=%d,%d,%d,%d,%d,%d\n",i,j,k,tlweKey->getIntKey()->getCoef(i),in_M,lwePhase(keySwitchingKey[i][j][k],key));
//             }
// }

LweKeySwitch::LweKeySwitch(TGSWKey *tgswKey, LWEKey *key)
{
    this->N = DEF_N;
    this->tt = DEF_tt;
    this->base = (1 << DEF_basebit);
    TLWEKey *tlweKey = tgswKey->getTLWEKey();
    const int32_t sizeks = N*tt*(base-1);
    Torus32 zeromsg = 0;

    std::vector<Torus32> noise;
    for (int i = 0; i < sizeks; i++)
        noise.push_back(addGaussianNoise(&zeromsg, DEF_TLWE_ALPHA));

    int32_t noiseIndex = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < tt; j++)
        {
            keySwitchingKey[i][j][0] = new LWESample{key->getParams()}; // 0 here
            for (int k = 1; k < base; k++)
            {
                // keySwitchingKey[i][j][k] = new LWESample{key->getParams()};
                Torus32 in_M = (k * tlweKey->getIntKey()->getCoef(i)) * (1 << (32 - DEF_basebit * (j + 1)));
                LWESample result = lweEncryptWthExtNoise(&in_M, DEF_TLWE_ALPHA, noise[noiseIndex], key);
                this->keySwitchingKey[i][j][k] = &result;
                noiseIndex++;
                // printf("i,j,k,ki,x,phase=%d,%d,%d,%d,%d,%d\n",i,j,k,tlweKey->getIntKey()->getCoef(i),in_M,lwePhase(keySwitchingKey[i][j][k],key));
            }
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
    this->switchKey = LweKeySwitch{tgswKey, lweKey};
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

std::array<std::array<std::array<LWESample *, (1U << DEF_basebit)>, DEF_tt>, DEF_N> LweKeySwitch::getSwitchKey()
{
    return keySwitchingKey;
}

/**
 * Functions needed for presentation
 */
LWEKey generatePrivateLWEKey(LWEParams* params) {
    LWEKey lwekey(params);
    lweKeyGen(&lwekey);

    return lwekey;
}

TGSWKey generatePrivateTGSWKey(TGSWParams* params) {
    TGSWKey tgswkey{params};
    tgswKeyGen(&tgswkey);

    return tgswkey;
}

GateKey generateGateKey(LWEKey* lwekey, TGSWKey* tgswkey) {
    // Generate public Cloud Key
    GateKey gk(tgswkey, lwekey);

    return gk;
}

Torus32 generateMessage() {
    int32_t M = 8;
    bool binMsg = rand() % 2;
    int32_t msg = binMsg ? switchToTorus32(1, M) : -switchToTorus32(1, M);
    return msg;
}

int32_t testAdditionTillErrorOccurs(LWESample& s1, Torus32& plaintext1, LWEKey& key) {
    int32_t M = 8;
    int32_t loops = 1;
    bool condition = true;
    LWEParams* params = key.getParams();
    Torus32 res = 0;
    int32_t msg0 = 0;
    LWESample backupSample{params};
    LWESample s2 = lweEncrypt(&msg0, DEF_TLWE_ALPHA, &key);  // 0, we want to test only noise behaviour

    while (condition) {
        lweAdd(&s1, &s2, params);
        res = lweDecrypt(&s1, &key, M);

        if (res != plaintext1) {
            lweCopy(&s1, &backupSample, params); // keep msg1 at 1 operation before noise exceeds critical lvl
            condition=!condition;
        }
        
        if (!condition)
            break;
            
        lweCopy(&backupSample, &s1, params);
        loops++;
    }
    return loops;
}

int32_t testAdditionTillErrorOccurs(LWESample& s1, Torus32& plaintext1, TLWEKey* key) {
    int32_t M = 8;
    int32_t loops = 1;
    bool condition = true;
    LWEParams* params = key->getTLWEParams()->getLWEParams();
    Torus32 res = 0;
    int32_t msg0 = 0;
    LWESample backupSample{params};
    LWESample s2 = lweEncrypt(&msg0, DEF_TLWE_ALPHA, key);  // 0, we want to test only noise behaviour

    while (condition) {
        res = lweDecrypt(&s1, key, M);
        lweAdd(&s1, &s2, params);
        res = lweDecrypt(&s1, key, M);

        if (res != plaintext1)
            condition=!condition;
        
        if (!condition)
            break;
        lweCopy(&backupSample, &s1, params);
        loops++;
    }
    return loops;
}

void bootstrapping(LWESample& victim, GateKey& gk) {
    bootstrapinglwe2lwe(&victim, &victim, &gk);
}