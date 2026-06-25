#pragma once
#include "WaveIntf.h"

#if !MY_USE_MINLIB
class FFWaveDecoderCreator : public tTVPWaveDecoderCreator
{
public:
    tTVPWaveDecoder* Create(const ttstr& storagename, const ttstr& extension);
};
#endif

