/*
 * SCServo.h
 * 硬件通信接口
 * 日期: 2016.8.25
 * 作者: 谭雄乐
 */

#ifndef _SCSERVO_H
#define _SCSERVO_H

#include "SCSProtocol.h"

#include <QSerialPort>

class SCServo : public SCSProtocol
{
public:
	SCServo(void);
        virtual int writeSCS(unsigned char *nDat, int nLen);
        virtual int readSCS(unsigned char *nDat, int nLen);
        virtual int writeSCS(unsigned char bDat);
        virtual void flushSCS();
public:
        unsigned long int IOTimeOut;
        QSerialPort pSerial;
};

#endif
