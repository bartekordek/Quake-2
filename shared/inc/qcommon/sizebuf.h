#ifndef __shared_sizebuf_h__
#define __shared_sizebuf_h__

#include "shared/config.h"
#include "shared/boolean.h"
#include "shared/defines.h"
#include "math/vector.h"

typedef struct sizebuf_s
{
	qboolean allowoverflow;	 // if e_false, do a Com_Error
	qboolean overflowed;	 // set to e_true if the buffer size failed
	byte	*data;
	int		 maxsize;
	int		 cursize;
	int		 readcount;
} sizebuf_t;

EXTERNC void  SZ_Init(sizebuf_t *buf, byte *data, int length);
EXTERNC void  SZ_Clear(sizebuf_t *buf);
EXTERNC void *SZ_GetSpace(sizebuf_t *buf, int length);
EXTERNC void  SZ_Write(sizebuf_t *buf, const void *data, int length);
EXTERNC void  SZ_Print(sizebuf_t *buf, char *data);	 // strcats onto the sizebuf
EXTERNC void  MSG_WriteChar(sizebuf_t *sb, int c);
EXTERNC void  MSG_WriteByte(sizebuf_t *sb, int c);
EXTERNC void  MSG_WriteShort(sizebuf_t *sb, int c);
EXTERNC void  MSG_WriteLong(sizebuf_t *sb, int c);
EXTERNC void  MSG_WriteFloat(sizebuf_t *sb, float f);
EXTERNC void  MSG_WriteString(sizebuf_t *sb, char *s);
EXTERNC void  MSG_WriteCoord(sizebuf_t *sb, float f);
EXTERNC void  MSG_WritePos(sizebuf_t *sb, vec3_t pos);
EXTERNC void  MSG_WriteAngle(sizebuf_t *sb, float f);
EXTERNC void  MSG_WriteAngle16(sizebuf_t *sb, float f);
EXTERNC void  MSG_WriteDeltaUsercmd(sizebuf_t *sb, struct usercmd_s *from, struct usercmd_s *cmd);
EXTERNC void  MSG_WriteDeltaEntity(struct entity_state_s *from, struct entity_state_s *to, sizebuf_t *msg, qboolean force,
								   qboolean newentity);
EXTERNC void  MSG_WriteDir(sizebuf_t *sb, vec3_t vector);

EXTERNC void MSG_BeginReading(sizebuf_t *sb);

EXTERNC int	  MSG_ReadChar(sizebuf_t *sb);
EXTERNC int	  MSG_ReadByte(sizebuf_t *sb);
EXTERNC int	  MSG_ReadShort(sizebuf_t *sb);
EXTERNC int	  MSG_ReadLong(sizebuf_t *sb);
EXTERNC float MSG_ReadFloat(sizebuf_t *sb);
EXTERNC char *MSG_ReadString(sizebuf_t *sb);
EXTERNC char *MSG_ReadStringLine(sizebuf_t *sb);

EXTERNC float MSG_ReadCoord(sizebuf_t *sb);
EXTERNC void  MSG_ReadPos(sizebuf_t *sb, vec3_t pos);
EXTERNC float MSG_ReadAngle(sizebuf_t *sb);
EXTERNC float MSG_ReadAngle16(sizebuf_t *sb);
EXTERNC void  MSG_ReadDeltaUsercmd(sizebuf_t *sb, struct usercmd_s *from, struct usercmd_s *cmd);

EXTERNC void MSG_ReadDir(sizebuf_t *sb, vec3_t vector);

EXTERNC void MSG_ReadData(sizebuf_t *sb, void *buffer, int size);

#endif	// __shared_sizebuf_h__