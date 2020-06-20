#include "rtmp_client.h"

#include <unistd.h>

#include <iostream>

using namespace std;

RTMPClient::RTMPClient() : ifmt_ctx(nullptr) {}
int Initialize() {
  av_register_all();
  avformat_network_init();
  return 0;
}

RTMPClient::~RTMPClient() {}
int RTMPClient::Initialize() {}
void RTMPClient::Deinitialize() {
  avformat_network_deinit();
}
int RTMPClient::OpenInput(std::string in_filename,
                          std::string out_fmt,
                          std::string out_filename) {
  video_index = -1;
  audio_index = -1;
  if (avformat_open_input(&ifmt_ctx, in_filename.c_str(), 0, 0) < 0) {
    cout << "could not open " << in_filename << " failed";
    return -1;
  }
  if (avformat_find_stream_info(ifmt_ctx, 0) < 0) {
    cout << "failed to retrieve input stream information" << endl;
    return -1;
  }

  // int video_index =
  //     av_find_best_stream(ifmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
  av_dump_format(ifmt_ctx, 0, in_filename.c_str(), 0);

  // create out stream
  avformat_alloc_output_context2(&ofmt_ctx, NULL, out_fmt.c_str(),
                                 out_filename.c_str());
  if (!ofmt_ctx) {
    cout << "could not create output context" << endl;
    return -1;
  }

  ofmt = ofmt_ctx->oformat;
  for (int i = 0; i < ifmt_ctx->nb_streams; ++i) {
    istream = ifmt_ctx->streams[i];
    if (istream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
      audio_index = i;
    }
    if (istream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
      video_index = i;
      frame_rate = av_guess_frame_rate(ifmt_ctx, istream, NULL);
      duration = av_q2d(frame_rate);
      cout << "video: frame_rate: " << frame_rate.den << "/" << frame_rate.num
           << endl;
    }

    in_codec = avcodec_find_decoder(istream->codecpar->codec_id);
    cout << in_codec->name << " " << istream->codecpar->codec_id << endl;
    ostream = avformat_new_stream(ofmt_ctx, in_codec);
    if (ostream == nullptr) {
      cout << "avformat_new_stream failed" << endl;
    }

    if (avcodec_parameters_copy(ostream->codecpar, istream->codecpar) < 0) {
      cout << "avcodec_parameters_copy failed" << endl;
    }
    ostream->codecpar->codec_tag = 0;
    ////AVFMT_GLOBALHEADER代表封装格式包含“全局头”（即整个文件的文件头），大部分封装格式是这样的。一些封装格式没有“全局头”，比如MPEG2TS
    if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER) {
      ostream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
  }
  av_dump_format(ofmt_ctx, 0, out_filename.c_str(), 1);
  if (avio_open(&ofmt_ctx->pb, out_filename.c_str(), AVIO_FLAG_WRITE) < 0) {
    cout << "avio_open failed" << endl;
    return -1;
  }
  int64_t start_time = av_gettime();
  avformat_write_header(ofmt_ctx, nullptr);
  av_packet = av_packet_alloc();

  int video_frame_count = 0;
  int video_frame_size = 0;
  int audio_frame_count = 0;
  int audio_frame_size = 0;
  int frame_index = 0;
  while (1) {
    if (av_read_frame(ifmt_ctx, av_packet) < 0) {
      cout << "read frame end" << endl;
      break;
    }
    istream = ifmt_ctx->streams[av_packet->stream_index];

    // 没有封装格式的裸流（例如H.264裸流）是不包含PTS、DTS这些参数的。在发送这种数据的时候，需要自己计算并写入AVPacket的PTS，DTS，duration等参数
    if (av_packet->pts == AV_NOPTS_VALUE) {
      //write PTS
      AVRational timebase = ifmt_ctx->streams[video_index]->time_base;
      int64_t calc_duration =
          (double)AV_TIME_BASE /
          av_q2d(ifmt_ctx->streams[video_index]->r_frame_rate);
      av_packet->pts = (double)(frame_index * calc_duration) /
                       (double)(av_q2d(timebase) * AV_TIME_BASE);
      av_packet->dts = av_packet->pts;
      av_packet->duration =
          (double)calc_duration / (double)(av_q2d(timebase) * AV_TIME_BASE);
    }

    if (av_packet->stream_index == video_index) {
      video_frame_size += av_packet->size;
      frame_index++;
      cout << "[VIDEO] recv " << ++video_frame_count << " frame "
           << av_packet->size << " - " << video_frame_size
           << " pts:" << av_packet->pts << endl;
    }

    if (av_packet->stream_index == audio_index) {
      audio_frame_size += av_packet->size;
      cout << "[AUDIO] recv " << ++audio_frame_count << " frame "
           << av_packet->size << " - " << audio_frame_size << " pts"
           << av_packet->pts << endl;
    }

    if (istream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
      AVRational dst_time_base = {1, AV_TIME_BASE};
      int64_t pts =
          av_rescale_q(av_packet->pts, istream->time_base, dst_time_base);
      int64_t now = av_gettime() - start_time;
      cout << "now:" << now << " pts:" << pts << endl;
      if (pts > now) {
        cout << "sleep:" << (pts - now) << endl;
        av_usleep(pts - now);
      }
    }

    av_packet->pts = av_rescale_q_rnd(av_packet->pts, istream->time_base, ostream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
		av_packet->dts = av_rescale_q_rnd(av_packet->dts, istream->time_base, ostream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
		av_packet->duration = av_rescale_q(av_packet->duration, istream->time_base, ostream->time_base);
    av_packet->pos = -1;

    if (av_interleaved_write_frame(ofmt_ctx, av_packet) < 0) {
      cout << "av_interleaved_write_frame failed" << endl;
      break;
    }
    av_packet_unref(av_packet);
  }
  av_write_trailer(ofmt_ctx);
  av_packet_free(&av_packet);
  avformat_close_input(&ifmt_ctx);
  avio_close(ofmt_ctx->pb);
  avformat_free_context(ofmt_ctx);
  return 0;
}