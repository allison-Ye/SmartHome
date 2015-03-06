#ifndef _RFID_DEV_H
#define _RFID_DEV_H
#include <sys/ioctl.h>
#include <common.h>

typedef unsigned int __u32;
typedef unsigned char uchar;

#define SPI_IOC_MAGIC 'k'
#define SPI_IOC_RD_REG         _IOR(SPI_IOC_MAGIC, 5, __u32)
#define SPI_IOC_WR_REG         _IOW(SPI_IOC_MAGIC, 5, __u32)

#define MAX_LEN 16

#define PCD_IDLE                                0x00 //NO action;取消当前命令
#define PCD_AUTHENT                     0x0E //验证密钥
#define PCD_RECEIVE                     0x08 //r接收数据
#define PCD_TRANSMIT                    0x04 //发送数据
#define PCD_TRANSCEIVE                  0x0C //发送并接收数据
#define PCD_RESETPHASE                  0x0F //复位
#define PCD_CALCCRC                     0x03 //CRC计算

//Mifare_One卡片命令字
#define PICC_REQIDL                     0x26 //寻找天线范围内没有进入休眠状态的卡片
//寻天线区内未进入休眠状态
#define PICC_REQALL                     0x52 //寻天线区内全部卡
#define PICC_ANTICOLL                   0x93 //防冲撞
#define PICC_SElECTTAG                  0x93 //选卡
#define PICC_AUTHENT1A                  0x60 //验证A密钥
#define PICC_AUTHENT1B                  0x61 //验证B密钥
#define PICC_READ                               0x30 //读块
#define PICC_WRITE                              0xA0 //写块
#define PICC_DECREMENT                  0xC0 //降值
#define PICC_INCREMENT                  0xC1 //升值
#define PICC_RESTORE                    0xC2 //调块数据到缓冲区
#define PICC_TRANSFER                   0xB0 //保存缓冲区中数据
#define PICC_HALT                               0x50 //休眠


//和MF522通讯时返回的错误代码
#define MI_OK 0
#define MI_NOTAGERR 1
#define MI_ERR 2

//------------------MFRC522寄存器---------------
//页 0:命令和状态
#define Reserved00 0x00 
#define CommandReg 0x01 
#define CommIEnReg 0x02 
#define DivlEnReg 0x03 
#define CommIrqReg 0x04 
#define DivIrqReg 0x05
#define ErrorReg 0x06 
#define Status1Reg 0x07 
#define Status2Reg 0x08 
#define FIFODataReg 0x09
#define FIFOLevelReg 0x0A
#define WaterLevelReg 0x0B
#define ControlReg 0x0C
#define BitFramingReg 0x0D
#define CollReg 0x0E
#define Reserved01 0x0F
//页 1:命令
#define Reserved10 0x10
#define ModeReg 0x11
#define TxModeReg 0x12
#define RxModeReg 0x13
#define TxControlReg 0x14
#define TxAutoReg 0x15
#define TxSelReg 0x16
#define RxSelReg 0x17
#define RxThresholdReg 0x18
#define DemodReg 0x19
#define Reserved11 0x1A
#define Reserved12 0x1B
#define MifareReg 0x1C
#define Reserved13 0x1D
#define Reserved14 0x1E
//页 2:CFG 
#define Reserved20 0x20 
#define CRCResultRegM 0x21
#define CRCResultRegL 0x22
#define Reserved21 0x23
#define ModWidthReg 0x24
#define Reserved22 0x25
#define RFCfgReg 0x26
#define GsNReg 0x27
#define CWGsPReg 0x28
#define ModGsPReg 0x29
#define TModeReg 0x2A
#define TPrescalerReg 0x2B
#define TReloadRegH 0x2C
#define TReloadRegL 0x2D
#define TCounterValueRegH 0x2E
#define TCounterValueRegL 0x2F
//页 3:测试寄存器
#define Reserved30 0x30
#define TestSel1Reg 0x31
#define TestSel2Reg 0x32
#define TestPinEnReg 0x33
#define TestPinValueReg 0x34
#define TestBusReg 0x35
#define AutoTestReg 0x36
#define VersionReg 0x37
#define AnalogTestReg 0x38
#define TestDAC1Reg 0x39 
#define TestDAC2Reg 0x3A 
#define TestADCReg 0x3B 
#define Reserved31 0x3C 
#define Reserved32 0x3D 
#define Reserved33 0x3E 
#define Reserved34 0x3F


int rfid_dev_init(GlobalHandle * handle);
void rfid_dev_exit(GlobalHandle * handle);
int rfid_dev_search_card(GlobalHandle * handle, uchar * id);
void MFRC522_Init(GlobalHandle * handle);
#endif
