#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>

#define SPI_IOC_MAGIC 'k'
typedef unsigned int __u32;

#define SPI_IOC_RD_REG         _IOR(SPI_IOC_MAGIC, 5, __u32)
#define SPI_IOC_WR_REG         _IOW(SPI_IOC_MAGIC, 5, __u32)
typedef unsigned char uchar;
int fd;

//数组最大长度
#define MAX_LEN 16

/////////////////////////////////////////////////////////////////////
//set the pin
/////////////////////////////////////////////////////////////////////
const int chipSelectPin = 10;
const int NRSTPD = 5;



//MF522 command bits
#define PCD_IDLE 				0x00 //NO action;取消当前命令
#define PCD_AUTHENT 			0x0E //验证密钥
#define PCD_RECEIVE 			0x08 //r接收数据
#define PCD_TRANSMIT 			0x04 //发送数据
#define PCD_TRANSCEIVE 			0x0C //发送并接收数据
#define PCD_RESETPHASE		 	0x0F //复位
#define PCD_CALCCRC 			0x03 //CRC计算

//Mifare_One卡片命令字
#define PICC_REQIDL 			0x26 //寻找天线范围内没有进入休眠状态的卡片
//寻天线区内未进入休眠状态
#define PICC_REQALL 			0x52 //寻天线区内全部卡
#define PICC_ANTICOLL 			0x93 //防冲撞
#define PICC_SElECTTAG 			0x93 //选卡
#define PICC_AUTHENT1A 			0x60 //验证A密钥
#define PICC_AUTHENT1B			0x61 //验证B密钥
#define PICC_READ 				0x30 //读块
#define PICC_WRITE 				0xA0 //写块
#define PICC_DECREMENT 			0xC0 //降值
#define PICC_INCREMENT 			0xC1 //升值
#define PICC_RESTORE 			0xC2 //调块数据到缓冲区
#define PICC_TRANSFER 			0xB0 //保存缓冲区中数据
#define PICC_HALT 				0x50 //休眠


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
#define SerialSpeedReg 0x1F
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
//-----------------------------------------------

//4字节卡序列号，第5字节为校验字节
uchar serNum[5];



uchar Read_MFRC522(uchar addr)
{
    uchar buf[2];
    buf[0] =addr;
    ioctl(fd, SPI_IOC_RD_REG,buf);
    return buf[1];
}
void Write_MFRC522(uchar addr, uchar val)
{
    uchar buf[2];
    buf[0] = addr;
    buf[1] = val;
    ioctl(fd, SPI_IOC_WR_REG, buf);
}
/*
 * 函 数 名：ShowCardID
 * 功能描述：显示卡片ID号
 * 输入参数：ID string
 * 返 回 值：无
 */
void ShowCardID(uchar *id)
{
    int IDlen=4;
    int i;
    printf("0x");
    for(i=0; i<IDlen; i++){
        printf("%x",0x0F & (id[i]>>4));
        printf("%x",0x0F & id[i]);
    }
    printf("\n");
}

/*
 * 函 数 名：ShowCardType
 * 功能描述：显示卡片类型
 * 输入参数：Type string
 * 返 回 值：无
 */
void ShowCardType(uchar* type)
{
    printf("Card type: ");
    if(type[0]==0x04&&type[1]==0x00) 
        printf("MFOne-S50");
    else if(type[0]==0x02&&type[1]==0x00)
        printf("MFOne-S70");
    else if(type[0]==0x44&&type[1]==0x00)
        printf("MF-UltraLight");
    else if(type[0]==0x08&&type[1]==0x00)
        printf("MF-Pro");
    else if(type[0]==0x44&&type[1]==0x03)
        printf("MF Desire");
    else
        printf("Unknown");
    printf("\n");
}

/*
* 函 数 名：SetBitMask
* 功能描述：置RC522寄存器位
* 输入参数：reg--寄存器地址;mask--置位值
* 返 回 值：无
*/
void SetBitMask(uchar reg, uchar mask) 
{
    uchar tmp;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp | mask); // set bit mask
}


/*
* 函 数 名：ClearBitMask
* 功能描述：清RC522寄存器位
* 输入参数：reg--寄存器地址;mask--清位值
* 返 回 值：无
*/
void ClearBitMask(uchar reg, uchar mask) 
{
    uchar tmp;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp & (~mask)); // clear bit mask
} 


/*
* 函 数 名：AntennaOn
* 功能描述：开启天线,每次启动或关闭天险发射之间应至少有1ms的间隔
* 输入参数：无
* 返 回 值：无
*/
void AntennaOn(void)
{
    uchar temp;

    temp = Read_MFRC522(TxControlReg);
    if (!(temp & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}


/*
* 函 数 名：AntennaOff
* 功能描述：关闭天线,每次启动或关闭天险发射之间应至少有1ms的间隔
* 输入参数：无
* 返 回 值：无
*/
void AntennaOff(void)
{
    ClearBitMask(TxControlReg, 0x03);
}


/*
* 函 数 名：ResetMFRC522
* 功能描述：复位RC522
* 输入参数：无
* 返 回 值：无
*/
void MFRC522_Reset(void)
{
    Write_MFRC522(CommandReg, PCD_RESETPHASE);
}


/*
* 函 数 名：InitMFRC522
* 功能描述：初始化RC522
* 输入参数：无
* 返 回 值：无
*/
void MFRC522_Init(void)
{

    MFRC522_Reset();
         
    //Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
    Write_MFRC522(TModeReg, 0x8D); //Tauto=1; f(Timer) = 6.78MHz/TPreScaler
    Write_MFRC522(TPrescalerReg, 0x3E); //TModeReg[3..0] + TPrescalerReg
    Write_MFRC522(TReloadRegL, 30); 
    Write_MFRC522(TReloadRegH, 0);
    
    Write_MFRC522(TxAutoReg, 0x40); //100%ASK
    Write_MFRC522(ModeReg, 0x3D); //CRC 初始值 0x6363 ???

    //ClearBitMask(Status2Reg, 0x08); //MFCrypto1On=0
    //Write_MFRC522(RxSelReg, 0x86); //RxWait = RxSelReg[5..0]
    //Write_MFRC522(RFCfgReg, 0x7F); //RxGain = 48dB

    AntennaOn(); //打开天线
}

uchar MFRC522_ToCard(uchar command, uchar *sendData, uchar sendLen, uchar *backData, uint *backLen);
/*
* 函 数 名：MFRC522_Request
* 功能描述：寻卡，读取卡类型号
* 输入参数：reqMode--寻卡方式，
* TagType--返回卡片类型
* 0x4400 = Mifare_UltraLight
* 0x0400 = Mifare_One(S50)
* 0x0200 = Mifare_One(S70)
* 0x0800 = Mifare_Pro(X)
* 0x4403 = Mifare_DESFire
* 返 回 值：成功返回MI_OK
*/
uchar MFRC522_Request(uchar reqMode, uchar *TagType)
{
    uchar status; 
    uint backBits; //接收到的数据位数

    Write_MFRC522(BitFramingReg, 0x07); //TxLastBists = BitFramingReg[2..0] ???
    
    TagType[0] = reqMode;
    status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

    if ((status != MI_OK) || (backBits != 0x10))
    {
        status = MI_ERR;
    }
   
    return status;
}


/*
* 函 数 名：MFRC522_ToCard
* 功能描述：RC522和ISO14443卡通讯
* 输入参数：command--MF522命令字，
* sendData--通过RC522发送到卡片的数据, 
* sendLen--发送的数据长度                 
* backData--接收到的卡片返回数据，
* backLen--返回数据的位长度
* 返 回 值：成功返回MI_OK
*/
uchar MFRC522_ToCard(uchar command, uchar *sendData, uchar sendLen, uchar *backData, uint *backLen)
{
    uchar status = MI_ERR;
    uchar irqEn = 0x00;
    uchar waitIRq = 0x00;
    uchar lastBits;
    uchar n;
    uint i;

    switch (command)
    {
        case PCD_AUTHENT: //认证卡密
        {
            irqEn = 0x12;
            waitIRq = 0x10;
            break;
        }
        case PCD_TRANSCEIVE: //发送FIFO中数据
        {
            irqEn = 0x77;
            waitIRq = 0x30;
            break;
        }
        default:
            break;
    }
   
    Write_MFRC522(CommIEnReg, irqEn|0x80); 		//允许中断请求
    ClearBitMask(CommIrqReg, 0x80); 			//清除所有中断请求位
    SetBitMask(FIFOLevelReg, 0x80);				//FlushBuffer=1, FIFO初始化
    
    Write_MFRC522(CommandReg, PCD_IDLE); 		//没有动作;取消当前命令 ???

   //向FIFO中写入数据
    for (i=0; i<sendLen; i++)
    { 
        Write_MFRC522(FIFODataReg, sendData[i]); 
    }

    //执行命令
    Write_MFRC522(CommandReg, command);
    if (command == PCD_TRANSCEIVE)
    { 
        SetBitMask(BitFramingReg, 0x80); 		//StartSend=1,传输数据的开始 
    } 
    
    //等待接收数据完成
    i = 2000; 									//i根据时钟频率调整，操作M1卡最大等待时间25ms        ???
    do 
    {
        //CommIrqReg[7..0]
        //Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
        n = Read_MFRC522(CommIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitIRq));

    ClearBitMask(BitFramingReg, 0x80); 			//StartSend=0
    
    if (i != 0)
    {
        uchar ret;
        ret = Read_MFRC522(ErrorReg);
        if(!(ret & 0x1B)) 	//BufferOvfl Collerr CRCErr ProtecolErr
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            { 
                status = MI_NOTAGERR; //?? 
            }
            
            if (command == PCD_TRANSCEIVE)
            {
                n = Read_MFRC522(FIFOLevelReg);
                lastBits = Read_MFRC522(ControlReg) & 0x07;
                if (lastBits)
                { 
                    *backLen = (n-1)*8 + lastBits; 
                }
                else
                { 
                    *backLen = n*8; 
                }
                
                if (n == 0)
                { 
                    n = 1; 
                }
                if (n > MAX_LEN)
                { 
                    n = MAX_LEN; 
                }
                
                //读取FIFO中接收到的数据
                for (i=0; i<n; i++)
                { 
                    backData[i] = Read_MFRC522(FIFODataReg); 
                }
            }
        }
        else
        { 
            status = MI_ERR; 
        }
        
    }
    
    //SetBitMask(ControlReg,0x80); //timer stops
    //Write_MFRC522(CommandReg, PCD_IDLE); 

    return status;
}


/*
* 函 数 名：MFRC522_Anticoll
* 功能描述：防冲突检测，读取选中卡片的卡序列号
* 输入参数：serNum--返回4字节卡序列号,第5字节为校验字节
* 返 回 值：成功返回MI_OK
*/
uchar MFRC522_Anticoll(uchar *serNum)
{
    uchar status;
    uchar i;
    uchar serNumCheck=0;
    uint unLen;
    
    //ClearBitMask(Status2Reg, 0x08); //strSensclear
    //ClearBitMask(CollReg,0x80); //ValuesAfterColl
    Write_MFRC522(BitFramingReg, 0x00); //TxLastBists = BitFramingReg[2..0]
 
    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

    if (status == MI_OK)
    {
        //校验卡序列号
        for (i=0; i<4; i++)
        { 
            serNumCheck ^= serNum[i];
        }
        if (serNumCheck != serNum[i])
        { 
            status = MI_ERR; 
        }
    }

    //SetBitMask(CollReg, 0x80); //ValuesAfterColl=1

    return status;
} 


/*
* 函 数 名：CalulateCRC
* 功能描述：用MF522计算CRC
* 输入参数：pIndata--要读数CRC的数据，len--数据长度，pOutData--计算的CRC结果
* 返 回 值：无
*/
void CalulateCRC(uchar *pIndata, uchar len, uchar *pOutData)
{
    uchar i, n;

    ClearBitMask(DivIrqReg, 0x04); //CRCIrq = 0
    SetBitMask(FIFOLevelReg, 0x80); //清FIFO指针
    //Write_MFRC522(CommandReg, PCD_IDLE);

    //向FIFO中写入数据 
    for (i=0; i<len; i++)
    { 
        Write_MFRC522(FIFODataReg, *(pIndata+i)); 
    }
    Write_MFRC522(CommandReg, PCD_CALCCRC);

    //等待CRC计算完成
    i = 0xFF;
    do 
    {
        n = Read_MFRC522(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04)); //CRCIrq = 1

    //读取CRC计算结果
    pOutData[0] = Read_MFRC522(CRCResultRegL);
    pOutData[1] = Read_MFRC522(CRCResultRegM);
}



/*
* 函 数 名：MFRC522_Write
* 功能描述：写块数据
* 输入参数：blockAddr--块地址;writeData--向块写16字节数据
* 返 回 值：成功返回MI_OK
*/
uchar MFRC522_Write(uchar blockAddr, uchar *writeData)
{
    uchar status;
    uint recvBits;
    uchar i;
    uchar buff[18]; 
    
    buff[0] = PICC_WRITE;
    buff[1] = blockAddr;
    CalulateCRC(buff, 2, &buff[2]);
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);

    if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
    { 
        status = MI_ERR; 
    }
        
    if (status == MI_OK)
    {
        for (i=0; i<16; i++) //向FIFO写16Byte数据
        { 
            buff[i] = *(writeData+i); 
        }
        CalulateCRC(buff, 16, &buff[16]);
        status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);
        
        if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
        { 
            status = MI_ERR; 
        }
    }
    
    return status;
}


/*
* 函 数 名：MFRC522_Halt
* 功能描述：命令卡片进入休眠状态
* 输入参数：无
* 返 回 值：无
*/
void MFRC522_Halt(void)
{
    uchar status;
    uint unLen;
    uchar buff[4]; 

    buff[0] = PICC_HALT;
    buff[1] = 0;
    CalulateCRC(buff, 2, &buff[2]);
 
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff,&unLen);
}

void loop();
int main()
{
    int i;
    fd = open("/dev/spidev0.0",O_RDWR, 0);
    if(fd < 0)
    {
        perror("Open AudioCtrl fail!");
        return 0;
    }
    for(i = 0x1; i < 0x10; i++)
    {
    //    Write_MFRC522(i,0x55);
    }
    for(i = 0x1; i < 0x10; i++) 
    {
        printf("addr = 0x%x, val = 0x%x\n",i, Read_MFRC522(i));
    }
    MFRC522_Init();
    while(1)
        loop(); 

    close(fd);
    return 0;

}
void loop()
{

    uchar status;
    uchar str[MAX_LEN];

    
    // 寻卡，返回卡类型 
    status = MFRC522_Request(PICC_REQIDL, str); 
    if (status != MI_OK)
    {
   //     printf("Request Error!\n");
        return;
    }
    
    
    // Show card type
    ShowCardType(str);
    
    //防冲撞，返回卡的序列号 4字节
    status = MFRC522_Anticoll(str);
    
    // str[0..3]: serial number of the card
    // str[4]: XOR checksum of the SN.
    if (status == MI_OK)
    {
        printf("The card's number is: ");
        memcpy(serNum, str, 5);
        ShowCardID(serNum);
    
        // Check people associated with card ID
        uchar* id = serNum;
        if( id[0]==0x53 && id[1]==0x68 && id[2]==0x3B && id[3]==0x8B ) {
            printf("Hello CloudSail!\n");
        } else if(id[0]==0x3B && id[1]==0xE6 && id[2]==0xD1 && id[3]==0x3B) {
            printf("Hello Orange!\n");
        }else{
            printf("Hello unkown guy!\n");
        }
    }

   
    MFRC522_Halt(); //command the card into sleep mode 

    usleep(200000);
}
