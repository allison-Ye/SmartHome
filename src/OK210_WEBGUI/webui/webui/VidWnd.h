#pragma once


// CVidWnd �Ի���

class CVidWnd : public CDialog
{
	DECLARE_DYNAMIC(CVidWnd)

public:
	CVidWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVidWnd();

// �Ի�������
	enum { IDD = IDD_VIDWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
