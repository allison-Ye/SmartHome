#pragma once


// CVidWnd 对话框

class CVidWnd : public CDialog
{
	DECLARE_DYNAMIC(CVidWnd)

public:
	CVidWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVidWnd();

// 对话框数据
	enum { IDD = IDD_VIDWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
