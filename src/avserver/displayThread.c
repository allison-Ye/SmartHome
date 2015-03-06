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

#include "displayThread.h"
#include "s5p_fimc.h"
#include "mem_mng.h"
#include "common.h"


#define V4L2_CID_PRIV_OFFSET         0x0
#define V4L2_CID_PRIV_ROTATION       (V4L2_CID_PRIVATE_BASE \
        + V4L2_CID_PRIV_OFFSET + 0)
#define V4L2_CID_PRIV_COLORKEY       (V4L2_CID_PRIVATE_BASE \
        + V4L2_CID_PRIV_OFFSET + 1)
#define V4L2_CID_PRIV_COLORKEY_EN    (V4L2_CID_PRIVATE_BASE \
        + V4L2_CID_PRIV_OFFSET + 2)
static int fd = -1;
struct buffer {
    void *                  start;
    size_t                  length;
};

//static io_method        io              = IO_METHOD_MMAP;

static struct buffer *         buffers         = NULL;
static unsigned int     n_buffers       = 0;

// global settings

#define DISPDEV_PATH "/dev/video1"


static int fb_bpp;
static int cam_fp = -1;
static int fb_fp = -1;

extern char *fb_addr;
extern int fb_size;


void display_open(void)
{
    fd = open(DISPDEV_PATH, O_RDWR /* required */ | O_NONBLOCK, 0);
    // check if opening was successfull
    if (-1 == fd) {
        fprintf(stderr, "Cannot open '%s': %d, %s\n", DISPDEV_PATH, errno, strerror (errno));
        exit(EXIT_FAILURE);
    }
}
static void mmapInit(void);

#define V4L2_CID_ROTATION		(V4L2_CID_PRIVATE_BASE + 0)
#define V4L2_CID_RESERVED_MEM_BASE_ADDR	(V4L2_CID_PRIVATE_BASE + 20)
#define V4L2_CID_FIMC_VERSION		(V4L2_CID_PRIVATE_BASE + 21)
/**
  initialize device
*/
int display_init(GlobalHandle * handle)
{
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;
    struct v4l2_framebuffer fbuf;
    struct v4l2_control vc;
    unsigned int width = handle->width;
    unsigned int height = handle->height;
    unsigned int disp_width = handle->disp_width;
    unsigned int disp_height = handle->disp_height;
    int ret;

    unsigned int min;
    fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    if(-1 == xioctl(fd , VIDIOC_G_FMT, &fmt))
    {
        printf("VIDIOC_G_FMT error!\n");
        errno_exit("VIDIOC_G_FMT");
    }
    fmt.fmt.pix.width       = width;
    fmt.fmt.pix.height      = height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_NV12T;

    if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
        errno_exit("VIDIOC_S_FMT");

    if(-1 == xioctl(fd, VIDIOC_G_FBUF, &fbuf))
        errno_exit("VIDIOC_G_FBUF");
    fbuf.fmt.width       = width;
    fbuf.fmt.height      = height;
    fbuf.fmt.pixelformat = V4L2_PIX_FMT_RGB24;
    fbuf.flags &= ~V4L2_FBUF_FLAG_GLOBAL_ALPHA;
    if(-1 == xioctl(fd, VIDIOC_S_FBUF, &fbuf))
        errno_exit("VIDIOC_S_FBUF");

    vc.id = V4L2_CID_FIMC_VERSION;
    ret = xioctl(fd, VIDIOC_G_CTRL, &vc);
    printf("version =0x%x\n",vc.value);
 
    crop.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    if(-1 == xioctl(fd, VIDIOC_G_CROP, &crop))
        errno_exit("VIDIOC_G_CROP");
    crop.c.left = 0;
    crop.c.top = 0;
    crop.c.width = width;
    crop.c.height = height;
    crop.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    if(-1 == xioctl(fd, VIDIOC_S_CROP, &crop))
        errno_exit("VIDIOC_S_CROP");
    
    memset(&fmt, 0 ,sizeof(struct v4l2_format));
    fmt.type = V4L2_BUF_TYPE_VIDEO_OVERLAY;
    fmt.fmt.win.w.left = 50;
    fmt.fmt.win.w.top = 50;
    fmt.fmt.win.w.width = 400;//width;
    fmt.fmt.win.w.height = 300;//height;
    fmt.fmt.win.global_alpha = 0xff;
   
    if(-1 == xioctl(fd, VIDIOC_S_FMT,&fmt))
        errno_exit("VIDIOC_S_FMT"); 

 
    mmapInit();
}

static void mmapInit(void)
{
    struct v4l2_requestbuffers req;

    CLEAR (req);

    req.count               = 4;
    req.type                = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    req.memory              = V4L2_MEMORY_MMAP;

    if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            fprintf(stderr, "%s does not support memory mapping\n", DISPDEV_PATH);
            exit(EXIT_FAILURE);
        } else {
            errno_exit("VIDIOC_REQBUFS");
        }
    }

    if (req.count < 2) {
        fprintf(stderr, "Insufficient buffer memory on %s\n", DISPDEV_PATH);
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

        buf.type        = V4L2_BUF_TYPE_VIDEO_OUTPUT;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = n_buffers;

        if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
            errno_exit("VIDIOC_QUERYBUF");

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start = mmap (NULL /* start anywhere */, buf.length, PROT_READ | PROT_WRITE /* required */, MAP_SHARED /* recommended */, fd, buf.m.offset);

        if (MAP_FAILED == buffers[n_buffers].start)
            errno_exit("mmap");
    }
}

void displayStart(void)
{
    unsigned int i;
    enum v4l2_buf_type type;
    
    type = V4L2_BUF_TYPE_VIDEO_OUTPUT;

    if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
        errno_exit("VIDIOC_STREAMON");
}

static unsigned  frame_count=0;

void *display_thread(void *args)
{
     GlobalHandle * handle = (GlobalHandle *)args;
     struct v4l2_buffer buf;
     unsigned long i = 0;
     struct timeval tv1,tv2;
     gettimeofday(&tv1, NULL);
     while(1)
     {
         unsigned char * addr;
         buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
         buf.memory = V4L2_MEMORY_MMAP;
         buf.index = i%3;
#ifndef QUEUE_MODE
         ca_mem_read(buffers[buf.index].start, 0, buffers[buf.index].length);
#else
         get_queue(&handle->capture_fullque, ((unsigned int *)&addr));
         memcpy(buffers[buf.index].start , addr,buffers[buf.index].length);
         put_queue(&handle->display_fullque, (unsigned int)buffers[buf.index].start);
#endif
         if(-1 == xioctl(fd, VIDIOC_QBUF, &buf))
             errno_exit("VIDIOC_QBUF");
         if(-1 == xioctl(fd, VIDIOC_DQBUF, &buf))
             errno_exit("VIDIOC_DQBUF");
         i++;
         if((i % 1000) == 0)
         {
             gettimeofday(&tv2, NULL);
             printf("displayThread: %d frames, %d fps\n",
                 i, 1000 / (tv2.tv_sec - tv1.tv_sec));
             gettimeofday(&tv1, NULL);
         }
     }


}

