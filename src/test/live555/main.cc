#include <iostream>

#include "BasicUsageEnvironment.hh"
#include "dynamic_rtsp_server.hh"

#define MEDIA_SERVER_VERSION_STRING "1.00"

using namespace std;

int main(int argc, char *argv[]) {
  TaskScheduler *scheduler = BasicTaskScheduler::createNew();
  UsageEnvironment *env = BasicUsageEnvironment::createNew(*scheduler);

  UserAuthenticationDatabase *auth_db = new UserAuthenticationDatabase();
  auth_db->addUserRecord("user", "passwd");

  RTSPServer *rtsp_server;
  portNumBits rtsp_server_port = 554;
  rtsp_server = DynamicRTSPServer::createNew(*env, rtsp_server_port, auth_db);
  if (nullptr == rtsp_server) {
    rtsp_server_port = 8554;
    rtsp_server = DynamicRTSPServer::createNew(*env, rtsp_server_port, auth_db);
  }
  if (nullptr == rtsp_server) {
    *env << "Failed to create RTSP Server:" << env->getResultMsg() << "\n";
    exit(1);
  }
  *env << "Live555 Media Server\n";
  *env << "\tversion " << MEDIA_SERVER_VERSION_STRING
       << " (LIVE555 Streaming Media library version "
       << LIVEMEDIA_LIBRARY_VERSION_STRING << ").\n";

  const char *prefix = rtsp_server->rtspURLPrefix();
  *env << "Play streams from this server using the URL\n\t" << prefix
       << "<filename>\nwhere <filename> is a file present in the current "
          "directory.\n";
  if (rtsp_server->setUpTunnelingOverHTTP(80) ||
      rtsp_server->setUpTunnelingOverHTTP(8000) ||
      rtsp_server->setUpTunnelingOverHTTP(8080)) {
    *env << "(We use port " << rtsp_server->httpServerPortNum()
         << " for optional RTSP-over-HTTP tunneling, or for HTTP live "
            "streaming (for indexed Transport Stream files only).)\n";
  } else {
    *env << "(RTSP-over-HTTP tunneling is not available.)\n";
  }
  env->taskScheduler().doEventLoop();

  return 0;
}