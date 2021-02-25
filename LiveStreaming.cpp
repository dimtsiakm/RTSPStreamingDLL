#include "pch.h"
#include "LiveStreaming.h"

__declspec(dllexport) sdimension* init(void) 
{
	lsc = new LiveStreamingCore();
	return lsc->init();
}

__declspec(dllexport) sdecoded_frame* live_streaming(void)
{
    return lsc->live_streaming();
}
__declspec(dllexport) int close(void)
{
    return lsc->close();
}

__declspec(dllexport) int test(void)
{
    return 102;
}