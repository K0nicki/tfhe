#include "../../include/tgsw/TGSWParams.h"

TGSWParams::TGSWParams(TLWEParams *tlweParams)
{
    this->l = DEF_l;
    this->BgBit = DEF_BgBit;
    this->Bg = DEF_Bg;
    this->mask = Bg - 1;
    this->tlweParams = tlweParams;
    this->rows = (tlweParams->getPolyAmount() + 1) * l;

    for (int i = 0; i < l; i++)
    {
        h[i] = 1U << (32 - (i + 1) * BgBit);
        offset += Bg / 2 * h[i];
    }
}

int32_t TGSWParams::getDecompositionLength() { return this->l; }
int32_t TGSWParams::getBgbit() { return this->BgBit; }
int32_t TGSWParams::getBg() { return this->Bg; }
int32_t TGSWParams::getMask() { return this->mask; }
int32_t TGSWParams::getNoRows() { return this->rows; }
int32_t *TGSWParams::getBgbitPowers() { return this->h.data(); }
uint32_t TGSWParams::getOffset() { return this->offset; }
TLWEParams *TGSWParams::getTLWEParams() { return this->tlweParams; }

