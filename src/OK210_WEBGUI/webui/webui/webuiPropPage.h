#pragma once
#include "afxwin.h"
#include "VidWnd.h"
#include "vlc/vlc.h"
#include "curl.h"
// webuiPropPage.h : CwebuiPropPage 属性页类的声明。


// CwebuiPropPage : 有关实现的信息，请参阅 webuiPropPage.cpp。

class CwebuiPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CwebuiPropPage)
	DECLARE_OLECREATE_EX(CwebuiPropPage)

// 构造函数
public:
	CwebuiPropPage();
	~CwebuiPropPage();
// 对话框数据
	enum { IDD = IDD_PROPPAGE_WEBUI };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()

private:
	
	CVidWnd * VideoWnd;
	libvlc_instance_t * inst;
	libvlc_media_player_t *mp;
	libvlc_media_t *m;
	CURL * pCurl;
	CString URL;
	string data;
	double len;
	
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonLed1();
	afx_msg void OnBnClickedButtonLed2();
	afx_msg void OnBnClickedButtonMotdir1();
	afx_msg void OnBnClickedButtonMotdir2();
	afx_msg void OnBnClickedButtonMotstop();
	afx_msg void OnBnClickedButtonBeepon();
	afx_msg void OnBnClickedButtonBeepoff();
	afx_msg int GetTempData();
	afx_msg int GetHumiData();
	afx_msg int GetLuxData();
	afx_msg int GetAlarmStatus();
};