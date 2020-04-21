#include "dynamic_rtsp_server.hh"

#include <liveMedia.hh>

using namespace std;

static ServerMediaSession *CreateNewSMS(UsageEnvironment &env, string file_name,
                                        FILE *fid) {
  size_t pos = file_name.rfind(".");
  if (pos == string::npos) {
    env << "not find extension of " << file_name.c_str() << "\n";
    return nullptr;
  }
  std::string extension = file_name.substr(pos);
  env << "extension:" << extension.c_str() << "\n";

  ServerMediaSession *sms = nullptr;

  bool reuseSource = false;
  if (extension.compare(".aac") == 0) {
    sms = ServerMediaSession::createNew(env, file_name.c_str(),
                                        file_name.c_str(), "AAC Audio");
    sms->addSubsession(ADTSAudioFileServerMediaSubsession::createNew(
        env, file_name.c_str(), reuseSource));
  } else if (extension.compare(".264") == 0) {
    sms = ServerMediaSession::createNew(env, file_name.c_str(),
                                        file_name.c_str(), "H.264");
    OutPacketBuffer::maxSize = 100000;
    sms->addSubsession(H264VideoFileServerMediaSubsession::createNew(
        env, file_name.c_str(), reuseSource));
  } else if (extension.compare(".265") == 0) {
    sms = ServerMediaSession::createNew(env, file_name.c_str(),
                                        file_name.c_str(), "H.265");
    sms->addSubsession(H265VideoFileServerMediaSubsession::createNew(
        env, file_name.c_str(), reuseSource));
  }
  return sms;
}

DynamicRTSPServer *DynamicRTSPServer::createNew(
    UsageEnvironment &env, Port port, UserAuthenticationDatabase *auth_database,
    unsigned reclamation_seconds) {
  int socket = setUpOurSocket(env, port);
  if (socket == -1) return nullptr;

  return new DynamicRTSPServer(env, socket, port, auth_database,
                               reclamation_seconds);
}

DynamicRTSPServer::DynamicRTSPServer(UsageEnvironment &env, int socket,
                                     Port port,
                                     UserAuthenticationDatabase *auth_database,
                                     unsigned reclamation_seconds)
    : RTSPServer(env, socket, port, auth_database, reclamation_seconds) {}
DynamicRTSPServer::~DynamicRTSPServer() {}

ServerMediaSession *DynamicRTSPServer::lookupServerMediaSession(
    char const *stream_name, Boolean is_first_lookup_in_session) {
  FILE *fid = fopen(stream_name, "rb");
  bool file_exits = fid != nullptr;

  ServerMediaSession *sms = RTSPServer::lookupServerMediaSession(stream_name);
  bool sms_exits = sms != nullptr;

  if (!file_exits) {
    if (sms_exits) {
      removeServerMediaSession(sms);
      sms = nullptr;
    }
    return nullptr;
  } else {
    if (sms_exits && is_first_lookup_in_session) {
      removeServerMediaSession(sms);
      sms = nullptr;
    }
  }
  if (sms == nullptr) {
    sms = CreateNewSMS(envir(), stream_name, fid);
    addServerMediaSession(sms);
  }
  fclose(fid);
  return sms;
}