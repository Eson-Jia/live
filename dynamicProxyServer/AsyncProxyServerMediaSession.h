//
// Created by ubuntu on 2021/9/3.
//

#ifndef LIVE_ASYNCPROXYSERVERMEDIASESSION_H
#define LIVE_ASYNCPROXYSERVERMEDIASESSION_H

#include "ProxyServerMediaSession.hh"

class AsyncProxyRTSPClient: public ProxyRTSPClient{
private:
    friend class AsyncProxyServerMediaSession;
    friend class AsyncProxyServerMediaSubsession;
    Boolean  fDoneDESCRIBE;
};

class AsyncProxyServerMediaSubsession: public ProxyServerMediaSubsession{

};

class AsyncProxyServerMediaSession : public ProxyServerMediaSession {
public:
    static AsyncProxyServerMediaSession *createNew(UsageEnvironment &env,
                                                   GenericMediaServer *ourMediaServer, // Note: We can be used by just one server
                                                   char const *inputStreamURL, // the "rtsp://" URL of the stream we'll be proxying
                                                   char const *streamName = NULL,
                                                   char const *username = NULL,
                                                   char const *password = NULL,
                                                   Boolean *finishedDescribe = NULL,
                                                   portNumBits tunnelOverHTTPPortNum = 0,
                                                   int verbosityLevel = 0,
                                                   int socketNumToServer = -1,
                                                   MediaTranscodingTable *transcodingTable = NULL);

    ~AsyncProxyServerMediaSession();

protected:


    AsyncProxyServerMediaSession(UsageEnvironment
                                 &env,
                                 GenericMediaServer *ourMediaServer,
                                 char const *inputStreamURL,
                                 char const *streamName,
                                 char const *username,
                                 char const *password,
                                 portNumBits
                                 tunnelOverHTTPPortNum,
                                 int verbosityLevel,
                                 int socketNumToServer,
                                 MediaTranscodingTable *transcodingTable,
                                 Boolean *finishedDescribe,
                                 createNewProxyRTSPClientFunc *ourCreateNewProxyRTSPClientFunc
                                 = defaultCreateNewProxyRTSPClientFunc,
                                 portNumBits initialPortNum = 6970,
                                 Boolean
                                 multiplexRTCPWithRTP = False);


private:
    friend class ProxyRTSPClient;

    friend class ProxyServerMediaSubsession;

    void continueAfterDESCRIBE(char const *sdpDescription);

    void resetDESCRIBEState(); // undoes what was done by "contineAfterDESCRIBE()"

private:
    int fVerbosityLevel;

    class PresentationTimeSessionNormalizer *fPresentationTimeSessionNormalizer;

    createNewProxyRTSPClientFunc *fCreateNewProxyRTSPClientFunc;
    MediaTranscodingTable *fTranscodingTable;
    portNumBits fInitialPortNum;
    Boolean fMultiplexRTCPWithRTP;
    Boolean *fFinishedDescribe;
};


#endif //LIVE_ASYNCPROXYSERVERMEDIASESSION_H
