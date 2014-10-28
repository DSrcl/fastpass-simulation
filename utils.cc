#include "utils.h"

uint8_t* u32to8(uint32_t u32)
{
	uint8_t* u8 = new uint8_t[4];
	u8[0] = (u32 & 0xff000000) >> 24;
	u8[1] = (u32 & 0xff0000) >> 16;
	u8[2] = (u32 & 0xff00) >> 8;
	u8[3] = u32 & 0xff;
	return u8;
}


uint32_t u8to32(uint8_t *u8)
{
	uint32_t u32 = 0;
	u32 += u8[0] << 24;
	u32 += u8[1] << 16;
	u32 += u8[2] << 8;
	u32 += u8[3];
	return u32;
}



