/* ************************************
*����ͨWindows API��
* ʾ������
* xioctl.h
* 16.2  IO���ơ��ں�ͨ��
**************************************/

// �ں��������û�̬���Ƴ�����Ҫʹ�õ��ĳ�������
#define XIOCTL_TYPE 40000

#define IOCTL_XIOCTL_BUFFER \
    CTL_CODE( XIOCTL_TYPE, 0x902, METHOD_BUFFERED, FILE_ANY_ACCESS  )

#define DRIVER_NAME       "xIoctl"
