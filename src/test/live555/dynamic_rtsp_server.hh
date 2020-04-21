#ifndef DYNAMIC_RTSP_SERVER_HH
#define DYNAMIC_RTSP_SERVER_HH

#include <string>

#include "RTSPServer.hh"

class DynamicRTSPServer : public RTSPServer {
 public:
  static DynamicRTSPServer *createNew(UsageEnvironment &env, Port port,
                                      UserAuthenticationDatabase *auth_database,
                                      unsigned reclamation_seconds = 65);

 protected:
  DynamicRTSPServer(UsageEnvironment &env, int socket, Port port,
                    UserAuthenticationDatabase *auth_database,
                    unsigned reclamation_seconds);
  virtual ~DynamicRTSPServer();

 protected:
  ServerMediaSession *lookupServerMediaSession(
      char const *stream_name, Boolean is_first_lookup_in_session);
};

#endif