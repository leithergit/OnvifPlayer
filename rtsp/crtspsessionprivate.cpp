#include "crtspsessionprivate.h"
#include "crtspsession.h"
#include "GroupsockHelper.hh"
#include "RTSPCommon.hh"
int CRTSPSessionPrivate::rtspClientCount = 0;

void CRTSPSessionPrivate::continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString)
{
    do
    {
        UsageEnvironment& env = rtspClient->envir(); // alias
        StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

        if (resultCode != 0)
        {
            env /*<< *rtspClient*/ << "Failed to get a SDP description: " << resultString << "\n";
            break;
        }

        char* const sdpDescription = resultString;
        env /*<< *rtspClient*/ << "Got a SDP description:\n" << sdpDescription << "\n";

        // Create a media session object from this SDP description:
        scs.session = MediaSession::createNew(env, sdpDescription);
        if (scs.session == NULL)
        {
            env /*<< *rtspClient*/ << "Failed to create a MediaSession object from the SDP description: " << env.getResultMsg() << "\n";
            break;
        }
        else if (!scs.session->hasSubsessions())
        {
            env /*<< *rtspClient*/ << "This session has no media subsessions (i.e., no \"m=\" lines)\n";
            break;
        }
        //((ourRTSPClient*)rtspClient)->session->pRtspSessionPrivate->();
        // Then, create and set up our data source objects for the session.  We do this by iterating over the session's 'subsessions',
        // calling "MediaSubsession::initiate()", and then sending a RTSP "SETUP" command, on each one.
        // (Each 'subsession' will have its own data source.)
        scs.iter = new MediaSubsessionIterator(*scs.session);
        setupNextSubsession(rtspClient);
        return;
    }
    while (0);

    if (((ourRTSPClient*)rtspClient)->session->pSessionPrivate->m_bWait)
    {
        //��һ�����:������waittime ������waittime
        ((ourRTSPClient*)rtspClient)->session->pSessionPrivate->m_nResult = -2;
        SetEvent(((ourRTSPClient*)rtspClient)->session->pSessionPrivate->m_hEvent);
        // An unrecoverable error occurred with this stream.
        //shutdownStream(rtspClient);
    }
    else
    {
        //�ڶ������:��������waittime ��������waittime
        ((ourRTSPClient*)rtspClient)->session->onDisconnect(resultCode,__FUNCTION__);
    }
}

void CRTSPSessionPrivate::setupNextSubsession(RTSPClient* rtspClient)
{
    UsageEnvironment& env = rtspClient->envir(); // alias
    StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias
    CRTSPSession* pSession = ((ourRTSPClient*)rtspClient)->session;

    scs.subsession = scs.iter->next();
    if (scs.subsession != NULL)
    {
        if (!scs.subsession->initiate())
        {
            setupNextSubsession(rtspClient); // give up on this subsession; go to the next one
        }
        else
        {
            // Continue setting up this subsession, by sending a RTSP "SETUP" command:
            rtspClient->sendSetupCommand(*scs.subsession, continueAfterSETUP,False,pSession->useTCP());

        }
        return;
    }

    if (pSession->isRtspSession())
    {
        // We've finished setting up all of the subsessions.  Now, send a RTSP "PLAY" command to start the streaming:
		
        scs.duration = scs.session->playEndTime() - scs.session->playStartTime();
		if (scs.duration == 0)
			scs.duration = 15;
        rtspClient->sendPlayCommand(*scs.session, continueAfterPLAY);
    }
    else
    {
        rtspClient->sendGetParameterCommand(*scs.session, continueAfterGetParameter,"");
    }
}

void CRTSPSessionPrivate::continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString)
{
    do
    {
        UsageEnvironment& env = rtspClient->envir(); // alias
        StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias
        CRTSPSession* pSession = ((ourRTSPClient*)rtspClient)->session;

        if (resultCode != 0)
        {
            if (pSession->pSessionPrivate->m_bWait)
            {
                pSession->pSessionPrivate->m_nResult = -503;
                SetEvent(pSession->pSessionPrivate->m_hEvent);
            }
            else
            {
                pSession->onDisconnect(resultCode, __FUNCTION__);
            }
            return;
            //break;
        }

        env /*<< *rtspClient*/ << "Set up the \"" /*<< *scs.subsession*/
                               << "\" subsession (client ports " << scs.subsession->clientPortNum() << "-" << scs.subsession->clientPortNum()+1 << ")\n";

        // Having successfully setup the subsession, create a data sink for it, and call "startPlaying()" on it.
        // (This will prepare the data sink to receive data; the actual flow of data from the client won't start happening until later,
        // after we've sent a RTSP "PLAY" command.)


        DummySink* sink = DummySink::createNew(env, *scs.subsession, rtspClient->url());
        if (pSession->isRtspSession())
        {
            sink->init();
            sink->session = ((ourRTSPClient*)rtspClient)->session;
            SDP SDP;
            SDP.nHeight = scs.subsession->fmtp_height();
            SDP.nWidth = scs.subsession->fmtp_width();
            SDP.nRate = scs.subsession->videoFPS();
            SDP.szCodec = (char *)scs.subsession->codecName();
            SDP.pMediaName = (char *)scs.subsession->mediumName();
            SDP.nSPSLen = scs.subsession->fmtp_spsLen();
            SDP.nPPSLen = scs.subsession->fmtp_ppsLen();
            SDP.nRTPPayloadFormat = scs.subsession->fmtp_rtppayloadformat();
            SDP.nRTPTimestampFrequency = scs.subsession->fmrp_rtptimestampfrequency();
            sink->SDPNotify(&SDP);
            //pSession->pRtspSessionPrivate->_rtspSDPCallBack()
        }
        scs.subsession->sink = sink;

        // perhaps use your own custom "MediaSink" subclass instead
        if (scs.subsession->sink == NULL)
        {
            env /*<< *rtspClient*/ << "Failed to create a data sink for the \"" /*<< *scs.subsession*/
                                   << "\" subsession: " << env.getResultMsg() << "\n";
            break;
        }

        env /*<< *rtspClient*/ << "Created a data sink for the \"" /*<< *scs.subsession*/ << "\" subsession\n";
        scs.subsession->miscPtr = rtspClient; // a hack to let subsession handle functions get the "RTSPClient" from the subsession
        scs.subsession->sink->startPlaying(*(scs.subsession->readSource()), subsessionAfterPlaying, scs.subsession);
        // Also set a handler to be called if a RTCP "BYE" arrives for this subsession:
        if (scs.subsession->rtcpInstance() != NULL)
        {
            scs.subsession->rtcpInstance()->setByeHandler(subsessionByeHandler, scs.subsession);
        }
    }
    while (0);

    // Set up the next subsession, if any:
    setupNextSubsession(rtspClient);
}


void CRTSPSessionPrivate::subsessionAfterPlaying(void* clientData)
{
    MediaSubsession* subsession = (MediaSubsession*)clientData;
    RTSPClient* rtspClient = (RTSPClient*)(subsession->miscPtr);

    // Begin by closing this subsession's stream:
    Medium::close(subsession->sink);
    subsession->sink = NULL;

    // Next, check whether *all* subsessions' streams have now been closed:
    MediaSession& session = subsession->parentSession();
    MediaSubsessionIterator iter(session);
    while ((subsession = iter.next()) != NULL)
    {
        if (subsession->sink != NULL) return; // this subsession is still active
    }

    // All subsessions' streams have now been closed, so shutdown the client:
    //shutdownStream(rtspClient);
}

void CRTSPSessionPrivate::subsessionByeHandler(void* clientData)
{
    MediaSubsession* subsession = (MediaSubsession*)clientData;
    RTSPClient* rtspClient = (RTSPClient*)subsession->miscPtr;
    UsageEnvironment& env = rtspClient->envir(); // alias

    env /*<< *rtspClient*/ << "Received RTCP \"BYE\" on \"" /*<< *subsession*/ << "\" subsession\n";

    // Now act as if the subsession had closed:
    subsessionAfterPlaying(subsession);
}


void CRTSPSessionPrivate::continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString)
{
    do
    {
        UsageEnvironment& env = rtspClient->envir(); // alias
        StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

        if (resultCode != 0)
        {
            env /*<< *rtspClient*/ << "Failed to start playing session: " << resultString << "\n";
            break;
        }

        // Set a timer to be handled at the end of the stream's expected duration (if the stream does not already signal its end
        // using a RTCP "BYE").  This is optional.  If, instead, you want to keep the stream active - e.g., so you can later
        // 'seek' back within it and do another RTSP "PLAY" - then you can omit this code.
        // (Alternatively, if you don't want to receive the entire stream, you could set this timer for some shorter value.)
       if (scs.duration > 0)
       {
             unsigned const delaySlop = 2; // number of seconds extra to delay, after the stream's expected duration.  (This is optional.)
             scs.duration += delaySlop;
             unsigned uSecsToDelay = (unsigned)(scs.duration*1000000);
             scs.streamTimerTask = env.taskScheduler().scheduleDelayedTask(uSecsToDelay, (TaskFunc*)scheduleLivenessCommand, rtspClient);
		}

        env /*<< *rtspClient*/ << "Started playing session";
        if (scs.duration > 0)
        {
            env << " (for up to " << scs.duration << " seconds)";
        }
        env << "...\n";

        ((ourRTSPClient*)rtspClient)->session->pSessionPrivate->m_nResult = 0;
        SetEvent(((ourRTSPClient*)rtspClient)->session->pSessionPrivate->m_hEvent);
        return;
    }
    while (0);

    if (((ourRTSPClient*)rtspClient)->session->pSessionPrivate->m_bWait)
    {
        ((ourRTSPClient*)rtspClient)->session->pSessionPrivate->m_nResult = -4;
        SetEvent(((ourRTSPClient*)rtspClient)->session->pSessionPrivate->m_hEvent);
        // An unrecoverable error occurred with this stream.
        //shutdownStream(rtspClient);
    }
    else
    {
        ((ourRTSPClient*)rtspClient)->session->onDisconnect(resultCode,__FUNCTION__);
    }

}

void CRTSPSessionPrivate::continueAfterGetParameter(RTSPClient* rtspClient, int resultCode, char* resultString)
{
    CRTSPSession* session = ((ourRTSPClient*)rtspClient)->session;
    // ĳЩ�����֧��GetParameter������405��Ϊ��֧�ָù���
    if (resultCode != 0 && resultCode != 405)
    {
        if (session->pSessionPrivate->m_bWait)//�еȴ�
        {
            session->pSessionPrivate->m_nResult = -4;
            SetEvent(session->pSessionPrivate->m_hEvent);
        }
        else
        {
            if (session->pSessionPrivate->_rtspStream == NULL && session->pSessionPrivate->m_bHeartbeat == true)
            {
                session->pSessionPrivate->m_bHeartbeat = false;
            }
            session->onDisconnect(resultCode, __FUNCTION__);
        }
    }
    else
    {
        if (session->pSessionPrivate->m_bWait)//�еȴ�
        {
            session->pSessionPrivate->m_nResult = 0;
            SetEvent(session->pSessionPrivate->m_hEvent);
            session->pSessionPrivate->m_bWait = false;
        }
        if (session->pSessionPrivate->_rtspStream == NULL && session->pSessionPrivate->m_bHeartbeat == false)
        {
            session->pSessionPrivate->m_bHeartbeat = true;//��һ��ʱ�����m_bHeartbeat��false�����Բ�����setup֮ǰ����getparameter����
        }
    }
    return;
}


void CRTSPSessionPrivate::scheduleLivenessCommand(void* clientData)
{
	ourRTSPClient* rtspClient = (ourRTSPClient*)clientData;
	
	// Delay a random time before sending another 'liveness' command.
	unsigned delayMax = 30; // if the server specified a maximum time between 'liveness' probes, then use that

	// Choose a random time from [delayMax/2,delayMax-1) seconds:
	unsigned const us_1stPart = delayMax * 500000;
	unsigned uSecondsToDelay;
	if (us_1stPart <= 1000000)
	{
		uSecondsToDelay = us_1stPart;
	}
	else
	{
		unsigned const us_2ndPart = us_1stPart - 1000000;
		uSecondsToDelay = us_1stPart + (us_2ndPart*our_random()) % us_2ndPart;
	}
	rtspClient->fLivenessCommandTask = rtspClient->envir().taskScheduler().scheduleDelayedTask(uSecondsToDelay, sendLivenessCommand, rtspClient);

}

void CRTSPSessionPrivate::sendLivenessCommand(void* clientData)
{
	RTSPClient* rtspClient = (RTSPClient*)clientData;
	UsageEnvironment& env = rtspClient->envir(); // alias
	StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias
	env << __FUNCTION__ << "\r\n";
	// Note.  By default, we do not send "GET_PARAMETER" as our 'liveness notification' command, even if the server previously
	// indicated (in its response to our earlier "OPTIONS" command) that it supported "GET_PARAMETER".  This is because
	// "GET_PARAMETER" crashes some camera servers (even though they claimed to support "GET_PARAMETER").
//#ifdef SEND_GET_PARAMETER_IF_SUPPORTED
	MediaSession* sess = scs.session;
	if (sess != NULL)
	{
		rtspClient->sendGetParameterCommand(*sess, continueAfterGET_PARAMETER, "", rtspClient->auth());
	}
	else
	{
//#endif
		rtspClient->sendOptionsCommand(continueAfterOPTIONS, rtspClient->auth());
//#ifdef SEND_GET_PARAMETER_IF_SUPPORTED
	}
//#endif
}

void CRTSPSessionPrivate::continueAfterGET_PARAMETER(RTSPClient* rtspClient, int resultCode, char* resultString)
{
	UsageEnvironment& env = rtspClient->envir(); // alias
	continueAfterLivenessCommand(rtspClient, resultCode, True);
	//delete[] resultString;
}


void CRTSPSessionPrivate::continueAfterLivenessCommand(RTSPClient* rtspClient, int resultCode, Boolean serverSupportsGetParameter)
{
	if (resultCode != 0)
	{
		return;
	}

	// Schedule the next 'liveness' command (i.e., to tell the back-end server that we're still alive):
	scheduleLivenessCommand(rtspClient);
}

void CRTSPSessionPrivate::continueAfterOPTIONS(RTSPClient* rtspClient, int resultCode, char* resultString)
{
	Boolean serverSupportsGetParameter = False;
	if (resultCode == 0)
	{
		// Note whether the server told us that it supports the "GET_PARAMETER" command:
		serverSupportsGetParameter = RTSPOptionIsSupported("GET_PARAMETER", resultString);
	}
	continueAfterLivenessCommand(rtspClient, resultCode, serverSupportsGetParameter);
	//delete[] resultString;
}

void CRTSPSessionPrivate::shutdownStream(RTSPClient* rtspClient, int exitCode)
{
    UsageEnvironment& env = rtspClient->envir(); // alias
    StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

    // First, check whether any subsessions have still to be closed:
    if (scs.session != NULL)
    {
        Boolean someSubsessionsWereActive = False;
        MediaSubsessionIterator iter(*scs.session);
        MediaSubsession* subsession;

        while ((subsession = iter.next()) != NULL)
        {
            if (subsession->sink != NULL)
            {
                Medium::close(subsession->sink);
                subsession->sink = NULL;

                if (subsession->rtcpInstance() != NULL)
                {
                    subsession->rtcpInstance()->setByeHandler(NULL, NULL); // in case the server sends a RTCP "BYE" while handling "TEARDOWN"
                }

                someSubsessionsWereActive = True;
            }
        }

        if (someSubsessionsWereActive)
        {
            // Send a RTSP "TEARDOWN" command, to tell the server to shutdown the stream.
            // Don't bother handling the response to the "TEARDOWN".
            rtspClient->sendTeardownCommand(*scs.session, NULL);
        }
    }

    env /*<< *rtspClient*/ << "Closing the stream.\n";

    Medium::close(rtspClient);
    // Note that this will also cause this stream's "StreamClientState" structure to get reclaimed.

    //if (--rtspClientCount == 0) {
    //	// The final stream has ended, so exit the application now.
    //	// (Of course, if you're embedding this code into your own application, you might want to comment this out,
    //	// and replace it with "eventLoopWatchVariable = 1;", so that we leave the LIVE555 event loop, and continue running "main()".)
    //	exit(exitCode);
    //}
}

