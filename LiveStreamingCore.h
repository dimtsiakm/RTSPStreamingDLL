#pragma once

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}
#undef main
#include <fstream>
#include <iostream>
#include <sstream>
#include <iostream>

using namespace std;

struct svideoparams {
	int width;
	int height;
	double fps;
};
struct sdecoded_frame {
	uint8_t* data; //data as rgb or yuv format; set the dependencies
	int linesize;
	int width;
	int height;
};
class LiveStreamingCore
{
	public:
		svideoparams* init(char* filename);
		int init_debug();
		sdecoded_frame* live_streaming();
		int close();

		AVCodec* c;
		SwsContext* img_convert_ctx;
		AVFormatContext* context;
		AVCodecContext* ccontext;
		AVPacket packet;
		AVFormatContext* oc;
		AVStream* stream;
		AVCodec* codec;
		
		AVFrame* pic;
		AVFrame* picrgb;
		uint8_t* picture_buf;
		uint8_t* picture_buf2;
		int size;
		int size2;
		int video_stream_index;
		sdecoded_frame* frame_export;
};
