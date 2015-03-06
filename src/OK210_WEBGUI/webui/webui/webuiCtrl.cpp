// webuiCtrl.cpp : CwebuiCtrl ActiveX �ؼ����ʵ�֡�

#include "stdafx.h"
#include "webui.h"
#include "webuiCtrl.h"
#include "webuiPropPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CwebuiCtrl, COleControl)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CwebuiCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// ����ӳ��

BEGIN_DISPATCH_MAP(CwebuiCtrl, COleControl)
	DISP_FUNCTION_ID(CwebuiCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()



// �¼�ӳ��

BEGIN_EVENT_MAP(CwebuiCtrl, COleControl)
END_EVENT_MAP()



// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CwebuiCtrl, 1)
	PROPPAGEID(CwebuiPropPage::guid)
END_PROPPAGEIDS(CwebuiCtrl)



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CwebuiCtrl, "WEBUI.webuiCtrl.1",
	0x55608e4b, 0x7384, 0x4bfb, 0x8a, 0xd, 0x9c, 0x3b, 0x4c, 0x77, 0x74, 0xf8)



// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CwebuiCtrl, _tlid, _wVerMajor, _wVerMinor)



// �ӿ� ID

const IID BASED_CODE IID_Dwebui =
		{ 0x50F9499A, 0xA5CB, 0x4174, { 0x8E, 0x25, 0xFE, 0x2F, 0x1E, 0xA, 0xFB, 0xDC } };
const IID BASED_CODE IID_DwebuiEvents =
		{ 0xAA1FBA33, 0xBFF6, 0x4458, { 0xA4, 0x85, 0xFD, 0xD3, 0x40, 0x98, 0x1A, 0x8C } };



// �ؼ�������Ϣ

static const DWORD BASED_CODE _dwwebuiOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CwebuiCtrl, IDS_WEBUI, _dwwebuiOleMisc)



// CwebuiCtrl::CwebuiCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CwebuiCtrl ��ϵͳע�����

BOOL CwebuiCtrl::CwebuiCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��

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



// CwebuiCtrl::CwebuiCtrl - ���캯��

CwebuiCtrl::CwebuiCtrl()
{
	InitializeIIDs(&IID_Dwebui, &IID_DwebuiEvents);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
}



// CwebuiCtrl::~CwebuiCtrl - ��������

CwebuiCtrl::~CwebuiCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
	TerminateThread(m_pThread->m_hThread,0);
}



// CwebuiCtrl::OnDraw - ��ͼ����

void CwebuiCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

}



// CwebuiCtrl::DoPropExchange - �־���֧��

void CwebuiCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
}



// CwebuiCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CwebuiCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}



// CwebuiCtrl::AboutBox - ���û���ʾ�����ڡ���

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
// CwebuiCtrl ��Ϣ�������

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
