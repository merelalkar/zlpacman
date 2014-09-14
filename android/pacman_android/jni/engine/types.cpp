#include "engine.h"
#include "types.h"

using namespace Pegas;

const int32	    Pegas::MIN_INT32   =   (int32)0xffffffff; // (-2147483646);
const int32	    Pegas::MAX_INT32   =   (int32)0x7fffffff; // ( 2147483647);
const int16	    Pegas::MIN_INT16   =   (int16)0xffff; // (-32768);
const int16	    Pegas::MAX_INT16   =   (int16)0x7fff; // ( 32767);
const int8	    Pegas::MIN_INT8    =   (int8)0xff; // (-128);
const int8	    Pegas::MAX_INT8    =   (int8)0x7f; // ( 127);
const uint32    Pegas::MIN_UINT32  =   (uint32)0;
const uint32    Pegas::MAX_UINT32  =   (uint32)0xffffffff;
const uint16    Pegas::MIN_UINT16  =   (uint16)0;
const uint16    Pegas::MAX_UINT16  =   (uint16)0xffff;
const uint8	    Pegas::MIN_UINT8   =   (uint8)0;
const uint8	    Pegas::MAX_UINT8   =   (uint8)0xff;

const float	Pegas::MAX_REAL32  =   static_cast<float>(3.4E+38);
const float	Pegas::MIN_REAL32  =   static_cast<float>(-3.4E+38);
const float	Pegas::TINY_REAL32  =   static_cast<float>(3.4E-38);