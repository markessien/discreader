/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Jan 20 00:02:37 2004
 */
/* Compiler settings for X:\Dev\x86Win32\CDReaderControl\CDReader.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


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

const IID LIBID_CDREADERLib = {0xDB9E17F3,0x5D32,0x458D,{0x94,0x54,0xA8,0xA2,0x19,0xC3,0x4D,0x3C}};


const IID IID_ICDReaderObj = {0x101D6863,0x4A50,0x48AE,{0x97,0x56,0x95,0xCD,0x6B,0x55,0x95,0xE8}};


const IID DIID__ICDReaderObjEvents = {0x9F8045F0,0x07ED,0x4DD4,{0x92,0x18,0x04,0xE6,0x6A,0x71,0xB7,0xD0}};


const CLSID CLSID_CDReader = {0x4A790E89,0x6210,0x40AB,{0x96,0x35,0x07,0xEE,0xA6,0x36,0x2F,0xEB}};


#ifdef __cplusplus
}
#endif

