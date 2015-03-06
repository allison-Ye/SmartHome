#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>
#include "rfid_dev.h"

#define RFIDDEV_PATH "/dev/spidev0.0"

//4字节卡序列号，第5字节为校验字节
uchar serNum[5];

int rfid_dev_init(GlobalHandle * handle)
{
    int fd;
    if(handle == NULL)
    {
        printf("%s:handle is NULL\n",__func__);
        return -1;
    }
    fd = open(RFIDDEV_PATH, O_RDWR, 0);
    if(fd < 0)
    {
        printf("%s:",__func__);
        perror("open");
        return -1;
    }
    handle->rfid_fd = fd;
    MFRC522_Init(handle);
    return 0;
}

void rfid_dev_exit(GlobalHandle * handle)
{
    if(handle == NULL)
        return;
    close(handle->rfid_fd);
}

uchar Read_MFRC522(GlobalHandle * handle,uchar addr)
{
    int fd = handle->rfid_fd;
    uchar buf[2];
    buf[0] =addr;
    ioctl(fd, SPI_IOC_RD_REG,buf);
    return buf[1];
}
void Write_MFRC522(GlobalHandle * handle,uchar addr, uchar val)
{
    uchar buf[2];
    int fd = handle->rfid_fd;
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
void SetBitMask(GlobalHandle * handle,uchar reg, uchar mask) 
{
    uchar tmp;
    tmp = Read_MFRC522(handle, reg);
    Write_MFRC522(handle, reg, tmp | mask); // set bit mask
}


/*
* 函 数 名：ClearBitMask
* 功能描述：清RC522寄存器位
* 输入参数：reg--寄存器地址;mask--清位值
* 返 回 值：无
*/
void ClearBitMask(GlobalHandle * handle, uchar reg, uchar mask) 
{
    uchar tmp;
    tmp = Read_MFRC522(handle, reg);
    Write_MFRC522(handle, reg, tmp & (~mask)); // clear bit mask
} 


/*
* 函 数 名：AntennaOn
* 功能描述：开启天线,每次启动或关闭天险发射之间应至少有1ms的间隔
* 输入参数：无
* 返 回 值：无
*/
void AntennaOn(GlobalHandle * handle)
{
    uchar temp;

    temp = Read_MFRC522(handle, TxControlReg);
    if (!(temp & 0x03))
    {
        SetBitMask(handle, TxControlReg, 0x03);
    }
}


/*
* 函 数 名：AntennaOff
* 功能描述：关闭天线,每次启动或关闭天险发射之间应至少有1ms的间隔
* 输入参数：无
* 返 回 值：无
*/
void AntennaOff(GlobalHandle * handle)
{
    ClearBitMask(handle, TxControlReg, 0x03);
}


/*
* 函 数 名：ResetMFRC522
* 功能描述：复位RC522
* 输入参数：无
* 返 回 值：无
*/
void MFRC522_Reset(GlobalHandle * handle)
{
    Write_MFRC522(handle, CommandReg, PCD_RESETPHASE);
}


/*
* 函 数 名：InitMFRC522
* 功能描述：初始化RC522
* 输入参数：无
* 返 回 值：无
*/
void MFRC522_Init(GlobalHandle * handle)
{

    MFRC522_Reset(handle);
         
    //Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
    Write_MFRC522(handle, TModeReg, 0x8D); //Tauto=1; f(Timer) = 6.78MHz/TPreScaler
    Write_MFRC522(handle, TPrescalerReg, 0x3E); //TModeReg[3..0] + TPrescalerReg
    Write_MFRC522(handle, TReloadRegL, 30); 
    Write_MFRC522(handle, TReloadRegH, 0);
    
    Write_MFRC522(handle, TxAutoReg, 0x40); //100%ASK
    Write_MFRC522(handle, ModeReg, 0x3D); //CRC 初始值 0x6363 ???

    //ClearBitMask(Status2Reg, 0x08); //MFCrypto1On=0
    //Write_MFRC522(RxSelReg, 0x86); //RxWait = RxSelReg[5..0]
    //Write_MFRC522(RFCfgReg, 0x7F); //RxGain = 48dB

    AntennaOn(handle); //打开天线
}

uchar MFRC522_ToCard(GlobalHandle * handle, uchar command, uchar *sendData, uchar sendLen, uchar *backData, uint *backLen);
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
uchar MFRC522_Request(GlobalHandle * handle, uchar reqMode, uchar *TagType)
{
    uchar status; 
    uint backBits; //接收到的数据位数

    Write_MFRC522(handle, BitFramingReg, 0x07); //TxLastBists = BitFramingReg[2..0] ???
    
    TagType[0] = reqMode;
    status = MFRC522_ToCard(handle, PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

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
uchar MFRC522_ToCard(GlobalHandle * handle, uchar command, uchar *sendData, uchar sendLen, uchar *backData, uint *backLen)
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
   
    Write_MFRC522(handle, CommIEnReg, irqEn|0x80); 		//允许中断请求
    ClearBitMask(handle, CommIrqReg, 0x80); 			//清除所有中断请求位
    SetBitMask(handle, FIFOLevelReg, 0x80);				//FlushBuffer=1, FIFO初始化
    
    Write_MFRC522(handle, CommandReg, PCD_IDLE); 		//没有动作;取消当前命令 ???

   //向FIFO中写入数据
    for (i=0; i<sendLen; i++)
    { 
        Write_MFRC522(handle, FIFODataReg, sendData[i]); 
    }

    //执行命令
    Write_MFRC522(handle, CommandReg, command);
    if (command == PCD_TRANSCEIVE)
    { 
        SetBitMask(handle, BitFramingReg, 0x80); 		//StartSend=1,传输数据的开始 
    } 
    
    //等待接收数据完成
    i = 2000; 									//i根据时钟频率调整，操作M1卡最大等待时间25ms        ???
    do 
    {
        //CommIrqReg[7..0]
        //Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
        n = Read_MFRC522(handle, CommIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitIRq));

    ClearBitMask(handle, BitFramingReg, 0x80); 			//StartSend=0
    
    if (i != 0)
    {
        uchar ret;
        ret = Read_MFRC522(handle, ErrorReg);
        if(!(ret & 0x1B)) 	//BufferOvfl Collerr CRCErr ProtecolErr
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            { 
                status = MI_NOTAGERR; //?? 
            }
            
            if (command == PCD_TRANSCEIVE)
            {
                n = Read_MFRC522(handle, FIFOLevelReg);
                lastBits = Read_MFRC522(handle, ControlReg) & 0x07;
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
                    backData[i] = Read_MFRC522(handle, FIFODataReg); 
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
uchar MFRC522_Anticoll(GlobalHandle * handle,uchar *serNum)
{
    uchar status;
    uchar i;
    uchar serNumCheck=0;
    uint unLen;
    
    //ClearBitMask(Status2Reg, 0x08); //strSensclear
    //ClearBitMask(CollReg,0x80); //ValuesAfterColl
    Write_MFRC522(handle, BitFramingReg, 0x00); //TxLastBists = BitFramingReg[2..0]
 
    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    status = MFRC522_ToCard(handle, PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

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
void CalulateCRC(GlobalHandle * handle, uchar *pIndata, uchar len, uchar *pOutData)
{
    uchar i, n;

    ClearBitMask(handle, DivIrqReg, 0x04); //CRCIrq = 0
    SetBitMask(handle, FIFOLevelReg, 0x80); //清FIFO指针
    //Write_MFRC522(CommandReg, PCD_IDLE);

    //向FIFO中写入数据 
    for (i=0; i<len; i++)
    { 
        Write_MFRC522(handle, FIFODataReg, *(pIndata+i)); 
    }
    Write_MFRC522(handle, CommandReg, PCD_CALCCRC);

    //等待CRC计算完成
    i = 0xFF;
    do 
    {
        n = Read_MFRC522(handle, DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04)); //CRCIrq = 1

    //读取CRC计算结果
    pOutData[0] = Read_MFRC522(handle, CRCResultRegL);
    pOutData[1] = Read_MFRC522(handle, CRCResultRegM);
}



/*
* 函 数 名：MFRC522_Write
* 功能描述：写块数据
* 输入参数：blockAddr--块地址;writeData--向块写16字节数据
* 返 回 值：成功返回MI_OK
*/
uchar MFRC522_Write(GlobalHandle * handle, uchar blockAddr, uchar *writeData)
{
    uchar status;
    uint recvBits;
    uchar i;
    uchar buff[18]; 
    
    buff[0] = PICC_WRITE;
    buff[1] = blockAddr;
    CalulateCRC(handle, buff, 2, &buff[2]);
    status = MFRC522_ToCard(handle, PCD_TRANSCEIVE, buff, 4, buff, &recvBits);

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
        CalulateCRC(handle, buff, 16, &buff[16]);
        status = MFRC522_ToCard(handle, PCD_TRANSCEIVE, buff, 18, buff, &recvBits);
        
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
void MFRC522_Halt(GlobalHandle * handle)
{
    uchar status;
    uint unLen;
    uchar buff[4]; 

    buff[0] = PICC_HALT;
    buff[1] = 0;
    CalulateCRC(handle, buff, 2, &buff[2]);
 
    status = MFRC522_ToCard(handle, PCD_TRANSCEIVE, buff, 4, buff,&unLen);
}

int rfid_dev_search_card(GlobalHandle * handle, uchar * id)
{

    uchar status;

    // 寻卡，返回卡类型 
    status = MFRC522_Request(handle,PICC_REQIDL, id); 
    if (status != MI_OK)
    {
        return -status;
    }
    
    //防冲撞，返回卡的序列号 4字节
    status = MFRC522_Anticoll(handle, id);

    if(status != MI_OK)
        return -status;   

    MFRC522_Halt(handle); //command the card into sleep mode
    return 0;
}
