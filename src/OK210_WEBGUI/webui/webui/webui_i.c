

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Thu Mar 20 13:35:22 2014
 */
/* Compiler settings for .\webui.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_webuiLib,0x7E101825,0x1E92,0x4159,0x9E,0x2A,0xAF,0xF0,0x52,0xBE,0x55,0x7E);


MIDL_DEFINE_GUID(IID, DIID__Dwebui,0x50F9499A,0xA5CB,0x4174,0x8E,0x25,0xFE,0x2F,0x1E,0x0A,0xFB,0xDC);


MIDL_DEFINE_GUID(IID, DIID__DwebuiEvents,0xAA1FBA33,0xBFF6,0x4458,0xA4,0x85,0xFD,0xD3,0x40,0x98,0x1A,0x8C);


MIDL_DEFINE_GUID(CLSID, CLSID_webui,0x55608E4B,0x7384,0x4BFB,0x8A,0x0D,0x9C,0x3B,0x4C,0x77,0x74,0xF8);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



