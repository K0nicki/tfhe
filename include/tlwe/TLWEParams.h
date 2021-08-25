#include "../lwe/LWEParams.h"

/**
 * @brief   Class which contains information about lenght, number of polynomials in the mask, minimal and maximal allowed noise
 *          
 * @param N power of 2, polynomial degree
 * @param k number of polynomials in the mask
 * @param alpha_min minimal noise lvl
 * @param alpha_max maximal noise lvl
 * @param lweParams ????
*/
class TLWEParams
{
private:
    const int32_t N;
    const int32_t k;
    const double alpha_min;
    const double alpha_max;
    LWEParams lweParams;

public:
    // Getters
    int32_t getDegree();
    int32_t getPolyAmount();
    LWEParams* getLWEParams();

    TLWEParams(int32_t N, int32_t k, double alpha_min, double alpha_max);
    ~TLWEParams();
    
    // forbidden
    TLWEParams(TLWEParams&) = delete;
    TLWEParams* operator=(TLWEParams&) = delete;
};
