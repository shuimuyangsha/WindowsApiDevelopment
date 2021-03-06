/* ************************************
*《精通Windows API》
* 示例代码
* xioctl.h
* 16.2  IO控制、内核通信
**************************************/

// 内核驱动和用户态控制程序都需要使用到的常量定义
#define XIOCTL_TYPE 40000

#define IOCTL_XIOCTL_BUFFER \
    CTL_CODE( XIOCTL_TYPE, 0x902, METHOD_BUFFERED, FILE_ANY_ACCESS  )

#define DRIVER_NAME       "xIoctl"
