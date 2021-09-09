#include "../fft/SPQLIOS/spqlios-fft.h"
#include "../../include/general/polyfunctions.h"

template<int32_t N>
void torusPolyfft(FftPoly* result, std::array<int32_t, N>* poly) {
    fftplvl1.execute_reverse_torus32(result->getArray(), poly->data());
}

// void intPolyfft(FftPoly* result, IntPolynomial* poly) {
//     fftplvl1.execute_reverse_int(result->getArray(), poly->getCoef());
// }

template<int32_t N>
void torusPolyifft (std::array<int32_t, N>* result, FftPoly* poly) {
    fftplvl1.execute_direct_torus32(result->data(), poly->getArray());
}

void mulPolyFD(FftPoly* result, FftPoly* a, FftPoly* b) {
    int N = a->getLength();

    for (int i = 0; i < N/2; i++)
    {
        double aImg_bImg = a->getCoef(i+(N/2)) * b->getCoef(i+(N/2));
        double aReal_bImg = a->getCoef(i) * b->getCoef(i+(N/2));

        result->setCoef(i, a->getCoef(i) * b->getCoef(i) - aImg_bImg);
        result->setCoef(i+(N/2), a->getCoef(i+(N/2)) * b->getCoef(i) + aReal_bImg);
    }
}

void torusPolyAddTo(TorusPolynomial* result, TorusPolynomial* a) {
    int32_t N = a->getLenght();
    int32_t r;
    int32_t b;

    for (int i = 0; i < N; i++) {
        r = result->getCoef(i);
        b = a->getCoef(i);
        result->setCoefficient(i, r+b);
    }
}

void torusPolySubTo(TorusPolynomial* result, TorusPolynomial* a) {
    int32_t N = a->getLenght();
    int32_t r;
    int32_t b;

    for (int i = 0; i < N; i++) {
        r = result->getCoef(i);
        b = a->getCoef(i);
        result->setCoefficient(i, r-b);
    }
}

void torusPolyCopy(TorusPolynomial* result, TorusPolynomial* a) {
    int32_t N = a->getLenght();

    for (int i = 0; i < N; i++)
        result->setCoefficient(i, a->getCoef(i));    
}

void torusPolyMulFD(TorusPolynomial* result, IntPolynomial* poly1, TorusPolynomial* poly2) {
    int32_t N = result->getLenght();
    FftPoly fft1;
    FftPoly fft2;
    TorusPolynomial tmp(N);
    TorusPolynomial poly1ifft(N);
    TorusPolynomial poly2ifft(N);

    // Switch to the frequency domain
    torusPolyfft<DEF_N>(&fft1, poly1->getCoefAsArray());
    torusPolyfft<DEF_N>(&fft2, poly2->getCoefAsArray());

    // Multiply vectors
    mulPolyFD(&fft1, &fft1, &fft2);
    torusPolyifft<DEF_N>((&poly1ifft)->getCoefAsArray(), &fft1);
    torusPolyifft<DEF_N>((&poly2ifft)->getCoefAsArray(), &fft2);

    // Do inverse fft
    torusPolyifft<DEF_N>((&tmp)->getCoefAsArray(), &fft1);
    
    torusPolyAddTo(result, &tmp);
}

void torusPolyMulSubFD(TorusPolynomial* result, IntPolynomial* poly1, TorusPolynomial* poly2) {
    int32_t N = result->getLenght();
    FftPoly fft1;
    FftPoly fft2;
    TorusPolynomial tmp(N);

    // Go to the frequency domain
    torusPolyfft<DEF_N>(&fft1, poly1->getCoefAsArray());
    torusPolyfft<DEF_N>(&fft2, poly2->getCoefAsArray());

    // Multiply vectors
    mulPolyFD(&fft1, &fft1, &fft2);

    // Do inverse fft
    torusPolyifft<DEF_N>((&tmp)->getCoefAsArray(), &fft1);
    
    torusPolySubTo(result, &tmp);
}


void torusPolyMakeRandom(TorusPolynomial* result) {
    int32_t N = result->getLenght();
    std::uniform_int_distribution<Torus32> distrib(INT32_MIN, INT32_MAX);

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

    for (int i = 0; i < N; i++)
        result->setCoefficient(i, distrib(gen));
}
