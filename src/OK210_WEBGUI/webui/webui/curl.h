#ifndef _CURL_H_
#define _CURL_H_
#include "afxwin.h"
#include <curl/curl.h>
#include <string>
using namespace std;
int writer(char *data, size_t size, size_t nmemb, string *writerData);
CURL * curl_init();
int curl_down(CURL *curl,CString cmdUrl,string *buf, double * length,int timeout);
void curl_close();
int curl_check_ok(string *buf, int num);
int curl_post(char * url, char * filename,void *pridata,string * resBuf);
extern CString usrpass;
extern CString ipaddr;
extern CString gSnapdir;
extern CString gRecorddir;



#endif