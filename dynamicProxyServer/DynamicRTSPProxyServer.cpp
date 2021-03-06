//
// Created by ubuntu on 2021/9/1.
//

#include <ProxyServerMediaSession.hh>
#include "DynamicRTSPProxyServer.h"


DynamicRTSPProxyServer *DynamicRTSPProxyServer::createNew(UsageEnvironment &env, Port ourPort,
                                                          UserAuthenticationDatabase *authDatabase,
                                                          unsigned int reclamationSeconds) {
    int ourSocketIPv4 = setUpOurSocket(env, ourPort, AF_INET);
    int ourSocketIPv6 = setUpOurSocket(env, ourPort, AF_INET6);
    if (ourSocketIPv4 < 0 && ourSocketIPv6 < 0) return NULL;

    return new DynamicRTSPProxyServer(env, ourSocketIPv4, ourSocketIPv6, ourPort,
                                      authDatabase, reclamationSeconds);
}

DynamicRTSPProxyServer::DynamicRTSPProxyServer(UsageEnvironment &env, int ourSocketIPv4, int ourSocketIPv6,
                                               Port ourPort, UserAuthenticationDatabase *authDatabase,
                                               unsigned reclamationTestSeconds) : RTSPServer(env, ourSocketIPv4,
                                                                                             ourSocketIPv6, ourPort,
                                                                                             authDatabase,
                                                                                             reclamationTestSeconds) {
}

DynamicRTSPProxyServer::~DynamicRTSPProxyServer() {
}

void DynamicRTSPProxyServer::lookupServerMediaSession(const char *streamName,
                                                      lookupServerMediaSessionCompletionFunc *completionFunc,
                                                      void *completionClientData, Boolean isFirstLookupInSession) {
    char const *proxiedStreamURL = "rtsp://192.168.10.180:554/Streaming/Channels/101";
    ServerMediaSession *sms = NULL;
    sms = getServerMediaSession(streamName);
    Boolean const smsExists = sms != NULL;
    if (!smsExists) {
        char flag;
        sms = ProxyServerMediaSession::createNew(envir(), this,
                                                 proxiedStreamURL,
                                                 &flag, streamName,
                                                 "admin", "123qweasd",
                                                 0, 5,
                                                 -1, NULL);
        envir().taskScheduler().doEventLoop(&flag);
        this->addServerMediaSession(sms);
    }
    completionFunc(completionClientData, sms);
}