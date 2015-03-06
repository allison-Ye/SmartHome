// webuiPropPage.cpp : CwebuiPropPage 属性页类的实现。

#include "stdafx.h"
#include "webui.h"
#include "webuiPropPage.h"
#include "vlc/vlc.h"
#include "curl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static UINT UpdataThread(LPVOID pParam);
IMPLEMENT_DYNCREATE(CwebuiPropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CwebuiPropPage, COlePropertyPage)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LED1, &CwebuiPropPage::OnBnClickedButtonLed1)
	ON_BN_CLICKED(IDC_BUTTON_LED2, &CwebuiPropPage::OnBnClickedButtonLed2)
	ON_BN_CLICKED(IDC_BUTTON_MOTDIR1, &CwebuiPropPage::OnBnClickedButtonMotdir1)
	ON_BN_CLICKED(IDC_BUTTON_MOTDIR2, &CwebuiPropPage::OnBnClickedButtonMotdir2)
	ON_BN_CLICKED(IDC_BUTTON_MOTSTOP, &CwebuiPropPage::OnBnClickedButtonMotstop)
	ON_BN_CLICKED(IDC_BUTTON_BEEPON, &CwebuiPropPage::OnBnClickedButtonBeepon)
	ON_BN_CLICKED(IDC_BUTTON_BEEPOFF, &CwebuiPropPage::OnBnClickedButtonBeepoff)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CwebuiPropPage, "WEBUI.webuiPropPage.1",
	0xefb9249c, 0x1cc9, 0x4705, 0xbd, 0x2e, 0x6d, 0x5a, 0x62, 0x4e, 0xb2, 0x2c)



// CwebuiPropPage::CwebuiPropPageFactory::UpdateRegistry -
// 添加或移除 CwebuiPropPage 的系统注册表项

BOOL CwebuiPropPage::CwebuiPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_WEBUI_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CwebuiPropPage::CwebuiPropPage - 构造函数

CwebuiPropPage::CwebuiPropPage() :
	COlePropertyPage(IDD, IDS_WEBUI_PPG_CAPTION)
{
}

	CwebuiPropPage::~CwebuiPropPage() 
{
	/* Stop playing */
	   libvlc_media_player_stop (mp);
	/* Free the media_player */
	   libvlc_media_player_release (mp);
	   libvlc_release (inst);
	   curl_close();
}


// CwebuiPropPage::DoDataExchange - 在页和属性间移动数据

void CwebuiPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);

}



// CwebuiPropPage 消息处理程序

BOOL CwebuiPropPage::OnInitDialog()
{
	COlePropertyPage::OnInitDialog();
     
	  OutputDebugString("OnInitDialog\n");

	 ((CButton *)GetDlgItem(IDC_BUTTON_LED1))->SetWindowText("Enable LED1");
	 ((CButton *)GetDlgItem(IDC_BUTTON_LED1))->MoveWindow(500,50,70,25);
	 ((CButton *)GetDlgItem(IDC_BUTTON_LED2))->SetWindowText("Enable LED2");
	 ((CButton *)GetDlgItem(IDC_BUTTON_LED2))->MoveWindow(500,90,70,25);
	 ((CButton *)GetDlgItem(IDC_BUTTON_MOTDIR1))->SetWindowText("Motor dir 1");
	 ((CButton *)GetDlgItem(IDC_BUTTON_MOTDIR1))->MoveWindow(500,130,70,25);
	 ((CButton *)GetDlgItem(IDC_BUTTON_MOTDIR2))->SetWindowText("Motor dir 2");
	 ((CButton *)GetDlgItem(IDC_BUTTON_MOTDIR2))->MoveWindow(500,170,70,25);
	 ((CButton *)GetDlgItem(IDC_BUTTON_MOTSTOP))->SetWindowText("Motor Stop");
	 ((CButton *)GetDlgItem(IDC_BUTTON_MOTSTOP))->MoveWindow(500,210,70,25);
	 ((CButton *)GetDlgItem(IDC_BUTTON_BEEPON))->SetWindowText("Beeper ON");
	 ((CButton *)GetDlgItem(IDC_BUTTON_BEEPON))->MoveWindow(500,250,70,25);
	 ((CButton *)GetDlgItem(IDC_BUTTON_BEEPOFF))->SetWindowText("Beeper OFF");
	 ((CButton *)GetDlgItem(IDC_BUTTON_BEEPOFF))->MoveWindow(500,290,70,25);

	 ((CEdit *)GetDlgItem(IDC_LABEL_TEMP))->SetWindowText("Temperature:");
     ((CEdit *)GetDlgItem(IDC_LABEL_TEMP))->MoveWindow(600,50,100,25);
	 ((CEdit *)GetDlgItem(IDC_LABEL_TEMPVAL))->SetWindowText("0.00 ℃");
	 ((CEdit *)GetDlgItem(IDC_LABEL_TEMPVAL))->MoveWindow(700,50,100,25);
	 ((CEdit *)GetDlgItem(IDC_LABEL_HUMI))->SetWindowText("Humidity:");
	 ((CEdit *)GetDlgItem(IDC_LABEL_HUMI))->MoveWindow(600,90,100,25);
	 ((CEdit *)GetDlgItem(IDC_LABEL_HUMIVAL))->SetWindowText("0.00");
	 ((CEdit *)GetDlgItem(IDC_LABEL_HUMIVAL))->MoveWindow(700,90,100,25);
	 ((CEdit *)GetDlgItem(IDC_LABEL_LUX))->SetWindowText("Illuminance:");
	 ((CEdit *)GetDlgItem(IDC_LABEL_LUX))->MoveWindow(600,130,100,25);
	 ((CEdit *)GetDlgItem(IDC_LABEL_LUXVAL))->SetWindowText("0.00 lux");
	 ((CEdit *)GetDlgItem(IDC_LABEL_LUXVAL))->MoveWindow(700,130,100,25);
	 ((CEdit *)GetDlgItem(IDC_LABEL_ALARM))->SetWindowText("Alarm Status:");
	 ((CEdit *)GetDlgItem(IDC_LABEL_ALARM))->MoveWindow(600,170,100,25);
	 ((CEdit *)GetDlgItem(IDC_LABEL_ALARMVAL))->SetWindowText("Normal");
	 ((CEdit *)GetDlgItem(IDC_LABEL_ALARMVAL))->MoveWindow(700,170,100,25);

     
	 VideoWnd = new CVidWnd(this);
	 VideoWnd->Create(IDD_VIDWND,this);
	 VideoWnd->MoveWindow(50,50,400,300);
	 VideoWnd->ShowWindow(SW_SHOW);

	 pCurl = curl_init();

     
     /* Load the VLC engine */

     inst = libvlc_new (0, NULL);
  
	 //if(inst == NULL)
	//	 return 0;
     /* Create a new item */
      m = libvlc_media_new_location (inst, "rtsp://10.0.1.199:8554/h264ESVideoTest");
     //m = libvlc_media_new_path (inst, "/path/to/test.mov");
        
     /* Create a media player playing environement */
     mp = libvlc_media_player_new_from_media (m);
     
     /* No need to keep the media now */
     libvlc_media_release (m);
 
	 libvlc_media_player_set_hwnd (mp, VideoWnd->GetSafeHwnd());

 
     /* play the media_player */
     libvlc_media_player_play (mp);
    
    

 
	 return FALSE;
}


void CwebuiPropPage::OnBnClickedButtonLed1()
{
    static int led1_enable = 0;
	led1_enable = !led1_enable;

	if (led1_enable == 0)
	{
        ((CButton *)GetDlgItem(IDC_BUTTON_LED1))->SetWindowText("Enable LED1");
	} 
	else
	{
		((CButton *)GetDlgItem(IDC_BUTTON_LED1))->SetWindowText("Disable LED1");
	}
	URL.Format("http://%s/cgi-bin/cmd?setled1=%d","10.0.1.199",led1_enable);
	data.clear();

	curl_down(pCurl, URL, &data, &len, 3);

}

void CwebuiPropPage::OnBnClickedButtonLed2()
{
	// TODO: 在此添加控件通知处理程序代码

	static int led2_enable = 0;
	led2_enable = !led2_enable;
	if (led2_enable == 0)
	{
		((CButton *)GetDlgItem(IDC_BUTTON_LED2))->SetWindowText("Enable LED2");
	} 
	else
	{
		((CButton *)GetDlgItem(IDC_BUTTON_LED2))->SetWindowText("Disable LED2");
	}
	URL.Format("http://%s/cgi-bin/cmd?setled2=%d","10.0.1.199",led2_enable);
	data.clear();
	curl_down(pCurl, URL, &data, &len, 3);
}

void CwebuiPropPage::OnBnClickedButtonMotdir1()
{
	// TODO: 在此添加控件通知处理程序代码
	URL.Format("http://%s/cgi-bin/cmd?setmotdir=%d","10.0.1.199",1);
	data.clear();
	curl_down(pCurl, URL, &data, &len, 3);
}

void CwebuiPropPage::OnBnClickedButtonMotdir2()
{
	// TODO: 在此添加控件通知处理程序代码
	URL.Format("http://%s/cgi-bin/cmd?setmotdir=%d","10.0.1.199",0);
	data.clear();
	curl_down(pCurl, URL, &data, &len, 3);
}

void CwebuiPropPage::OnBnClickedButtonMotstop()
{
	// TODO: 在此添加控件通知处理程序代码
	URL.Format("http://%s/cgi-bin/cmd?setmotstop","10.0.1.199");
	data.clear();
	curl_down(pCurl, URL, &data, &len, 3);
}

void CwebuiPropPage::OnBnClickedButtonBeepon()
{
	// TODO: 在此添加控件通知处理程序代码
	URL.Format("http://%s/cgi-bin/cmd?setbeepen=%d","10.0.1.199",1);
	data.clear();
	curl_down(pCurl, URL, &data, &len, 3);
}

void CwebuiPropPage::OnBnClickedButtonBeepoff()
{
	// TODO: 在此添加控件通知处理程序代码
	URL.Format("http://%s/cgi-bin/cmd?setbeepen=%d","10.0.1.199",0);
	data.clear();
	curl_down(pCurl, URL, &data, &len, 3);
}

int CwebuiPropPage::GetTempData()
{
	int pos1,pos2;
	int i;
	CString debugInfo;
	char value[16] = {0};
	URL.Format("http://%s/cgi-bin/cmd?gettemp","10.0.1.199");
	data.clear();
	curl_down(pCurl, URL, &data, &len, 3);
	pos1 = data.find("[");
	if (pos1 < 0)
	{
         return -1;
	}
	pos2 = data.find("]");
	if (pos2 < 0)
	{
		return -1;
	}
    for(i = pos1+1; i < pos2; i++)
	{
		value[i - pos1 - 1] = data[i];
	}
	strcat_s(value,"℃");
    ((CEdit *)GetDlgItem(IDC_LABEL_TEMPVAL))->SetWindowText(value);
    return 0;
}
int CwebuiPropPage::GetHumiData()
{
	int pos1,pos2;
	int i;
	CString debugInfo;
	char value[16] = {0};
	URL.Format("http://%s/cgi-bin/cmd?gethumi","10.0.1.199");
	data.clear();
	curl_down(pCurl, URL, &data, &len, 3);
	pos1 = data.find("[");
	if (pos1 < 0)
	{
		return -1;
	}
	pos2 = data.find("]");
	if (pos2 < 0)
	{
		return -1;
	}
	for(i = pos1+1; i < pos2; i++)
	{
		value[i - pos1 - 1] = data[i];
	}
	((CEdit *)GetDlgItem(IDC_LABEL_HUMIVAL))->SetWindowText(value);
	return 0;
}
int CwebuiPropPage::GetLuxData()
{
	int pos1,pos2;
	int i;
	CString debugInfo;
	char value[16] = {0};
	URL.Format("http://%s/cgi-bin/cmd?getlux","10.0.1.199");
	data.clear();
	curl_down(pCurl, URL, &data, &len, 3);
	pos1 = data.find("[");
	if (pos1 < 0)
	{
		return -1;
	}
	pos2 = data.find("]");
	if (pos2 < 0)
	{
		return -1;
	}
	for(i = pos1+1; i < pos2; i++)
	{
		value[i - pos1 - 1] = data[i];
	}
	strcat_s(value," lux");
	((CEdit *)GetDlgItem(IDC_LABEL_LUXVAL))->SetWindowText(value);
	return 0;
}
int CwebuiPropPage::GetAlarmStatus()
{
	int pos1,pos2;
	int i;
	CString debugInfo;
	char value[16] = {0};
	URL.Format("http://%s/cgi-bin/cmd?getalarm","10.0.1.199");
	data.clear();
	curl_down(pCurl, URL, &data, &len, 3);
	pos1 = data.find("[");
	if (pos1 < 0)
	{
		return -1;
	}
	pos2 = data.find("]");
	if (pos2 < 0)
	{
		return -1;
	}
	for(i = pos1+1; i < pos2; i++)
	{
		value[i - pos1 - 1] = data[i];
	}
	if (value[0] == '1')
	   ((CEdit *)GetDlgItem(IDC_LABEL_ALARMVAL))->SetWindowText("Audio Warning!");
	else
        ((CEdit *)GetDlgItem(IDC_LABEL_ALARMVAL))->SetWindowText("Normal");
	return 0;
}