#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <linux/fb.h>

#include "captureThread.h"
#include "s3cfb.h"
#include "queue.h"
#include "common.h"
#define V4L2_PIX_FMT_NV12T    v4l2_fourcc('T', 'V', '1', '2')
static int fd = -1;
struct buffer {
    void *                  start;
    size_t                  length;
};


static struct buffer * buffers = NULL;
static unsigned int n_buffers = 0;

static char* deviceName = "/dev/video0";

int fb_init(void)
{
    int dev_fp = -1;
    struct s3cfb_user_chroma chroma;

    dev_fp = open("/dev/fb0", O_RDWR);
    if (dev_fp < 0) {
        perror("/dev/fb0");
        return -1;
    }
    
    chroma.enabled = 1;
    chroma.red = 0;
    chroma.green = 255;
    chroma.blue =0; 
    if(ioctl(dev_fp, S3CFB_WIN_SET_CHROMA, &chroma))
    {
        printf("S3CFB_WIN_SET_CHROMA error");
        exit(1);
    }
    return dev_fp;
}

void capture_open(void)
{
    fd = open(deviceName, O_RDWR /* required */ | O_NONBLOCK, 0);
    // check if opening was successfull
    if (-1 == fd) {
        fprintf(stderr, "Cannot open '%s': %d, %s\n", deviceName, errno, strerror (errno));
        exit(EXIT_FAILURE);
    }
}
static void mmapInit(GlobalHandle * handle);
/**
  initialize device
*/
int capture_init(GlobalHandle * handle)
{
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;
    int height = handle->height;
    int width = handle->width;
    unsigned int min;


    if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
        if (EINVAL == errno) {
            fprintf(stderr, "%s is no V4L2 device\n",deviceName);
            exit(EXIT_FAILURE);
        } else {
            errno_exit("VIDIOC_QUERYCAP");
        }
    }

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    {
        fprintf(stderr, "%s is no video capture device\n",deviceName);
        exit(EXIT_FAILURE);
    }

    struct v4l2_input input;
    input.index = 0;
    if ( ioctl(fd, VIDIOC_ENUMINPUT, &input) != 0)
    {
        printf(" set input error.\n");
        exit(0);
    }

    if ((ioctl(fd, VIDIOC_S_INPUT, &input)) < 0)
    {
        printf("set s_input error.\n");
        exit(0);
    }
    /* Select video input, video standard and tune here. */
    CLEAR(cropcap);

    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap))
    {
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect; /* reset to default */

        if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop))
        {
            switch (errno)
            {
            case EINVAL:
                /* Cropping not supported. */
                break;
            default:
                /* Errors ignored. */
                break;
            }
            return 0;
        }
    } 

    CLEAR (fmt);

    // v4l2_format
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = width;
    fmt.fmt.pix.height      = height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_NV12T;

    if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
        errno_exit("VIDIOC_S_FMT");

    /* Note VIDIOC_S_FMT may change width and height. */
    if (width != fmt.fmt.pix.width) {
        width = fmt.fmt.pix.width;
        fprintf(stderr,"Image width set to %i by device %s.\n",width,deviceName);
    }
    if (height != fmt.fmt.pix.height) {
        height = fmt.fmt.pix.height;
        fprintf(stderr,"Image height set to %i by device %s.\n",height,deviceName);
    }

    /* Buggy driver paranoia. */
    min = fmt.fmt.pix.width * 2;
    if (fmt.fmt.pix.bytesperline < min)
        fmt.fmt.pix.bytesperline = min;

    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
    if (fmt.fmt.pix.sizeimage < min)
        fmt.fmt.pix.sizeimage = min;


    mmapInit(handle);
}

static void mmapInit(GlobalHandle * handle)
{
    struct v4l2_requestbuffers req;

    CLEAR (req);

    req.count               = 4;
    req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory              = V4L2_MEMORY_MMAP;

    if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            fprintf(stderr, "%s does not support memory mapping\n", deviceName);
            exit(EXIT_FAILURE);
        } else {
            errno_exit("VIDIOC_REQBUFS");
        }
    }

    if (req.count < 2) {
        fprintf(stderr, "Insufficient buffer memory on %s\n", deviceName);
        exit(EXIT_FAILURE);
    }

    buffers = (struct buffer*)calloc(req.count, sizeof(*buffers));

    if (!buffers) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }

    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        struct v4l2_buffer buf;

        CLEAR (buf);

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = n_buffers;

        if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
            errno_exit("VIDIOC_QUERYBUF");

        buffers[n_buffers].length = buf.length;
        handle->length = buf.length;
        buffers[n_buffers].start = mmap (NULL /* start anywhere */, buf.length, PROT_READ | PROT_WRITE /* required */, MAP_SHARED /* recommended */, fd, buf.m.offset);

        if (MAP_FAILED == buffers[n_buffers].start)
            errno_exit("mmap");
    }
}

void captureStart(void)
{
    unsigned int i;
    enum v4l2_buf_type type;

    for (i = 0; i < n_buffers; ++i)
    {
        struct v4l2_buffer buf;

        CLEAR (buf);
        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = i;

        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
            errno_exit("VIDIOC_QBUF");
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
        errno_exit("VIDIOC_STREAMON");
}

static unsigned int frame_count=1;
static FILE * fp;
void *capture_thread(void *args)
{
    struct timeval tv1,tv2;
    GlobalHandle * handle = (GlobalHandle * )args;
    gettimeofday(&tv1, NULL);
    printf("%s!\n",__func__);
    //fp = fopen("ov9650.yuvT","w+");
    while (1)
    {
        for (;;)
        {
            fd_set fds;
            struct timeval tv;
            int r;

            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            /* Timeout. */
            tv.tv_sec = 20;
            tv.tv_usec = 0;

            r = select(fd + 1, &fds, NULL, NULL, &tv);

            if (-1 == r)
            {
                if (EINTR == errno)
                    continue;
                errno_exit("select");
            }

            if (0 == r)
            {
                fprintf (stderr, "select timeout\n");
                exit(EXIT_FAILURE);
            }
	    if((frame_count % 1000) == 0)
            {
                gettimeofday(&tv2,NULL);
                
                printf("capture:%d frames,%d fps\n",
                    frame_count, 1000 / (tv2.tv_sec - tv1.tv_sec));
                gettimeofday(&tv1,NULL);
            }
            if (frameRead(handle))
                break;
            frame_count++;
        }
    }
}

int write_frame(FILE *fp, void *addr,unsigned long size)
{
    if(fp == NULL)
		return -1;
    if(addr == NULL)
		return -1;
	if(size < 0)
		return -1;
	fwrite(addr, size,1, fp);
}

unsigned char* frameRead(GlobalHandle * handle)
{

    struct v4l2_buffer buf;
    unsigned char * addr;
    CLEAR (buf);

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    
    if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
        switch (errno) {
        case EAGAIN:
            return 0;

        case EIO:
        default:
            errno_exit("VIDIOC_DQBUF");
        }
    }
    assert (buf.index < n_buffers);
#ifndef QUEUE_MODE
    ca_mem_write(buffers[buf.index].start, 0, buffers[buf.index].length);
#else
    put_queue(&handle->capture_fullque, (unsigned int)buffers[buf.index].start);
#endif
    if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
        errno_exit("VIDIOC_QBUF");

    // return (unsigned char*)n;
}

unsigned char* imageProcess(const void* p,unsigned char* dst)
{
    unsigned char* src = (unsigned char*)p;

    memcpy(dst,src,800*480*4);


    return (unsigned char*)dst;


}

void deviceUninit(void)
{
    unsigned int i;

    for (i = 0; i < n_buffers; ++i)
    {
        if (-1 == munmap (buffers[i].start, buffers[i].length))
            errno_exit("munmap");
    }

    free(buffers);
    close(fd);
}

void errno_exit(const char* s)
{
    fprintf(stderr, "%s error %d, %s\n", s, errno, strerror (errno));
    exit(EXIT_FAILURE);
}
int xioctl(int fd, int request, void* argp)
{
    int r;
    do r = ioctl(fd, request, argp);
    while (-1 == r && EINTR == errno);

    return r;
}




