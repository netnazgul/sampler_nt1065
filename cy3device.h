#ifndef CY3DEVICE_H
#define CY3DEVICE_H

#include <QtCore>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QVector>
#include <vector>

#ifdef Q_OS_WIN
#include <windows.h>
#include "cyapi\inc\CyAPI.h"
#endif

#ifdef Q_OS_LINUX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include <sys/ioctl.h>  /* ioctl */

#include "../../cyp-linux-driver/ntlcyp.h"
#endif

#define MAX_QUEUE_SZ        64
#define MAX_TRANSFER_LENGTH ( 0x400000 )
#define VENDOR_ID           ( 0x1209 )
#define PRODUCT_STREAM      ( 0xF010 )
#define PRODUCT_BOOT        ( 0x00F3 )

enum cy3device_err_t {
    CY3DEV_OK = 0,
    CY3DEV_ERR_DRV_NOT_IMPLEMENTED       = -5,
    CY3DEV_ERR_USB_INIT_FAIL             = -10,
    CY3DEV_ERR_NO_DEVICE_FOUND           = -11,
    CY3DEV_ERR_BAD_DEVICE                = -12,
    CY3DEV_ERR_FIRMWARE_FILE_IO_ERROR    = -20,
    CY3DEV_ERR_FIRMWARE_FILE_CORRUPTED   = -21,
    CY3DEV_ERR_ADDFIRMWARE_FILE_IO_ERROR = -25,
    CY3DEV_ERR_REG_WRITE_FAIL            = -32,
    CY3DEV_ERR_FW_TOO_MANY_ERRORS        = -33,
    CY3DEV_ERR_CTRL_TX_FAIL              = -35,
    CY3DEV_ERR_BULK_IO_ERROR             = -37
};

const char* cy3device_get_error_string(cy3device_err_t error);

struct EndPointParams{
    int Attr;
    bool In;
    int MaxPktSize;
    int MaxBurst;
    int Interface;
    int Address;
};

struct DeviceParams{
#ifdef Q_OS_WIN
    CCyFX3Device	*USBDevice;
    CCyUSBEndPoint  *EndPt;
#endif
#ifdef Q_OS_LINUX
    int             fd;
#endif
    int				PPX;
    long            TransferSize;
    int				QueueSize;
    int				TimeOut;
    bool			bHighSpeedDevice;
    bool			bSuperSpeedDevice;
    bool			RunStream;
    EndPointParams  CurEndPoint;
};

class cy3device : public QObject
{
    Q_OBJECT

private:
    QString FWName;

    DeviceParams Params;
    std::vector<EndPointParams> Endpoints;

    QTime time;

    int BytesXferred;
    unsigned long Successes;
    unsigned long Failures;
#ifdef Q_OS_WIN
    PUCHAR			*buffers;
    CCyIsoPktInfo	**isoPktInfos;
    PUCHAR			*contexts;
    OVERLAPPED		inOvLap[MAX_QUEUE_SZ];
#endif
    int CurrQueue;

   // QVector<unsigned char> qdata;

    unsigned char prev;
    unsigned long long cc;
    long cc_one;

    bool streamStarted;
    bool errorShown;

    QMutex ccMutex;
    long long ccData;

    cy3device_err_t scan(int &loadable_count , int &streamable_count);
    cy3device_err_t prepareEndPoints();
    void getEndPointParamsByInd(unsigned int EndPointInd, int *Attr, bool *In, int *MaxPktSize, int *MaxBurst, int *Interface, int *Address);

    cy3device_err_t startTransfer(unsigned int EndPointInd, int PPX, int QueueSize, int TimeOut);
    Q_INVOKABLE void transfer();
#ifdef Q_OS_WIN
    void abortTransfer(int pending, PUCHAR *buffers, CCyIsoPktInfo **isoPktInfos, PUCHAR *contexts, OVERLAPPED *inOvLap);
#endif
#ifdef Q_OS_LINUX
    void abortTransfer();
#endif
    Q_INVOKABLE void stopTransfer();

    Q_INVOKABLE void StartStreamQueue();
    Q_INVOKABLE void StopStreamQueue();

    void processData(char* data, int size);
public:
    explicit cy3device(const char* firmwareFileName, QObject *parent = 0);

    bool isStreaming;

    void ccInc(long long bytes);

signals:
    void DebugMessage(QString Message);

    void StopTransfer();

    void ReportBandwidth(int BW);

    void RawData(QVector<unsigned char>* qdata);

public slots:
    cy3device_err_t OpenDevice();
    void CloseDevice();

    cy3device_err_t WriteSPI(unsigned char Address, unsigned char Data);
    cy3device_err_t ReadSPI(unsigned char Address, unsigned char *Data);
    cy3device_err_t startStop(bool start);
    cy3device_err_t reset();

    void StartStream();
    void StopStream();
};

#endif // CY3DEVICE_H
