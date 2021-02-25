#pragma once

#include "LiveStreamingCore.h"

LiveStreamingCore* lsc;

extern "C" 
{
    __declspec(dllexport) int test(void);
    __declspec(dllexport) sdimension* init(void);
    __declspec(dllexport) sdecoded_frame* live_streaming(void);
    __declspec(dllexport) int close(void);
}


