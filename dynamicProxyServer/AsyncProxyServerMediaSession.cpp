//
// Created by ubuntu on 2021/9/3.
//

#include "AsyncProxyServerMediaSession.h"


AsyncProxyServerMediaSession::AsyncProxyServerMediaSession(
        UsageEnvironment
        &env,
        GenericMediaServer *ourMediaServer,
        char const *inputStreamURL,
        char const *streamName,
        char const *username,
        char const *password,
        portNumBits tunnelOverHTTPPortNum,
        int verbosityLevel,
        int socketNumToServer,
        MediaTranscodingTable *transcodingTable,
        Boolean *finishedDescribe,
        createNewProxyRTSPClientFunc *ourCreateNewProxyRTSPClientFunc = defaultCreateNewProxyRTSPClientFunc,
        portNumBits initialPortNum = 6970,
        Boolean multiplexRTCPWithRTP = False) : ProxyServerMediaSession(
        env, ourMediaServer, inputStreamURL, streamName, username, password, tunnelOverHTTPPortNum, verbosityLevel,
        socketNumToServer,
        transcodingTable, ourCreateNewProxyRTSPClientFunc, initialPortNum, multiplexRTCPWithRTP
) {
    fFinishedDescribe = finishedDescribe;
}

AsyncProxyServerMediaSession *
AsyncProxyServerMediaSession::createNew(UsageEnvironment &env, GenericMediaServer *ourMediaServer,
                                        char const *inputStreamURL, char const *streamName,
                                        char const *username, char const *password,
                                        Boolean *finishedDescribe, portNumBits tunnelOverHTTPPortNum,
                                        int verbosityLevel, int socketNumToServer,
                                        MediaTranscodingTable *transcodingTable) {
    return new AsyncProxyServerMediaSession(env, ourMediaServer, inputStreamURL, streamName, username, password,
                                            tunnelOverHTTPPortNum, verbosityLevel, socketNumToServer,
                                            transcodingTable, finishedDescribe);
}


void AsyncProxyServerMediaSession::continueAfterDESCRIBE(char const *sdpDescription) {
    escribeCompletedFlag = 1;

    // Create a (client) "MediaSession" object from the stream's SDP description ("resultString"), then iterate through its
    // "MediaSubsession" objects, to set up corresponding "ServerMediaSubsession" objects that we'll use to serve the stream's tracks.
    do {
        fClientMediaSession = MediaSession::createNew(envir(), sdpDescription);
        if (fClientMediaSession == NULL) break;

        MediaSubsessionIterator iter(*fClientMediaSession);
        for (MediaSubsession *mss = iter.next(); mss != NULL; mss = iter.next()) {
            if (!allowProxyingForSubsession(*mss)) continue;

            ServerMediaSubsession *smss
                    = new ProxyServerMediaSubsession(*mss, fInitialPortNum, fMultiplexRTCPWithRTP);
            addSubsession(smss);
            if (fVerbosityLevel > 0) {
                envir() << *this << " added new \"ProxyServerMediaSubsession\" for "
                        << mss->protocolName() << "/" << mss->mediumName() << "/" << mss->codecName() << " track\n";
            }
        }
    } while (0);
    if (fProxyRTSPClient->fDoneDESCRIBE == False && fFinishedDescribe != NULL) {
        *fFinishedDescribe = True;
    }
}