#include "stdafx.h"
#include <afxmt.h>
#include "curl.h"
#include <string>
#include <sys/stat.h>
using namespace std;
CURL *g_curl;
struct curl_slist *g_headerlist;
CString usrpass;
CString ipaddr;
CString gSnapdir;
CString gRecorddir;
struct progressData
{
    CProgressCtrl * ctrl;
    CEdit * status;
};
int writer(char *data, size_t size, size_t nmemb, string *writerData)
{
    unsigned long sizes = size * nmemb;
    int total = 0;
    if (writerData == NULL)
		return 0;
    while(total < sizes)
    {
        writerData->append(data, strlen(data));
        total += strlen(data) + 1;
        data += strlen(data) + 1;
    }
    



    return sizes;
}

int write_func(char *data, size_t size, size_t nmemb, void *ptr)
{
	unsigned long sizes = size * nmemb;
   
    FILE *fp = (FILE * )ptr;
    fwrite(data,size,nmemb,fp);

	return sizes;
}


int reader(char *data, size_t size, size_t nmemb, void *ptr)
{
    FILE * fp = (FILE *)ptr;

    fread(data,size,nmemb,fp);    
    CString debuginfo;
    debuginfo.Format("%s",data);
    OutputDebugString(debuginfo);
	return size*nmemb;
}
CMutex * mutex;
CURL * curl_init()
{
	CURL *curl = NULL;
	
	mutex = new CMutex();
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect:";
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl == NULL)
		return NULL;

	headerlist = curl_slist_append(headerlist, buf);
	g_curl = curl;
	g_headerlist = headerlist;
	
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	return curl;
}

int curl_down(CURL *curl,CString cmdUrl, string *buf, double * length, int timeout)
{
    CURLcode res;
	if (timeout < 0)
		timeout = 0;
	mutex->Lock();
	curl_easy_setopt(curl, CURLOPT_URL, cmdUrl);
	curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);  
    curl_easy_setopt(curl, CURLOPT_USERPWD, usrpass);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);
	res = curl_easy_perform(curl);
	mutex->Unlock();
	if (res == CURLE_OK) //成功执行了
	{
		long http_code = 0;
		res = curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
		if (http_code == 200 && res != CURLE_ABORTED_BY_CALLBACK)
		{
			res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD , length); 
			OutputDebugString("Succeeded");
		}
		else if(http_code == 401 && res != CURLE_ABORTED_BY_CALLBACK)
		{
		    OutputDebugString("Unauthorized!");
			return 2;
		}
		else 
		{
			OutputDebugString("Failed");
			return 1;
		}
	}
	else
	{
		return 1;
	}
	return 0;
}
void curl_close()
{	
	curl_easy_cleanup(g_curl);
	curl_slist_free_all(g_headerlist);
}

int curl_check_ok(string *buf, int num)
{
    int pos = -2,i = 0;

	while((pos = buf->find("OK",pos + 2)) != string::npos )
	{
		i++;
	}
    CString debugString;
	debugString.Format("real ok = %d, check ok = %d",num,i);
	OutputDebugString(debugString);

	return num == i ? 0 : -1;
}
int my_progress_func(char *progress_data, double dt, double dn, double ultotal, double ulnow)
{
    static double pos = 0;
    static int count,lastcount = 0;
    CProgressCtrl * progressCtrl = ((struct progressData *)progress_data)->ctrl;
    CEdit * progressStatus = ((struct progressData *)progress_data)->status;
/*
    if (count % 5 == 0)
    {
        progressStatus->Clear();
        progressStatus->SetWindowText("正在升级，请稍后");
    }
	if (count % 5 == 1)
    {
        progressStatus->Clear();
		progressStatus->SetWindowText("正在升级，请稍后.");
    }
	if (count % 5 == 2)
	{
		progressStatus->Clear();
		progressStatus->SetWindowText("正在升级，请稍后..");
	}
	if (count % 5 == 3)
	{
		progressStatus->Clear();
		progressStatus->SetWindowText("正在升级，请稍后...");
	}
	if (count % 5 == 4)
	{
		progressStatus->Clear();
		progressStatus->SetWindowText("正在升级，请稍后....");
	}
*/
    count++;
    if (ultotal != ulnow)
    {
         progressCtrl->SetRange(0,100);
         progressCtrl->SetStep(1);
         pos = ulnow / (ultotal / 50);
         progressCtrl->SetPos(pos);
    }
    else if (count - lastcount > 500)
    {
         pos++;
         if (pos > 95)
         {
             pos = 95;
         }
         progressCtrl->SetPos(pos);
    }
    
    return 0;
}



int curl_post(char * url, char * filename,void *pridata,string * resBuf)
{
    CURL *curl = curl_easy_init();
	char *progress_data = "* ";

	struct curl_httppost *formpost = 0;
	struct curl_httppost *lastptr  = 0;

	curl_formadd(&formpost,  
		&lastptr,  
		CURLFORM_COPYNAME, "sendfile",  
		CURLFORM_FILE, filename,  
		CURLFORM_END);  

	/* Fill in the filename field */  
	curl_formadd(&formpost,  
		&lastptr,  
		CURLFORM_COPYNAME, "filename",  
		CURLFORM_COPYCONTENTS, filename,  
		CURLFORM_END);  

	/* Fill in the submit field too, even if this is rarely needed */  
	curl_formadd(&formpost,  
		&lastptr,  
		CURLFORM_COPYNAME, "submit",  
		CURLFORM_COPYCONTENTS, "send",  
		CURLFORM_END);  
	curl_easy_setopt(curl, CURLOPT_URL, url);     
	//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);   
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);   
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);

	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, my_progress_func);

	curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, pridata);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, resBuf);

    CURLcode res = curl_easy_perform(curl);

    return res;
}