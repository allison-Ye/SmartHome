#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "mfc.h"


SSBSIP_MFC_ERROR_CODE initMFC(mfc_handle * handle,int w, int h, int qb)
{
    SSBSIP_MFC_ERROR_CODE ret;
    SSBSIP_MFC_ENC_H264_PARAM *param = handle->param;
    SSBSIP_MFC_ENC_INPUT_INFO * input_info = &handle->input_info;
    SSBSIP_MFC_ENC_OUTPUT_INFO * output_info = &handle->output_info;
    if(handle == NULL)
    {
        printf("handle is NULL\n");
        return MFC_RET_INVALID_PARAM;
    }    

    handle->width=w;
    handle->height=h;
    param = (SSBSIP_MFC_ENC_H264_PARAM*)malloc(sizeof(SSBSIP_MFC_ENC_H264_PARAM));
    memset(param, 0 , sizeof(SSBSIP_MFC_ENC_H264_PARAM));
    param->SourceWidth = handle->width;
    param->SourceHeight = handle->height;
    param->ProfileIDC=1;
    param->LevelIDC=40;
    param->IDRPeriod=3;
    param->NumberReferenceFrames=1;
    param->NumberRefForPframes=2;
    param->SliceMode=0;
    param->SliceArgument=1;
    param->NumberBFrames=0;
    param->LoopFilterDisable=0;
    param->LoopFilterAlphaC0Offset=2;
    param->LoopFilterBetaOffset=1;
    param->SymbolMode=0;
    param->PictureInterlace=0;
    param->Transform8x8Mode=0;
    param->RandomIntraMBRefresh=0;
    param->PadControlOn=0;
    param->LumaPadVal=0;
    param->CbPadVal=0;
    param->CrPadVal=0;
    param->EnableFRMRateControl=0;
    param->EnableMBRateControl=0;
    param->FrameRate=25;
    param->Bitrate=200;
    param->FrameQp=qb;
    param->QSCodeMax=0;
    param->QSCodeMin=0;
    param->CBRPeriodRf=20;
    param->DarkDisable=1;
    param->SmoothDisable=1;
    param->StaticDisable=1;
    param->ActivityDisable=1;

    param->codecType=H264_ENC;

    param->FrameQp_P = param->FrameQp+1;
    param->FrameQp_B = param->FrameQp+3;
    handle->hOpen = SsbSipMfcEncOpen();
    if(handle->hOpen == NULL)
    {
        printf("SsbSipMfcEncOpen Failed\n");
        ret = MFC_RET_FAIL;
        return ret;
    }

    if(SsbSipMfcEncInit(handle->hOpen, param) != MFC_RET_OK)
    {
        printf("SsbSipMfcEncInit Failed\n");
        ret = MFC_RET_FAIL;
        goto out;
    }

    if(SsbSipMfcEncGetInBuf(handle->hOpen, input_info) != MFC_RET_OK)
    {
        printf("SsbSipMfcEncGetInBuf Failed\n");
        ret = MFC_RET_FAIL;
        goto out;
    }

    ret=SsbSipMfcEncGetOutBuf(handle->hOpen, output_info);
    if(output_info->headerSize <= 0)
    {
        printf("Header Encoding Failed\n");
        ret = MFC_RET_FAIL;
        goto out;
    }
    handle->headerSize=output_info->headerSize;
    memcpy(handle->header,output_info->StrmVirAddr,handle->headerSize);
    printf("MFC init success:: Yphy(0x%08x) Cphy(0x%08x)\n",
           input_info->YPhyAddr, input_info->CPhyAddr);
    return ret;
out:
    SsbSipMfcEncClose(handle->hOpen);
    return ret;
}

int getHeader(mfc_handle *handle, unsigned char **p)
{
    *p=handle->header;
    return handle->headerSize;
}

void getInputBuf(mfc_handle * handle,void **Y,void **UV)
{
    *Y=handle->input_info.YVirAddr;
    *UV=handle->input_info.CVirAddr;
}

int encode(mfc_handle * handle,void **h264)
{
    void *hOpen = handle->hOpen;
    SSBSIP_MFC_ENC_OUTPUT_INFO * output_info = &handle->output_info;
    if(SsbSipMfcEncExe(hOpen) != MFC_RET_OK){
        printf("Encoding Failed\n");
        return 0;
    }
    SsbSipMfcEncGetOutBuf(hOpen, output_info);
    if(output_info->StrmVirAddr == NULL)
    {
        printf("SsbSipMfcEncGetOutBuf Failed\n");
        return 0;
    }
    *h264=output_info->StrmVirAddr;
    return output_info->dataSize;
}

void closeMFC(mfc_handle * handle)
{
    SsbSipMfcEncClose(handle->hOpen);
}
