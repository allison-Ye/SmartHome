// webuiCtrl.cpp : CwebuiCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "webui.h"
#include "webuiCtrl.h"
#include "webuiPropPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CwebuiCtrl, COleControl)



// 消息映射

BEGIN_MESSAGE_MAP(CwebuiCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP(CwebuiCtrl, COleControl)
	DISP_FUNCTION_ID(CwebuiCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP(CwebuiCtrl, COleControl)
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CwebuiCtrl, 1)
	PROPPAGEID(CwebuiPropPage::guid)
END_PROPPAGEIDS(CwebuiCtrl)



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CwebuiCtrl, "WEBUI.webuiCtrl.1",
	0x55608e4b, 0x7384, 0x4bfb, 0x8a, 0xd, 0x9c, 0x3b, 0x4c, 0x77, 0x74, 0xf8)



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CwebuiCtrl, _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID BASED_CODE IID_Dwebui =
		{ 0x50F9499A, 0xA5CB, 0x4174, { 0x8E, 0x25, 0xFE, 0x2F, 0x1E, 0xA, 0xFB, 0xDC } };
const IID BASED_CODE IID_DwebuiEvents =
		{ 0xAA1FBA33, 0xBFF6, 0x4458, { 0xA4, 0x85, 0xFD, 0xD3, 0x40, 0x98, 0x1A, 0x8C } };



// 控件类型信息

static const DWORD BASED_CODE _dwwebuiOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CwebuiCtrl, IDS_WEBUI, _dwwebuiOleMisc)



// CwebuiCtrl::CwebuiCtrlFactory::UpdateRegistry -
// 添加或移除 CwebuiCtrl 的系统注册表项

BOOL CwebuiCtrl::CwebuiCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_WEBUI,
			IDB_WEBUI,
			afxRegApartmentThreading,
			_dwwebuiOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CwebuiCtrl::CwebuiCtrl - 构造函数

CwebuiCtrl::CwebuiCtrl()
{
	InitializeIIDs(&IID_Dwebui, &IID_DwebuiEvents);
	// TODO: 在此初始化控件的实例数据。
}



// CwebuiCtrl::~CwebuiCtrl - 析构函数

CwebuiCtrl::~CwebuiCtrl()
{
	// TODO: 在此清理控件的实例数据。
	TerminateThread(m_pThread->m_hThread,0);
}



// CwebuiCtrl::OnDraw - 绘图函数

void CwebuiCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

}



// CwebuiCtrl::DoPropExchange - 持久性支持

void CwebuiCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}



// CwebuiCtrl::OnResetState - 将控件重置为默认状态

void CwebuiCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}



// CwebuiCtrl::AboutBox - 向用户显示“关于”框

void CwebuiCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_WEBUI);
	dlgAbout.DoModal();
}


static UINT UpdataThread(LPVOID pParam)
{
	CwebuiPropPage *pPage = (CwebuiPropPage *)pParam;
	while (1)
	{
		if (pPage != NULL)
		{
			pPage->GetTempData();
			pPage->GetHumiData();
			pPage->GetLuxData();
			pPage->GetAlarmStatus();
		}
		Sleep(20);
	}
}
// CwebuiCtrl 消息处理程序

int CwebuiCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	mainPage.Create(IDD_PROPPAGE_WEBUI,this);
	mainPage.MoveWindow(0,0,800,600);
	mainPage.ShowWindow(SW_SHOW);

	m_pThread = AfxBeginThread(UpdataThread, &mainPage);

	return 0;
}
