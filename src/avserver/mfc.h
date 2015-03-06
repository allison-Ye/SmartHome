#ifndef _MFC_H_
#define _MFC_H_

#include "SsbSipMfcApi.h"

typedef struct 
{
    void *hOpen;
    unsigned char header[100];
    int headerSize;
    SSBSIP_MFC_ENC_H264_PARAM *param;
    int width, height;
    SSBSIP_MFC_CODEC_TYPE codec_type;
    SSBSIP_MFC_ENC_INPUT_INFO input_info;
    SSBSIP_MFC_ENC_OUTPUT_INFO output_info;
}mfc_handle;

#endif // MFC_H
