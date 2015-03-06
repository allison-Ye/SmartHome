

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __webuiidl_h__
#define __webuiidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___Dwebui_FWD_DEFINED__
#define ___Dwebui_FWD_DEFINED__
typedef interface _Dwebui _Dwebui;
#endif 	/* ___Dwebui_FWD_DEFINED__ */


#ifndef ___DwebuiEvents_FWD_DEFINED__
#define ___DwebuiEvents_FWD_DEFINED__
typedef interface _DwebuiEvents _DwebuiEvents;
#endif 	/* ___DwebuiEvents_FWD_DEFINED__ */


#ifndef __webui_FWD_DEFINED__
#define __webui_FWD_DEFINED__

#ifdef __cplusplus
typedef class webui webui;
#else
typedef struct webui webui;
#endif /* __cplusplus */

#endif 	/* __webui_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __webuiLib_LIBRARY_DEFINED__
#define __webuiLib_LIBRARY_DEFINED__

/* library webuiLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_webuiLib;

#ifndef ___Dwebui_DISPINTERFACE_DEFINED__
#define ___Dwebui_DISPINTERFACE_DEFINED__

/* dispinterface _Dwebui */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__Dwebui;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("50F9499A-A5CB-4174-8E25-FE2F1E0AFBDC")
    _Dwebui : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DwebuiVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _Dwebui * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _Dwebui * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _Dwebui * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _Dwebui * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _Dwebui * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _Dwebui * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _Dwebui * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DwebuiVtbl;

    interface _Dwebui
    {
        CONST_VTBL struct _DwebuiVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _Dwebui_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _Dwebui_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _Dwebui_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _Dwebui_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _Dwebui_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _Dwebui_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _Dwebui_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___Dwebui_DISPINTERFACE_DEFINED__ */


#ifndef ___DwebuiEvents_DISPINTERFACE_DEFINED__
#define ___DwebuiEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DwebuiEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DwebuiEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("AA1FBA33-BFF6-4458-A485-FDD340981A8C")
    _DwebuiEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DwebuiEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DwebuiEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DwebuiEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DwebuiEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DwebuiEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DwebuiEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DwebuiEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DwebuiEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DwebuiEventsVtbl;

    interface _DwebuiEvents
    {
        CONST_VTBL struct _DwebuiEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DwebuiEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DwebuiEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DwebuiEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DwebuiEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DwebuiEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DwebuiEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DwebuiEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DwebuiEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_webui;

#ifdef __cplusplus

class DECLSPEC_UUID("55608E4B-7384-4BFB-8A0D-9C3B4C7774F8")
webui;
#endif
#endif /* __webuiLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


