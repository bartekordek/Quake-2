#ifndef ____QCOMMON_CRC_H__
#define ____QCOMMON_CRC_H__

/* crc.h */

void CRC_Init(unsigned short *crcvalue);
void CRC_ProcessByte(unsigned short *crcvalue, byte data);
unsigned short CRC_Value(unsigned short crcvalue);
unsigned short CRC_Block (byte *start, int count);

#endif // ____QCOMMON_CRC_H__