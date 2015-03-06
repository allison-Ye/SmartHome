/*
 * Filename: H26CameraLiveSource.hh
 * Auther: cloudsail
 * Create date: 2014/ 2/25
 */

#ifndef _H264CAMERALIVESOURCE_HH
#define _H264CAMERALIVESOURCE_HH

#include <FramedSource.hh>


class H264CameraLiveSource : public FramedSource
{
public:
    static H264CameraLiveSource* createNew(UsageEnvironment& env,
        char const* fileName,
        unsigned preferredFrameSize = 0,
        unsigned playTimePerFrame = 0); 

protected:
    H264CameraLiveSource(UsageEnvironment& env,
        char const* fileName, 
        unsigned preferredFrameSize,
        unsigned playTimePerFrame);
    // called only by createNew()
    ~H264CameraLiveSource();

private:
    // redefined virtual functions:
    virtual void doGetNextFrame();
    int TransportData( unsigned char* to, unsigned maxSize );

protected:
    FILE *fp;
};

#endif
