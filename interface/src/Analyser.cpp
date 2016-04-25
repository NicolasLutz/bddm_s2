#include "Analyser.h"
extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}

Analyser::Analyser(): m_videoAnalysers()
{}

Analyser::~Analyser()
{}

void Analyser::cbVideoPostrender(uint8_t * p_pixel_buffer, int width, int height, int pixel_pitch)
{
    for(ASubAnalyser *asa : m_videoAnalysers)
    {
        if(asa!=NULL)
            asa->updateVideoDetection(p_pixel_buffer, width, height, pixel_pitch);
    }
}

void Analyser::addAnalyser(ASubAnalyser *analyser)
{
    m_videoAnalysers.push_back(analyser);
}

void Analyser::produceOutputs()
{
    for(ASubAnalyser *asa : m_videoAnalysers)
    {
        if(asa!=NULL)
            asa->produceOutput();
    }
}

void Analyser::analyze_video(const QString& f) {
    // "God function", should be broken up in smaller chunks, but won't be because reasons
    AVFormatContext* av_context = nullptr;

    // Read input stream header
    if(avformat_open_input(&av_context, f.toStdString().c_str(), nullptr, nullptr))
        throw std::runtime_error("Media file could not be opened");
    if(avformat_find_stream_info(av_context, nullptr) < 0) {
        avformat_close_input(&av_context);
        throw std::runtime_error("Cannot obtain stream information (is the file a valid media file?)");
    }

    // File seems valid, let's find a video stream
    int stream_index = -1;
    for(int i = 0 ; i != av_context->nb_streams ; ++i) {
        if(av_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            stream_index = i;
            break;
        }
    }

    if(stream_index == -1) {
        avformat_close_input(&av_context);
        throw std::runtime_error("No video streams found in file");
    }

    // Cool, we got a stream. Find the codec and open it.
    AVCodecContext* codec_context_orig = av_context->streams[stream_index]->codec;
    AVCodec* codec = avcodec_find_decoder(codec_context_orig->codec_id);
    if(!codec) {
        avformat_close_input(&av_context);
        throw std::runtime_error("Unsupported video codec");
    }
    AVCodecContext* codec_context = avcodec_alloc_context3(codec);
    if(avcodec_copy_context(codec_context, codec_context_orig)) {
        avformat_close_input(&av_context);
        throw std::runtime_error("Internal FFmpeg error (cannot copy codec context)");
    }

    if(avcodec_open2(codec_context, codec, nullptr)) {
        avformat_close_input(&av_context);
        throw std::runtime_error("Internal FFmpeg error (cannot load codec)");
    }

    // So, we got a codec. Prepare buffers for reading and initialize swscale
    AVFrame *framebuf = av_frame_alloc(), *frame_rgb = av_frame_alloc();
    if(!framebuf || ! frame_rgb) {
        avformat_close_input(&av_context);
        throw std::runtime_error("Internal FFmpeg error (cannot allocate frame)");
    }
    uint8_t* buf = reinterpret_cast<uint8_t*>(av_malloc(avpicture_get_size(PIX_FMT_RGB24, codec_context->width, codec_context->height)));
    avpicture_fill(reinterpret_cast<AVPicture*>(frame_rgb), buf, PIX_FMT_RGB24, codec_context->width, codec_context->height);
    struct SwsContext* swscale_context = sws_getContext(codec_context->width, codec_context->height, codec_context->pix_fmt,
                                                        codec_context->width, codec_context->height, PIX_FMT_RGB24,
                                                        SWS_BILINEAR, nullptr, nullptr, nullptr);

    // C'mon, let's do this thing!
    AVPacket p;
    while(av_read_frame(av_context, &p) == 0) {
        int frame_decoded = 0;
        if(p.stream_index == stream_index) {
            avcodec_decode_video2(codec_context, framebuf, &frame_decoded, &p);
            if(frame_decoded) {
                sws_scale(swscale_context, framebuf->data,
                          framebuf->linesize, 0, codec_context->height,
                          frame_rgb->data, frame_rgb->linesize);
                cbVideoPostrender(frame_rgb->data[0], codec_context->width, codec_context->height, 24);
                av_free_packet(&p);
            }
        }
    }

    // Cleanup our mess
    av_free(buf);
    av_free(framebuf);
    av_free(frame_rgb);
    avcodec_close(codec_context);
    avcodec_close(codec_context_orig);
    avformat_close_input(&av_context);
}
