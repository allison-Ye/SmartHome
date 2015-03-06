#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#include <common.h>
#include <msg.h>
#include <devs.h>
void * proc_set( void * arg)
{
    GlobalHandle * global_handle = (GlobalHandle *)arg;
    int qid;
    if(global_handle == NULL)
    {
        printf("%s:arg is NULL\n",__func__);
    }
    qid = global_handle->qid;
    while(1)
    {
        
        Msgbuf msgbuf;
        int len;
        int size;
        printf("%s\n",__func__);
        memset(&msgbuf, 0, sizeof(msgbuf));
        len = sizeof(msgbuf) - sizeof(long);
        if((size = msgrcv(qid, &msgbuf, len, MSG_TYPE_SERVERSET,0)) < 0)
        {
            perror("Msgrcv");
            printf("%s:Msgrcv error0!\n",__func__);
            continue;
        }
        if(msgbuf.cmd_dst != MSG_TYPE_SERVERSET)
        {
            printf("cmd dst error!\n");
            continue;
        }
        if(msgbuf.cmd_src == 0)
        {
            printf("cmd src error!\n");
            continue;
        }
        if(size != len)
        {
            printf("%s:Msgrcv error1!\n",__func__);
            continue;
        }
        if((msgbuf.offset < 0) || (msgbuf.size < 0))
        {
            printf("%s:Msgrcv error2!\n",__func__);
            continue;
        } 
        if((msgbuf.offset + msgbuf.size) > SHARE_SIZE)
        {
            printf("%s:Msgrcv error3!\n",__func__);
            continue;
        }
        switch(msgbuf.cmd)
        {
#if 0
            case CMD_GET_TEMP:
            {
                float val = 0.0;
                printf("cmd = CMD_GET_TEMP\n");
                sht_get_tempC(global_handle, &val);
                setMsgData(global_handle,msgbuf.offset,msgbuf.size,&val);
                swap_cmd(&msgbuf);
                msgsnd(qid, &msgbuf, len, 0);
                printf("CMD_GET_TEMP over!\n");
                break;
            }
            case CMD_GET_HUMIDITY:
            {
                float val = 0.0;
                printf("cmd = CMD_GET_HUMIDITY\n");
                sht_get_humi(global_handle, &val);
                setMsgData(global_handle,msgbuf.offset,msgbuf.size,&val);
                swap_cmd(&msgbuf);
                msgsnd(qid, &msgbuf, len, 0);
                break;
            }
            case CMD_GET_ALARM:
            {
                int val;
                printf("cmd = CMD_GET_ALARM\n");
                val = global_handle->alarm_status;
                setMsgData(global_handle, msgbuf.offset, msgbuf.size, &val);
                swap_cmd(&msgbuf);
                msgsnd(qid, &msgbuf, len, 0);
                break;
            }
            case CMD_GET_LIGHT:
            {
                float val;
                printf("cmd = CMD_GET_LIGHT\n");
                val = get_lux(global_handle);
                setMsgData(global_handle, msgbuf.offset, msgbuf.size, &val);
                swap_cmd(&msgbuf);
                msgsnd(qid, &msgbuf, len, 0);
                break;
            }
            case CMD_GET_RFID:
            {
                char str[5]={0xff};
                int status;
                printf("cmd = CMD_GET_RFID\n");
                status = rfid_dev_search_card(global_handle,str);
                printf("rfid:0x%x%x%x%x!\n",str[3],str[2],str[1],str[0]);
                setMsgData(global_handle, msgbuf.offset, msgbuf.size,str);
                swap_cmd(&msgbuf);
                msgsnd(qid, &msgbuf, len, 0);
                break;
            }
#endif
            case CMD_SET_LED1_ENABLE:
            {
                int enable;
                printf("cmd = CMD_SET_LED1_ENABLE\n");
                getMsgData(global_handle, msgbuf.offset, msgbuf.size, &enable);
                leds_enable(global_handle, 0, enable);
                printf("enable = %d\n",enable);
                swap_cmd(&msgbuf);
                msgsnd(qid, &msgbuf, len, 0);
                break;
            }
            case CMD_SET_LED2_ENABLE:
            {
                int enable;
                printf("cmd = CMD_SET_LED2_ENABLE\n");
                getMsgData(global_handle, msgbuf.offset, msgbuf.size, &enable);
                leds_enable(global_handle, 1, enable);
                printf("enable = %d\n",enable);
                swap_cmd(&msgbuf);
                msgsnd(qid, &msgbuf, len, 0);
                break;
            }

	    case CMD_SET_MOTOR_DIR:
            {
                int dir;
		printf("cmd = CMD_SET_MOTOR_DIR\n");
                getMsgData(global_handle, msgbuf.offset, msgbuf.size, &dir);
                motor_set_dir(global_handle, dir); 
                printf("dir = %d\n",dir);
                swap_cmd(&msgbuf);
                msgsnd(qid, &msgbuf, len, 0);
                break;
            }   
	    case CMD_SET_MOTOR_STOP:
            {
		printf("cmd = CMD_SET_MOTOR_STOP\n");
                motor_stop(global_handle);
                swap_cmd(&msgbuf);
                msgsnd(qid, &msgbuf, len, 0);
                break;
            }
            case CMD_SET_BEEPER:
            {
                int enable;
                printf("cmd = CMD_SET_BEEPER\n");
                getMsgData(global_handle, msgbuf.offset, msgbuf.size, &enable);
                beeper_on(global_handle, enable);
                printf("beeper enable = %d\n",enable);
                swap_cmd(&msgbuf);
                msgsnd(qid, &msgbuf, len, 0); 
            }   
            default:
                printf("ERROR CMD!\n");
                break;
        }
    }
}
