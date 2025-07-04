#ifndef ALCHEMY_STDINT_H
#define ALCHEMY_STDINT_H





#include "Defines.h"





typedef unsigned char  auint8_t;
typedef unsigned short auint16_t;
typedef unsigned int   auint32_t;
typedef unsigned long  auint64_t;

typedef char           aint8_t;
typedef short          aint16_t;
typedef int            aint32_t;
typedef long           aint64_t;

// We don't support 64 bit platforms yet
typedef auint32_t      asize_t;





#endif // ALCHEMY_STDINT_H
