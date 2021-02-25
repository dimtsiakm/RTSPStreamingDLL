#include "pch.h"
#include "LiveStreamingCore.h"

sdimension* LiveStreamingCore::init() {
    c = NULL;
    c = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!c) exit(1);
    
    context = avformat_alloc_context();
    ccontext = avcodec_alloc_context3(c);

    av_register_all();
    avformat_network_init();

    if (avformat_open_input(&context, "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov", NULL, NULL) != 0) {
        return 0;
    }
    if (avformat_find_stream_info(context, NULL) < 0) {
        return 0;
    }
    //search video stream
    for (int i = 0; i < context->nb_streams; i++) {
        if (context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
            video_stream_index = i;
    }  
    av_init_packet(&packet);

    oc = avformat_alloc_context();

    stream = NULL;
    
    //start reading packets from stream and write them to file
    av_read_play(context);//play RTSP

    codec = NULL;
    codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!codec) exit(1);

    avcodec_get_context_defaults3(ccontext, codec);
    avcodec_copy_context(ccontext, context->streams[video_stream_index]->codec);

    if (avcodec_open2(ccontext, codec, NULL) < 0) exit(1);

    img_convert_ctx = sws_getContext(ccontext->width, ccontext->height, ccontext->pix_fmt, ccontext->width, ccontext->height,
        AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

    sdimension* s = new sdimension;
    s->height = ccontext->height;
    s->width = ccontext->width;

    int size = avpicture_get_size(AV_PIX_FMT_YUV420P, ccontext->width, ccontext->height);
    picture_buf = (uint8_t*)(av_malloc(size));
    pic = av_frame_alloc();
    picrgb = av_frame_alloc();
    int size2 = avpicture_get_size(AV_PIX_FMT_RGB24, ccontext->width, ccontext->height);
    picture_buf2 = (uint8_t*)(av_malloc(size2));
    avpicture_fill((AVPicture*)pic, picture_buf, AV_PIX_FMT_YUV420P, ccontext->width, ccontext->height);
    avpicture_fill((AVPicture*)picrgb, picture_buf2, AV_PIX_FMT_RGB24, ccontext->width, ccontext->height);

    frame_export = (sdecoded_frame*)malloc(sizeof(sdecoded_frame));

	return s;
}

sdecoded_frame* LiveStreamingCore::live_streaming() {
    while (av_read_frame(context, &packet) >= 0)
    {
        if (packet.stream_index == video_stream_index) {//packet is video
            if (stream == NULL)
            {
                stream = avformat_new_stream(oc, context->streams[video_stream_index]->codec->codec);
                avcodec_copy_context(stream->codec, context->streams[video_stream_index]->codec);
                stream->sample_aspect_ratio = context->streams[video_stream_index]->codec->sample_aspect_ratio;
            }
            int check = 0;
            packet.stream_index = stream->id;
            int result = avcodec_decode_video2(ccontext, pic, &check, &packet);
            
            sws_scale(img_convert_ctx, pic->data, pic->linesize, 0, ccontext->height, picrgb->data, picrgb->linesize);
            
            av_free_packet(&packet);
            av_init_packet(&packet);

            frame_export->data = picrgb->data[0];
            frame_export->linesize = picrgb->linesize[0];
            frame_export->width = ccontext->width;
            frame_export->height = ccontext->height;
            return frame_export;
        }
    }
    return NULL;
}

int LiveStreamingCore::close() {
    av_free(pic);
    av_free(picrgb);
    av_free(picture_buf);
    av_free(picture_buf2);
    av_read_pause(context);
    avio_close(oc->pb);
    avformat_free_context(oc);

    free(frame_export);

    return 0;
}