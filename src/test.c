#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <linux/videodev2.h>


#define CLEAR(x) memset(&(x), 0, sizeof(x))


# fswebcam -d RAW:test.raw  --jpeg 100 -r640x480 -S 0 -f 0 -p YUYV test.jpeg

static int xioctl(int fd, int request, void* argp)
{
    int r;
    
    do r = ioctl(fd, request, argp);
    while (-1 == r && EINTR == errno);
    
    return r;
}

static void errno_exit(const char* s)
{
//     fprintf(stderr, "%s error %d, %s\n", s, errno, strerror (errno));
    exit(EXIT_FAILURE);
}

struct  v4l_buff {
    void * start;
    size_t length;
};


int main(int argc, char const *argv[])
{
    unsigned char *f;
    struct v4l_buff *         buffers         = NULL;
    unsigned long size, i, n_buffers;
    struct v4l2_buffer buf;
    struct stat s;
    const char * file_name = argv[1];
    int fd = open (argv[1], O_RDWR | O_NONBLOCK, 0);
    
    /* Get the size of the file. */
    int status = fstat (fd, & s);
    size = s.st_size;

    
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    
    
    if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
        if (EINVAL == errno) {
//             fprintf(stderr, "%s is no V4L2 device\n",argv[1]);
            exit(EXIT_FAILURE);
        } else {
            errno_exit("VIDIOC_QUERYCAP");
        }
    }
    
    
    /* Select video input, video standard and tune here. */
    CLEAR(cropcap);
    
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    
    if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect; /* reset to default */
        
        if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) {
            switch (errno) {
                case EINVAL:
                    /* Cropping not supported. */
                    break;
                default:
                    /* Errors ignored. */
                    break;
            }
        }
    } else {        
        /* Errors ignored. */
    }
    
    // v4l2_format
    struct v4l2_format fmt;
    CLEAR(fmt);
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = 640; 
    fmt.fmt.pix.height      = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
        errno_exit("VIDIOC_S_FMT");
    
    
    struct v4l2_requestbuffers req;
    
    CLEAR (req);
    
    req.count               = 4;
    req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory              = V4L2_MEMORY_MMAP;
    
    
    xioctl(fd, VIDIOC_REQBUFS, &req);
//     printf("request: %i   ",xioctl(fd, VIDIOC_REQBUFS, &req));
    
//     buffers = calloc(req.count, sizeof(*buffers));
    

    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        
        CLEAR (buf);
        
        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = 3;

        
        xioctl(fd, VIDIOC_QUERYBUF, &buf);
//         printf("init:  %i ", xioctl(fd, VIDIOC_QUERYBUF, &buf));


        size = buf.length;
        
        buffers = calloc(req.count, sizeof(*buffers));
        

            struct v4l2_buffer buf;
            
            CLEAR (buf);
            
        buffers[n_buffers].start = mmap (NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
        
    }
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    xioctl(fd, VIDIOC_STREAMON, &type);
//     printf("after: %i ", xioctl(fd, VIDIOC_STREAMON, &type));
    
    for (i = 0; i < n_buffers; ++i) {
        struct v4l2_buffer buf;
        
        CLEAR (buf);
        
        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = i;
        
        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
            errno_exit("VIDIOC_QBUF");
    }
    

    
    
for (;;){
    
    
    fd_set fds;
    struct timeval tv;
    int r;
    
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    
    /* Timeout. */
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    
    r = select(fd + 1, &fds, NULL, NULL, &tv);


    CLEAR (buf);
   
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    
    if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
        switch (errno) {
            case EAGAIN:
                continue;
                
            case EIO:
                // Could ignore EIO, see spec
                
                // fall through
            default:
                errno_exit("VIDIOC_DQBUF");
        }
    }
    
    size = buf.length;
    unsigned char *j;
//      printf("traaa %lu and index: %lu>>", buffers[buf.index].start, buf.index);
    assert (buf.index < n_buffers);
    if (buffers[buf.index].start == 0)
        continue;
    fwrite(buffers[buf.index].start, sizeof(char), 680*480*24, stdout);
    if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
        errno_exit("VIDIOC_QBUF");
    

}
    
    
    



    return 0;
}