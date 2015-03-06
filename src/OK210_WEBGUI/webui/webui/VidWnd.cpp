// VidWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "webui.h"
#include "VidWnd.h"


// CVidWnd 对话框

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


// CVidWnd 消息处理程序
