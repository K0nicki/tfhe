#include "../../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>
#include <cassert>

int main(int argc, char const *argv[])
{

    LWEParams params(DEF_n, 0., 1.);
    TGSWParams tgswparams{new TLWEParams{DEF_N, 1, 0.,1.}};
    
    LWEKey privateLWEKey = generatePrivateLWEKey(&params);
    TGSWKey privateTGSWKey = generatePrivateTGSWKey(&tgswparams);
    GateKey publicCloudKey{&privateTGSWKey, &privateLWEKey};   // Encrypt LWEKey using TGSWKey => public key

    Torus32 message = generateMessage();    // Generate random message 

    LWESample encryptedMessage = lweEncrypt(&message, DEF_TLWE_ALPHA, &privateLWEKey);  // Encryption
    
    int32_t loops = testAdditionTillErrorOccurs(encryptedMessage, message, privateLWEKey);
    std::cout << "Maximum add operation number is: " << loops << std::endl;

    bootstrapping(encryptedMessage, publicCloudKey);

    loops += testAdditionTillErrorOccurs(encryptedMessage, message, privateTGSWKey.getTLWEKey());
    std::cout << "Maximum add operation number with bootstrapping is: " << loops << std::endl;

    return 0;
}




