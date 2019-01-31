#pragma once

#ifndef _INC_BYTE_ORDER_
#define _INC_BYTE_ORDER_

#ifndef _byteswap_ulong
ULONG _byteswap_ulong(ULONG val);
#endif

#ifndef _byteswap_ushort
USHORT _byteswap_ushort(USHORT val);
#endif

int is_little_endidan();

#endif
