#pragma once
#include "webuiPropPage.h"
// webuiCtrl.h : CwebuiCtrl ActiveX 控件类的声明。


// CwebuiCtrl : 有关实现的信息，请参阅 webuiCtrl.cpp。

class CwebuiCtrl : public COleControl
{
	DECLARE_DYNCREATE(CwebuiCtrl)

// 构造函数
public:
	CwebuiCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CwebuiCtrl();

	DECLARE_OLECREATE_EX(CwebuiCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CwebuiCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CwebuiCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CwebuiCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// 事件映射
	DECLARE_EVENT_MAP()
   
	CwebuiPropPage mainPage;
	CWinThread *m_pThread;
// 调度和事件 ID
public:
	enum {
	};
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

