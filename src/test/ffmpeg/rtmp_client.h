#include <iostream>
#include <string>
#ifdef __cplusplus
extern "C" {
#endif
#include <libavformat/avformat.h>
#include <libavutil/mathematics.h>
#include <libavutil/time.h>
#ifdef __cplusplus
};
#endif

class RTMPClient {
 public:
  RTMPClient();
  virtual ~RTMPClient();
  int Initialize();
  void Deinitialize();
  int OpenInput(std::string in_filename, std::string out_fmt, std::string out_filename);

 private:
  AVPacket* av_packet;
  AVStream* istream;
  AVStream* ostream;
  AVFormatContext* ifmt_ctx;
  AVFormatContext* ofmt_ctx;
  AVCodec* in_codec;
  AVOutputFormat* ofmt;
  int video_index;
  int audio_index;
  AVRational frame_rate;
  double duration;
};