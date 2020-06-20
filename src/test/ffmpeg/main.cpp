#include <iostream>
#include <memory>
#include <string>

#include "rtmp_client.h"
using namespace std;

int main(int argc, char* argv[]) {
  std::shared_ptr<RTMPClient> rtmp_client = make_shared<RTMPClient>();
  rtmp_client->Initialize();

  string in_filename = "test.flv";
  string out_fmt = "flv";
  string out_filename = "rtmp://127.0.0.1:1935/live/test264";

  rtmp_client->OpenInput(in_filename, out_fmt, out_filename);

  rtmp_client->Deinitialize();
  return 0;
}