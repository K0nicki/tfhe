#include "../fft/SPQLIOS/spqlios-fft.h"
#include "../../include/general/polyfunctions.h"
#include <cassert>

template <int32_t N>
void torusPolyfft(FftPoly *result, std::array<int32_t, N> *poly)
{
    fftplvl1.execute_reverse_torus32(result->getArray(), poly->data());
}

// void intPolyfft(FftPoly* result, IntPolynomial* poly) {
//     fftplvl1.execute_reverse_int(result->getArray(), poly->getCoef());
// }

template <int32_t N>
void torusPolyifft(std::array<int32_t, N> *result, FftPoly *poly)
{
    fftplvl1.execute_direct_torus32(result->data(), poly->getArray());
}

FftPoly mulPolyFD(FftPoly *a, FftPoly *b)
{
    int N = a->getLength();
    FftPoly result;

    for (int i = 0; i < N / 2; i++)
    {
        double aImg_bImg = a->getCoef(i + N / 2) * b->getCoef(i + N / 2);
        double aReal_bImg = a->getCoef(i) * b->getCoef(i + N / 2);

        (&result)->setCoef(i, a->getCoef(i) * b->getCoef(i) - aImg_bImg);
        (&result)->setCoef(i + N / 2, a->getCoef(i + N / 2) * b->getCoef(i) + aReal_bImg);
    }

    return result;
}

void fmaPolyFD(FftPoly *res, FftPoly *a, FftPoly *b)
{
    int N = b->getLength();

    for (int i = 0; i < N / 2; i++)
    {
        double aImg_bImg_mRes = a->getCoef(i + (N / 2)) * b->getCoef(i + N / 2) - res->getCoef(i);
        double aReal_bReal_mRes = a->getCoef(i) * b->getCoef(i) - res->getCoef(i);
        double aReal_bImg_pRes = a->getCoef(i) * b->getCoef(i + N / 2) + res->getCoef(i + N / 2);
        double aImg_bReal_pRes = a->getCoef(i + N / 2) * b->getCoef(i) + res->getCoef(i + N / 2);

        res->setCoef(i, aImg_bImg_mRes);
        res->setCoef(i, aReal_bReal_mRes);
        res->setCoef(i + (N / 2), aReal_bImg_pRes);
        res->setCoef(i + (N / 2), aImg_bReal_pRes);
    }
}

// Result = Source * (X^p - 1)
void polyMulByX_iMinOne(TorusPolynomial *result, TorusPolynomial *source, int32_t p)
{
    assert(result->getLenght() == source->getLenght());
    int32_t N = source->getLenght();

    if (p < N)
    {
        for (int i = 0; i < p; i++)
            result->setCoefficient(i, -source->getCoef(N - p + i) - source->getCoef(i));
        for (int i = p; i < N; i++)
            result->setCoefficient(i, source->getCoef(i - p) - source->getCoef(i));
    }
    else
    {
        p = p - N; // Because of modulo operation
        for (int i = 0; i < p; i++)
            result->setCoefficient(i, source->getCoef(N - p + i) - source->getCoef(i));
        for (int i = p; i < N; i++)
            result->setCoefficient(i, -source->getCoef(i - p) - source->getCoef(i));
    }
}

// Result = Source * X^p
void polyMulByX_i(TorusPolynomial *result, TorusPolynomial *source, int32_t p)
{
    assert(result->getLenght() == source->getLenght());
    int32_t N = source->getLenght();

    // if (p < N)
    // {
    //     for (int i = 0; i < p; i++)
    //         result->setCoefficient(i, -source->getCoef(N - p + i));
    //     for (int i = p; i < N; i++)
    //         result->setCoefficient(i, source->getCoef(i - p));
    // }
    // else
    // {
    //     uint32_t pp = p - N; // Because of modulo operation
    //     for (int i = 0; i < pp; i++)
    //         result->setCoefficient(i, source->getCoef(N - pp + i));
    //     for (int i = pp; i < N; i++)
    //         result->setCoefficient(i, -source->getCoef(i - pp));
    // }

    if (p < N)
    {
        for (int i = 0; i < (N - p); i++)
            result->setCoefficient(i + p, source->getCoef(i));
        for (int i = (N - p); i < N; i++)
            result->setCoefficient(i + p - N, -source->getCoef(i));

        // for (int i = 0; i < p; i++)
        //     result->setCoefficient(N - p + i, -source->getCoef(i));
        // for (int i = 1; i <= N - p; i++)
        //     result->setCoefficient(N - p - i, source->getCoef(N - i));
    }
    else
    {
        int32_t pp = p - N; // Because of modulo operation
        // Do exactly the same as above but change the sign
        for (int i = 0; i < (N - pp); i++)
            result->setCoefficient(i + pp, -source->getCoef(i));
        for (int i = (N - pp); i < N; i++)
            result->setCoefficient(i + pp - N, source->getCoef(i));

        // for (int i = 0; i < pp; i++)
        //     result->setCoefficient(N - pp + i, source->getCoef(i));
        // for (int i = 1; i <= N - pp; i++)
        //     result->setCoefficient(N - pp - i, -source->getCoef(N - i));
    }
}

void torusPolyAddTo(TorusPolynomial *result, TorusPolynomial *a)
{
    int32_t N = a->getLenght();
    int32_t r;
    int32_t b;

    for (int i = 0; i < N; i++)
    {
        r = result->getCoef(i);
        b = a->getCoef(i);
        result->setCoefficient(i, r + b);
    }
}

void torusPolyAdd(TorusPolynomial *result, TorusPolynomial *a, TorusPolynomial *b)
{
    int32_t N = a->getLenght();
    int32_t aVal;
    int32_t bVal;

    for (int i = 0; i < N; i++)
    {
        aVal = a->getCoef(i);
        bVal = b->getCoef(i);
        result->setCoefficient(i, aVal + bVal);
    }
}

void torusPolySubTo(TorusPolynomial *result, TorusPolynomial *a)
{
    int32_t N = a->getLenght();
    int32_t r;
    int32_t b;

    for (int i = 0; i < N; i++)
    {
        r = result->getCoef(i);
        b = a->getCoef(i);
        result->setCoefficient(i, r - b);
    }
}

bool torusPolyEQ(TorusPolynomial *a, TorusPolynomial *b)
{
    int32_t N = a->getLenght();
    bool result = true;

    if (a->getLenght() != b->getLenght())
        result = !result;

    for (int j = 0; j < N && result; j++)
        if (a->getCoef(j) != b->getCoef(j))
            result = !result;

    return result;
}

void torusPolyClear(TorusPolynomial *result)
{
    int32_t N = result->getLenght();

    for (int j = 0; j < N; j++)
        result->setCoefficient(j, 0);
}

void torusPolySub(TorusPolynomial *result, TorusPolynomial *a, TorusPolynomial *b)
{
    int32_t N = a->getLenght();
    int32_t aVal;
    int32_t bVal;

    for (int i = 0; i < N; i++)
    {
        aVal = a->getCoef(i);
        bVal = b->getCoef(i);
        result->setCoefficient(i, aVal - bVal);
    }
}

void torusPolyCopy(TorusPolynomial *result, TorusPolynomial *a)
{
    int32_t N = a->getLenght();

    for (int i = 0; i < N; i++)
        result->setCoefficient(i, a->getCoef(i));
}

void torusPolyMulFD(TorusPolynomial *result, IntPolynomial *poly1, TorusPolynomial *poly2)
{
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
    fft1 = mulPolyFD(&fft1, &fft2);

    // Do inverse fft
    torusPolyifft<DEF_N>(tmp.getCoefAsArray(), &fft1);

    torusPolyAddTo(result, &tmp);
}

void torusPolyMulSubFD(TorusPolynomial *result, IntPolynomial *poly1, TorusPolynomial *poly2)
{
    int32_t N = result->getLenght();
    FftPoly fft1;
    FftPoly fft2;
    TorusPolynomial tmp(N);

    // Go to the frequency domain
    torusPolyfft<DEF_N>(&fft1, poly1->getCoefAsArray());
    torusPolyfft<DEF_N>(&fft2, poly2->getCoefAsArray());

    // Multiply vectors
    fft1 = mulPolyFD(&fft1, &fft2);

    // Do inverse fft
    torusPolyifft<DEF_N>((&tmp)->getCoefAsArray(), &fft1);

    torusPolySubTo(result, &tmp);
}

void torusPolyMakeRandom(TorusPolynomial *result)
{
    int32_t N = result->getLenght();
    std::uniform_int_distribution<Torus32> distrib(INT32_MIN, INT32_MAX);

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

    for (int i = 0; i < N; i++)
        result->setCoefficient(i, distrib(gen));
}

double intPolySq2(IntPolynomial *poly)
{
    int32_t result = 0;

    for (int i = 0; i < DEF_N; i++)
    {
        int32_t tmp = poly->getCoef(i) * poly->getCoef(i);
        result += tmp;
    }

    return result;
}