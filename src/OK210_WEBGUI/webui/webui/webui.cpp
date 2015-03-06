// webui.cpp : CwebuiApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "webui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CwebuiApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x7E101825, 0x1E92, 0x4159, { 0x9E, 0x2A, 0xAF, 0xF0, 0x52, 0xBE, 0x55, 0x7E } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CwebuiApp::InitInstance - DLL 初始化

BOOL CwebuiApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。
	}

	return bInit;
}



// CwebuiApp::ExitInstance - DLL 终止

int CwebuiApp::ExitInstance()
{
	// TODO: 在此添加您自己的模块终止代码。

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
