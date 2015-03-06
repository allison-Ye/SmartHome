/*
 * Filename:  H264CameraLiveSource.cpp
 * Auther:  cloudsail
 * Create date: 2014/ 2/25
 */

#include "H264CameraLiveSource.hh"
#include "mem_mng.h"
int getFrame(void * addr);
H264CameraLiveSource::H264CameraLiveSource( UsageEnvironment& env,  
    char const* fileName, 
    unsigned preferredFrameSize, 
    unsigned playTimePerFrame )
    : FramedSource(env)
{
     fp = fopen( fileName, "wb" ); 
}

H264CameraLiveSource* H264CameraLiveSource::createNew( UsageEnvironment& env,
                                           char const* fileName, 
                                           unsigned preferredFrameSize /*= 0*/, 
                                           unsigned playTimePerFrame /*= 0*/ )
{ 
    H264CameraLiveSource* newSource = new H264CameraLiveSource(env, fileName, preferredFrameSize, playTimePerFrame);
 
    return newSource;
}

H264CameraLiveSource::~H264CameraLiveSource()
{
    fclose(fp);
}


long filesize(FILE *stream)
{
    long curpos, length;
    curpos = ftell(stream);
    fseek(stream, 0L, SEEK_END);
    length = ftell(stream);
    fseek(stream, curpos, SEEK_SET);
    return length;
}

void H264CameraLiveSource::doGetNextFrame()
{
#if 0
    if( filesize(fp) >  fMaxSize)
      fFrameSize = fread(fTo,1,fMaxSize,fp); 
    else
    {
        fFrameSize = fread(fTo,1,filesize(fp),fp);
        fseek(fp, 0, SEEK_SET);
    }
    printf("fFrameSize = %d\n", fFrameSize);      
#else
    msg_send_to_encoder();
    msg_rcv_from_encoder();
    mem_read(&fFrameSize, 0, sizeof(fFrameSize));
    if( fFrameSize >  fMaxSize)
    {
            fNumTruncatedBytes = fFrameSize - fMaxSize;  
            fFrameSize = fMaxSize;
    //        printf("fMaxSize = %d\n",fMaxSize);
    }  
    else
    {  
        fNumTruncatedBytes = 0;  
    }
    mem_read(fTo, sizeof(fFrameSize), fFrameSize);
#endif
    nextTask() = envir().taskScheduler().scheduleDelayedTask( 0,
        (TaskFunc*)FramedSource::afterGetting, this);//表示延迟0秒后再执行 afterGetting 函数
    return;
}
