#include "VideoExporter.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <filesystem>

namespace t2c {
namespace fs = std::filesystem;

bool VideoExporter::exportToMP4(const std::string& frameDir,
                                 const std::string& /*framePattern*/,
                                 const std::string& outputPath,
                                 int fps,
                                 int bitrate_kbps)
{
    int frameCount = 0;
    while (true) {
        std::ostringstream ss;
        ss << frameDir << "/frame_"
           << std::setw(6) << std::setfill('0') << frameCount << ".png";
        if (!fs::exists(ss.str())) break;
        ++frameCount;
    }

    if (frameCount == 0) {
        m_lastError = "No frames found in " + frameDir;
        std::cerr << "[VideoExporter] " << m_lastError << "\n";
        return false;
    }

    std::cout << "[VideoExporter] Encoding " << frameCount
              << " frames -> " << outputPath << "\n";

    std::ostringstream firstPath;
    firstPath << frameDir << "/frame_000000.png";
    int imgW=0, imgH=0, imgCh=0;
    {
        stbi_uc* probe = stbi_load(firstPath.str().c_str(),
                                   &imgW, &imgH, &imgCh, 3);
        if (!probe) {
            m_lastError = "Cannot read first frame";
            return false;
        }
        stbi_image_free(probe);
    }

    const AVCodec* codec = avcodec_find_encoder_by_name("libx264");
    if (!codec) codec    = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        m_lastError = "H264 encoder not found";
        return false;
    }

    AVFormatContext* fmtCtx = nullptr;
    avformat_alloc_output_context2(&fmtCtx,nullptr,nullptr,outputPath.c_str());
    if (!fmtCtx) { m_lastError="Cannot create format context"; return false; }

    AVStream* stream = avformat_new_stream(fmtCtx, codec);
    AVCodecContext* codecCtx = avcodec_alloc_context3(codec);

    codecCtx->width      = imgW;
    codecCtx->height     = imgH;
    codecCtx->time_base  = {1, fps};
    codecCtx->framerate  = {fps, 1};
    codecCtx->pix_fmt    = AV_PIX_FMT_YUV420P;
    codecCtx->bit_rate   = static_cast<int64_t>(bitrate_kbps)*1000;
    codecCtx->gop_size   = 10;
    codecCtx->max_b_frames=1;

    if (fmtCtx->oformat->flags & AVFMT_GLOBALHEADER)
        codecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    av_opt_set(codecCtx->priv_data,"preset","fast",0);
    av_opt_set(codecCtx->priv_data,"crf","23",0);
    avcodec_open2(codecCtx, codec, nullptr);
    avcodec_parameters_from_context(stream->codecpar, codecCtx);
    stream->time_base = codecCtx->time_base;

    if (!(fmtCtx->oformat->flags & AVFMT_NOFILE))
        avio_open(&fmtCtx->pb, outputPath.c_str(), AVIO_FLAG_WRITE);

    avformat_write_header(fmtCtx, nullptr);

    SwsContext* swsCtx = sws_getContext(imgW,imgH,AV_PIX_FMT_RGB24,
                                         imgW,imgH,AV_PIX_FMT_YUV420P,
                                         SWS_BILINEAR,nullptr,nullptr,nullptr);

    AVFrame* frame = av_frame_alloc();
    frame->format  = AV_PIX_FMT_YUV420P;
    frame->width   = imgW;
    frame->height  = imgH;
    av_image_alloc(frame->data,frame->linesize,imgW,imgH,AV_PIX_FMT_YUV420P,32);

    AVPacket* pkt = av_packet_alloc();

    for (int i=0; i<frameCount; ++i) {
        std::ostringstream fp;
        fp << frameDir << "/frame_"
           << std::setw(6) << std::setfill('0') << i << ".png";
        int w,h,ch;
        stbi_uc* rgb = stbi_load(fp.str().c_str(),&w,&h,&ch,3);
        if (!rgb) continue;
        const uint8_t* src[1]={rgb};
        int stride[1]={w*3};
        sws_scale(swsCtx,src,stride,0,h,frame->data,frame->linesize);
        stbi_image_free(rgb);
        frame->pts=i;
        if (avcodec_send_frame(codecCtx,frame)>=0) {
            while(avcodec_receive_packet(codecCtx,pkt)>=0){
                av_packet_rescale_ts(pkt,codecCtx->time_base,stream->time_base);
                pkt->stream_index=stream->index;
                av_interleaved_write_frame(fmtCtx,pkt);
                av_packet_unref(pkt);
            }
        }
        if(i%30==0)
            std::cout<<"[VideoExporter] Frame "<<i<<"/"<<frameCount<<"\r"<<std::flush;
    }

    avcodec_send_frame(codecCtx,nullptr);
    while(avcodec_receive_packet(codecCtx,pkt)>=0){
        av_packet_rescale_ts(pkt,codecCtx->time_base,stream->time_base);
        pkt->stream_index=stream->index;
        av_interleaved_write_frame(fmtCtx,pkt);
        av_packet_unref(pkt);
    }

    av_write_trailer(fmtCtx);
    av_frame_free(&frame);
    av_packet_free(&pkt);
    sws_freeContext(swsCtx);
    avcodec_free_context(&codecCtx);
    if(!(fmtCtx->oformat->flags&AVFMT_NOFILE)) avio_closep(&fmtCtx->pb);
    avformat_free_context(fmtCtx);

    std::cout<<"\n[VideoExporter] Done -> "<<outputPath<<"\n";
    return true;
}

} // namespace t2c