#pragma once
#include "pch.h"
#include "LiveStreamingCore.h"

LiveStreamingCore* lsc;

extern "C" __declspec(dllexport) int test(int a, int b);
extern "C" __declspec(dllexport) sdimension* init(void) {
    lsc = new LiveStreamingCore();
    return lsc->init();
};
extern "C" __declspec(dllexport) sdecoded_frame* live_streaming(void) {
    return lsc->live_streaming();
}
extern "C" __declspec(dllexport) int close(void) {
    return lsc->close();
}


