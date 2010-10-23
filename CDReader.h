/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Jan 20 00:02:37 2004
 */
/* Compiler settings for X:\Dev\x86Win32\CDReaderControl\CDReader.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __CDReader_h__
#define __CDReader_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ICDReaderObj_FWD_DEFINED__
#define __ICDReaderObj_FWD_DEFINED__
typedef interface ICDReaderObj ICDReaderObj;
#endif 	/* __ICDReaderObj_FWD_DEFINED__ */


#ifndef ___ICDReaderObjEvents_FWD_DEFINED__
#define ___ICDReaderObjEvents_FWD_DEFINED__
typedef interface _ICDReaderObjEvents _ICDReaderObjEvents;
#endif 	/* ___ICDReaderObjEvents_FWD_DEFINED__ */


#ifndef __CDReader_FWD_DEFINED__
#define __CDReader_FWD_DEFINED__

#ifdef __cplusplus
typedef class CDReader CDReader;
#else
typedef struct CDReader CDReader;
#endif /* __cplusplus */

#endif 	/* __CDReader_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __CDREADERLib_LIBRARY_DEFINED__
#define __CDREADERLib_LIBRARY_DEFINED__

/* library CDREADERLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_CDREADERLib;

#ifndef __ICDReaderObj_INTERFACE_DEFINED__
#define __ICDReaderObj_INTERFACE_DEFINED__

/* interface ICDReaderObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICDReaderObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("101D6863-4A50-48AE-9756-95CD6B5595E8")
    ICDReaderObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ListCDDrives( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ListSongsOnCD( 
            /* [in] */ long Index) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCDDBNamesOfCD( 
            /* [in] */ long DriveIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CDDBServerInfo( 
            /* [in] */ BSTR Server,
            /* [in] */ BSTR CGI,
            /* [in] */ VARIANT_BOOL UseProxy,
            /* [in] */ BSTR ProxyAddress,
            /* [in] */ long ProxyPort) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CDDBGetServerList( 
            /* [in] */ VARIANT_BOOL OnlyDefault) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SelectCDDBCDInfoFromMultiple( 
            /* [in] */ long Index) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentlyExtractedTrack( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ExtractionProgress( 
            /* [retval][out] */ double __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExtractTrack( 
            /* [in] */ long CDIndex,
            /* [in] */ long Index,
            /* [in] */ BSTR TargetFilePath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AbortExtraction( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ExtractedFileLength( 
            /* [retval][out] */ double __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICDReaderObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICDReaderObj __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICDReaderObj __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICDReaderObj __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICDReaderObj __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICDReaderObj __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICDReaderObj __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICDReaderObj __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ListCDDrives )( 
            ICDReaderObj __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ListSongsOnCD )( 
            ICDReaderObj __RPC_FAR * This,
            /* [in] */ long Index);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCDDBNamesOfCD )( 
            ICDReaderObj __RPC_FAR * This,
            /* [in] */ long DriveIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CDDBServerInfo )( 
            ICDReaderObj __RPC_FAR * This,
            /* [in] */ BSTR Server,
            /* [in] */ BSTR CGI,
            /* [in] */ VARIANT_BOOL UseProxy,
            /* [in] */ BSTR ProxyAddress,
            /* [in] */ long ProxyPort);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CDDBGetServerList )( 
            ICDReaderObj __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL OnlyDefault);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectCDDBCDInfoFromMultiple )( 
            ICDReaderObj __RPC_FAR * This,
            /* [in] */ long Index);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CurrentlyExtractedTrack )( 
            ICDReaderObj __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ExtractionProgress )( 
            ICDReaderObj __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ExtractTrack )( 
            ICDReaderObj __RPC_FAR * This,
            /* [in] */ long CDIndex,
            /* [in] */ long Index,
            /* [in] */ BSTR TargetFilePath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AbortExtraction )( 
            ICDReaderObj __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ExtractedFileLength )( 
            ICDReaderObj __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *pVal);
        
        END_INTERFACE
    } ICDReaderObjVtbl;

    interface ICDReaderObj
    {
        CONST_VTBL struct ICDReaderObjVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICDReaderObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICDReaderObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICDReaderObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICDReaderObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICDReaderObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICDReaderObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICDReaderObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICDReaderObj_ListCDDrives(This)	\
    (This)->lpVtbl -> ListCDDrives(This)

#define ICDReaderObj_ListSongsOnCD(This,Index)	\
    (This)->lpVtbl -> ListSongsOnCD(This,Index)

#define ICDReaderObj_GetCDDBNamesOfCD(This,DriveIndex)	\
    (This)->lpVtbl -> GetCDDBNamesOfCD(This,DriveIndex)

#define ICDReaderObj_CDDBServerInfo(This,Server,CGI,UseProxy,ProxyAddress,ProxyPort)	\
    (This)->lpVtbl -> CDDBServerInfo(This,Server,CGI,UseProxy,ProxyAddress,ProxyPort)

#define ICDReaderObj_CDDBGetServerList(This,OnlyDefault)	\
    (This)->lpVtbl -> CDDBGetServerList(This,OnlyDefault)

#define ICDReaderObj_SelectCDDBCDInfoFromMultiple(This,Index)	\
    (This)->lpVtbl -> SelectCDDBCDInfoFromMultiple(This,Index)

#define ICDReaderObj_get_CurrentlyExtractedTrack(This,pVal)	\
    (This)->lpVtbl -> get_CurrentlyExtractedTrack(This,pVal)

#define ICDReaderObj_get_ExtractionProgress(This,pVal)	\
    (This)->lpVtbl -> get_ExtractionProgress(This,pVal)

#define ICDReaderObj_ExtractTrack(This,CDIndex,Index,TargetFilePath)	\
    (This)->lpVtbl -> ExtractTrack(This,CDIndex,Index,TargetFilePath)

#define ICDReaderObj_AbortExtraction(This)	\
    (This)->lpVtbl -> AbortExtraction(This)

#define ICDReaderObj_get_ExtractedFileLength(This,pVal)	\
    (This)->lpVtbl -> get_ExtractedFileLength(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICDReaderObj_ListCDDrives_Proxy( 
    ICDReaderObj __RPC_FAR * This);


void __RPC_STUB ICDReaderObj_ListCDDrives_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICDReaderObj_ListSongsOnCD_Proxy( 
    ICDReaderObj __RPC_FAR * This,
    /* [in] */ long Index);


void __RPC_STUB ICDReaderObj_ListSongsOnCD_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICDReaderObj_GetCDDBNamesOfCD_Proxy( 
    ICDReaderObj __RPC_FAR * This,
    /* [in] */ long DriveIndex);


void __RPC_STUB ICDReaderObj_GetCDDBNamesOfCD_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICDReaderObj_CDDBServerInfo_Proxy( 
    ICDReaderObj __RPC_FAR * This,
    /* [in] */ BSTR Server,
    /* [in] */ BSTR CGI,
    /* [in] */ VARIANT_BOOL UseProxy,
    /* [in] */ BSTR ProxyAddress,
    /* [in] */ long ProxyPort);


void __RPC_STUB ICDReaderObj_CDDBServerInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICDReaderObj_CDDBGetServerList_Proxy( 
    ICDReaderObj __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL OnlyDefault);


void __RPC_STUB ICDReaderObj_CDDBGetServerList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICDReaderObj_SelectCDDBCDInfoFromMultiple_Proxy( 
    ICDReaderObj __RPC_FAR * This,
    /* [in] */ long Index);


void __RPC_STUB ICDReaderObj_SelectCDDBCDInfoFromMultiple_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICDReaderObj_get_CurrentlyExtractedTrack_Proxy( 
    ICDReaderObj __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ICDReaderObj_get_CurrentlyExtractedTrack_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICDReaderObj_get_ExtractionProgress_Proxy( 
    ICDReaderObj __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *pVal);


void __RPC_STUB ICDReaderObj_get_ExtractionProgress_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICDReaderObj_ExtractTrack_Proxy( 
    ICDReaderObj __RPC_FAR * This,
    /* [in] */ long CDIndex,
    /* [in] */ long Index,
    /* [in] */ BSTR TargetFilePath);


void __RPC_STUB ICDReaderObj_ExtractTrack_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICDReaderObj_AbortExtraction_Proxy( 
    ICDReaderObj __RPC_FAR * This);


void __RPC_STUB ICDReaderObj_AbortExtraction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICDReaderObj_get_ExtractedFileLength_Proxy( 
    ICDReaderObj __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *pVal);


void __RPC_STUB ICDReaderObj_get_ExtractedFileLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICDReaderObj_INTERFACE_DEFINED__ */


#ifndef ___ICDReaderObjEvents_DISPINTERFACE_DEFINED__
#define ___ICDReaderObjEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ICDReaderObjEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__ICDReaderObjEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("9F8045F0-07ED-4DD4-9218-04E66A71B7D0")
    _ICDReaderObjEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ICDReaderObjEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _ICDReaderObjEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _ICDReaderObjEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _ICDReaderObjEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _ICDReaderObjEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _ICDReaderObjEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _ICDReaderObjEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _ICDReaderObjEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _ICDReaderObjEventsVtbl;

    interface _ICDReaderObjEvents
    {
        CONST_VTBL struct _ICDReaderObjEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ICDReaderObjEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _ICDReaderObjEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _ICDReaderObjEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _ICDReaderObjEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _ICDReaderObjEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _ICDReaderObjEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _ICDReaderObjEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ICDReaderObjEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CDReader;

#ifdef __cplusplus

class DECLSPEC_UUID("4A790E89-6210-40AB-9635-07EEA6362FEB")
CDReader;
#endif
#endif /* __CDREADERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
