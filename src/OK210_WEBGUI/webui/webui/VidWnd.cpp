// VidWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "webui.h"
#include "VidWnd.h"


// CVidWnd �Ի���

IMPLEMENT_DYNAMIC(CVidWnd, CDialog)

CVidWnd::CVidWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CVidWnd::IDD, pParent)
{

}

CVidWnd::~CVidWnd()
{
}

void CVidWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVidWnd, CDialog)
END_MESSAGE_MAP()


// CVidWnd ��Ϣ�������
