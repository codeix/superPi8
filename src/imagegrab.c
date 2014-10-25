
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/videodev2.h>
#include "config.c"

#define CLEAR(x) memset(&(x), 0, sizeof(x))


int fd = -1;
static unsigned int n_buffers = 0;
struct buffer * buffers = NULL;


static int frameRead(void)
{
    struct v4l2_buffer buf;
    unsigned int i;

    CLEAR (buf);
    
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    
    if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
        switch (errno) {
            case EAGAIN:
                return 0;
                
            case EIO:
                // Could ignore EIO, see spec
                
                // fall through
            default:
                errno_exit("VIDIOC_DQBUF");
        }
    }
    
//     assert (buf.index < n_buffers);
    
//     imageProcess(buffers[buf.index].start);
    
//     if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
//         errno_exit("VIDIOC_QBUF");
    return 1;
}


void grab_image(int frames, char* path){
    
    

    struct v4l2_format              fmt;
    struct v4l2_buffer              buf;
    struct v4l2_requestbuffers      req;
    enum v4l2_buf_type              type;
    
    
    fd = v4l2_open(VIDEO_DEV, O_RDWR | O_NONBLOCK, 0);
    if (fd < 0) {
        perror("Cannot open device");
        exit(EXIT_FAILURE);
    }
    
    CLEAR(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = 640;
    fmt.fmt.pix.height      = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
}