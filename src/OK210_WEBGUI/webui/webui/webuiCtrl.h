#pragma once
#include "webuiPropPage.h"
// webuiCtrl.h : CwebuiCtrl ActiveX �ؼ����������


// CwebuiCtrl : �й�ʵ�ֵ���Ϣ������� webuiCtrl.cpp��

class CwebuiCtrl : public COleControl
{
	DECLARE_DYNCREATE(CwebuiCtrl)

// ���캯��
public:
	CwebuiCtrl();

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~CwebuiCtrl();

	DECLARE_OLECREATE_EX(CwebuiCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(CwebuiCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CwebuiCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CwebuiCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// �¼�ӳ��
	DECLARE_EVENT_MAP()
   
	CwebuiPropPage mainPage;
	CWinThread *m_pThread;
// ���Ⱥ��¼� ID
public:
	enum {
	};
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

