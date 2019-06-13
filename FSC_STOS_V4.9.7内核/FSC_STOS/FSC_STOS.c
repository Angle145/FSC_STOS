/*--------------------------------------------�汾: V4.9.7--------------------------------------------*/
#include "fsc_stos.h" 
char _FSC_STOS_Version[]={"V4.9.7"};//�汾��

/*---------------------------------------ȫ�ֱ�����������(Page Up)--------------------------------------*/

/*--------------------------------------------ϵͳ��-----------------------------------------*/
#if (OS_SYSTIME_ENABLE== 1) //ʱ�����ú�������
extern u8 RTC_SetTimes(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//����ʱ�亯��������ʱ����(���ⲿ����)
#endif
/*-------------------------------------------------------------------------------------------*/
/*--------------------------------------------�û���-----------------------------------------*/


/*-------------------------------------------------------------------------------------------*/
/*---------------------------------------ȫ�ֱ�����������(Page Down)--------------------------------------*/


/*-------------------------------------�Զ���ϵͳָ����(Page Up)------------------------------------------*/
/*ʹ�÷�����
1.�Զ�������ָ��;
2.ת����ҳCmd_Process()������������Ӧ�Ĵ���(�ڱ�ҳԼ560��)��
*/
/*+++++++++++++++++++ϵͳȱʡָ��+++++++++++++*/

//��ѡ��Cmd_Process�Ҽ�goto Definition ""��ʽ���뵽Cmd_Process()������
#if (OS_CMD_ALL_ENABLE == 1)
#if (OS_CMD_NSY_ENABLE == 1)
/*----------------------------------ȫ��ָ��----------------------------------*/
/*
*ȫ��ָ���κ�ʱ�̶���Ӧ��Ŀǰ��OSֻ��3��ȫ��ָ�
*��ȫ��ָ���⣬������Ϊ����ָ��,�ڽ���ָ��ϵͳ�������Ӧ��
*/
char cmd_help[]={"cmd/help//"};  //ָ�����
char cmd_enter[]={"cmd/enter//"};//����ָ��ϵͳ
char cmd_exit[]={"cmd/exit//"};  //�˳�ָ��ϵͳ
/*--------------------------------------------------------------------------*/

/*----------------------------------ϵͳָ��----------------------------------*/
//1.ϵͳ�޹أ�   �������񳬹�5����������«��ư��������ָ��.���˴���Cmd_Process()�����ﶼ��Ҫ���ӣ���

/*---------------���񿪹�----------------*/
char cmd_task_open_01[]={"Task1/open//"};//����1��  ֻҪ��ָ֤����//��β���ɣ���{"������1//"}
char cmd_task_open_02[]={"Task2/open//"};//����2�� 
char cmd_task_open_03[]={"Task3/open//"};//����3�� 
char cmd_task_open_04[]={"Task4/open//"};//����4�� 
char cmd_task_open_05[]={"Task5/open//"};//����5�� 

char cmd_task_close_01[]={"Task1/close//"};//����1�ر�
char cmd_task_close_02[]={"Task2/close//"};//����2�ر�
char cmd_task_close_03[]={"Task3/close//"};//����3�ر�
char cmd_task_close_04[]={"Task4/close//"};//����4�ر�
char cmd_task_close_05[]={"Task5/close//"};//����5�ر�
/*-------------------------------------------*/
/*-----------------�������ȼ�-----------------*/
char cmd_task_prio_01[]={"Task1/prio=%d//"};//%d��ʾ��������,����cmd/Task1/prio=8//
char cmd_task_prio_02[]={"Task2/prio=%d//"};
char cmd_task_prio_03[]={"Task3/prio=%d//"};
char cmd_task_prio_04[]={"Task4/prio=%d//"};
char cmd_task_prio_05[]={"Task5/prio=%d//"};
/*-------------------------------------------*/
/*---------------����ʱ����Ƭ-----------------*/
char cmd_task_timeslice_01[]={"Task1/timeslice=%d//"}; //��������1ʱ����Ƭʱ��(һ�������������е�ʱ��)
char cmd_task_timeslice_02[]={"Task2/timeslice=%d//"}; //
char cmd_task_timeslice_03[]={"Task3/timeslice=%d//"}; //
char cmd_task_timeslice_04[]={"Task4/timeslice=%d//"}; //
char cmd_task_timeslice_05[]={"Task5/timeslice=%d//"}; //
/*-------------------------------------------*/
#if (OS_SYSTIME_ENABLE ==1 )
/*----------------����ϵͳʱ��----------------*/
char cmd_systime_set[]={"setsystime=%d/%d/%d/%d.%d.%d//"};//��/��/��/ʱ.��.��
/*------------------------------------------*/
#endif
#endif
//2.ϵͳ�йأ�
/*------------------------------------------*/
char cmd_osinformation[]={"osinformation//"};//�鿴ϵͳ״̬
char cmd_cpu_occrate_max_information[]={"cpu_occrate_max_information//"};
char cmd_cpu_user_occrate_max_information[]={"cpu_user_occrate_max_information//"};
char cmd_runmode_order[]={"runmode/order//"};//ʱ��Ƭ����ģʽ
char cmd_runmode_prio[]={"runmode/prio//"};  //���ȼ�����ģʽ
char cmd_runmode_order_prio[]={"runmode/order+prio//"};//ʱ��Ƭ+���ȼ�����ģʽ
char cmd_oson[]={"oson//"};                  //ϵͳ�ر�
char cmd_osoff[]={"osoff//"};                //ϵͳ��
char cmd_hardreset[]={"hardreset//"};        //Ӳ������
char cmd_osoff_delaytime_set[]={"setosofftime=%d/%d/%d/%d.%d.%d//"};//��/��/��/ʱ.��.��  ����ϵͳ�ر�ʱ��
/*------------------------------------------*/

/*+++++++++++++++++++�û��Զ���ָ��+++++++++++++*/
//1.ȫ��ָ�


//2.����ָ� 


//3.�����ָ�����ת��Cmd_Process()�����������Ӧ����
/*--------------------------------------------*/
#endif
/*------------------------------------�Զ���ϵͳָ����(Page Down)-----------------------------------------*/






/*------------------------------------��ֲӲ���й�(Page Up)----------------------------------------------*/
/*1.ϵͳʱ�ӽ���*/
/*Ĭ��ʹ��SysTick��ʱ��   
*SysTick->CTRL: bit0-��ʱ��ʹ�� bit1-�ж�ʹ�� bit2-ʱ��Դѡ��(=1ϵͳ��Ƶ��=0ϵͳ��Ƶ/8)  bit16-�������λ*/

#ifndef SystemCoreClock
#define SystemCoreClock 72000000  //��Ϊ������Ƶ�������úö�Ӧ����Ƶ(�������ļ��Ѷ��������趨��)
#endif
#define OSTIMER_OFF()      SysTick->CTRL&=~(1<<0)     //�ر�ϵͳ��ʱ���ж�
#define OSTIMER_ON()       SysTick->CTRL|=(1<<0)      //��ϵͳ��ʱ���ж�
#define OSTIMER_INT_OFF()  SysTick->CTRL&=~(1<<1)     //�ر�ϵͳ��ʱ���ж�
#define OSTIMER_INT_ON()   SysTick->CTRL|=(1<<1)      //��ϵͳ��ʱ���ж�
#define OSTIMER_CONT_1us   (SystemCoreClock/8/1000000)//ϵͳ��ʱ��1us�������� 
#define OSTIMER_LOAD       SysTick->LOAD              //ϵͳ��ʱ����װ��ֵ�Ĵ���
#define OSTIMER_VAL        SysTick->VAL               //ϵͳ��ʱ����ǰֵ�Ĵ���
void OSSysTickInit(void)//����OS�ļ�ʱ����
{ 
	//Systick��ʱ����ʹ��(ʹ��������ʱ��ʱ�����޸�Ϊ������ʱ��)
	char * Systick_priority = (char *)0xe000ed23;       //Systick�ж����ȼ��Ĵ���
	SysTick->LOAD  = OSTIMER_CONT_1us* 1000;            //Systick��ʱ����װ�ؼ���ֵ:1ms(�̶�ֵ)
	*Systick_priority = 0x00;                           //Systick��ʱ���ж����ȼ�
	SysTick->VAL   = 0;                                 //Systick��ʱ����������0
	SysTick->CTRL = 0x3;//Systick�򿪲�ʹ���ж���ʹ���ⲿ����ʱ��,8��Ƶ  72MHz/8=9MHz  ����9000��=1ms  ����9��=1us
}
void SysTick_Handler(void) //Systick��ʱ���жϺ���(ʹ��������ʱ��ʱ,���޸�Ϊ������ʱ�����жϺ�����)
{
  OS_SysTick_Handler();
}
/*----------------------------------------------------------------------------------*/
/*2.�������*/
/*��ʽһ���������ж�PendSV*/
void OSPendSVInit(void)//PendSV��ʹ��
{
	char* NVIC_SYSPRI14= (char *)0xE000ED22;  //���ȼ��Ĵ�����ַ
	*NVIC_SYSPRI14=0xFF;//����PendSV�ж����ȼ���� 
}
int* NVIC_INT_CTRL= (int *)0xE000ED04;  //�жϿ��ƼĴ�����ַ
void OSPendSVPulse(void)//����PendSV�ж�
{
	OS_INT_ENTER();
  *NVIC_INT_CTRL=0x10000000; 
	OS_INT_EXIT();	
}
/*��ʽ��������ж�SVC*/
/*---------------------------------------------------------------------------------*/
void HardReset(void) //Ӳ������
{
	__disable_fault_irq();
  NVIC_SystemReset();
}
/*-----------------------------------��ֲӲ���й�(Page Down)---------------------------------------------*/






/*--------------------------------------������ˢuCGUI֧��(Page Up)----------------------------------------------*/
/*-------uCGUI֧�ֲ���ϵͳ��Ҫ��ӵĶ���4������-------*/
#if (OS_UCGUI_SUPPORT==1)
#define hMutex 0
void GUI_X_InitOS(void)
{

}
unsigned int GUI_X_GetTaskId(void)
{
return OSTCBRun->TaskNum;
}
void GUI_X_Lock(void)
{
	OSMutexPend(OSMutex_BPN,hMutex, 0);
}
void GUI_X_Unlock(void)
{
  OSMutexPost(hMutex);
}
#endif
/*--------------------------------------------------*/
/*-------------------------------------------uCGUI֧��(Page Down)----------------------------------------------*/




/*-------------------------------------------�ڴ����(Page Up)----------------------------------------------*/
/*
�ڴ������ƣ�
���ڴ����Ϊһ���������飬����Ŀ�ͷ4byte(32bit)*4=16byte���ڼ�¼���������Ϣ,���еڶ����������ڱ�־�������Ƿ���У�����ͼ��
Block:[byte0][byte1][byte2][byte3][byte4]--[byte7][byte8]--[byte11][byte12]--[byte15][byte16]--[byteN-2][byteN-1][byteN]
      {        4byte(32bit)      }{ 4byte(32bit) }{  4byte(32bit) }{  4byte(32bit)  }{  address:(N-16)byte(=struct+1)  }
                 size(N)            1(==1���б�־)        last             next                     data
һ�������ڴ�ʱ�����ڴ�ؿ�ʼ��ַ��ʼ���ҿ����ڴ����飬����ҵ��Ŀ����ڴ�����size������������ڴ�size����ָ���������ڴ��С�ڴ�����(��ǰ�濪ʼ��)��
           �ָ��ȥ���ڴ�ռ�=������Ϣ����+�����ڴ��С��������4*64byte=256byte�ڴ棬��ָ��ȥ���ڴ�=16+256=272������

1.��һ������4*64byte�ڴ棬�ָ��ȥ���ڴ�=16+256=272���õ����ڴ����飺
						[byte0][byte1][byte2][byte3][byte4]--[byte7][byte8]--[byte11][byte12]--[byte15][byte16]         --       [byte271]
						{        4byte(32bit)      }{ 4byte(32bit) }{  4byte(32bit) }{  4byte(32bit)  }{  address:(N-12)byte(=struct+1)  }
												256                0(==0ʹ�ñ�־)       last             next                      data
ʣ���ڴ����飺[byte272]--[byte275][byte276]--[byte279][byte280]--[byte283][byte284]--[byte287][byte288]   --   [byteN-(16+256)]
            {   4byte(32bit)   }{   4byte(32bit)    }{    4byte(32bit) } {  4byte(32bit)  }{  address:(N-12)byte(=struct+1)  }
             size(N-(16+256))       1(==1���б�־)            last               next                      data

2.�ڶ�������4*32byte�ڴ棬�ָ��ȥ���ڴ�=16+128=144���õ����ڴ����飺
						[byte272]--[byte275][byte276]--[byte279][byte280]--[byte283][byte284]--[byte287][byte288]         --       [byte415]
						{   4byte(32bit)    }{   4byte(32bit)   }{    4byte(32bit) }{   4byte(32bit)   }{   address:(N-16)byte(=struct+1)  }
								   256              0(==0ʹ�ñ�־)            last               next                     data
ʣ���ڴ����飺[byte416]--[byte419][byte420]--[byte423][byte424]--[byte427][byte428]--[byte431][byte432]--[byteN-(16+256)-(16+128)]
            {   4byte(32bit)   }{   4byte(32bit)    }{    4byte(32bit)  }{   4byte(32bit)   }{  address:(N-16)byte(=struct+1)  }
       size(N-(16+256)-(16+128))    1(==1���б�־)            last                next                       data
			 
�����ڴ��ͷ�ʱ��ֻ�ǽ����ڴ�����ı�־λ��1(����)����û������Ѹ����������п�������ϲ���һ���ͷ��ڴ���������ڴ���������ڴ���Ƭ��
              OS���ṩר�ŵ��ڴ���Ƭ��������

��������û�ж��ڴ�ؽ������ȷ�������������ڴ����ʱ��������ٶȻ�Խ��Խ���������Ե�Ƭ��С�����ڴ����˵Ӱ�첻������ڴ�رȽϴ�
             ��������ȶ��ڴ�ؽ��з��������������ڴ�ʱ��Ч��ݣ�Ҳ�����ڴ����
*/        
struct memblock{  
    INT32U size;                  //�ڴ������С  
    INT32U free;                  //�Ƿ���ʹ�� 
    struct memblock *last;        //ָ����һ���ڴ�����	
    struct memblock *next;        //ָ����һ���ڴ�����  
};
OSMEM memorypool[OS_MEMORYPOOL_SIZE];
struct memblock *memblockList = (void *)memorypool; 
INT32U FSC_MemoryFreeSizeGet(void)
{
	INT32U addsize=0;
	struct memblock *curr;
	curr=memblockList;
	while(curr!=NULL)
	{
		if(curr->free) addsize+=curr->size;
		curr=curr->next;
	}
	return addsize;
}
void FSC_MemoryInit(void)  
{          
    memblockList->size=OS_MEMORYPOOL_SIZE-sizeof(struct memblock);//�����ڴ�ռ��С  
    memblockList->free=1;                                      //1���ڴ���� 0���ڴ���ʹ�� 
    memblockList->last=NULL;                                   //ָ����ڴ� 	
    memblockList->next=NULL;                                   //ָ����ڴ�  
}

void split(struct memblock *fitting_slot,size_t size)//�ڴ�ָ�(�Ӵ�����ָ��һ������Ϊ���뵽���ڴ棬ʵ���ǰѿ�����Ϣ�ṹ�������ƶ�,�ٽ�ǰ�����Ϣ�ṹ��ı�־��Ϊʹ��)
{
    struct memblock *memnew=(void*)((INT8U*)fitting_slot+size+sizeof(struct memblock));  //���¶�λʣ����еĵ�ַ
    memnew->size=(fitting_slot->size)-size-sizeof(struct memblock);                   //���¶���ʣ�����size��С
    memnew->free=1;                                                        //���ÿ���
    memnew->last=fitting_slot;
   	memnew->next=fitting_slot->next;                                       //���뵽fitting_slot��fitting_slot->next�м�
    fitting_slot->size=size;
    fitting_slot->free=0;
    fitting_slot->next=memnew;                                             //���뵽fitting_slot��fitting_slot->next�м�
}
void *FSC_Malloc(size_t noOfBytes)//�ڴ�����(ÿ���ڴ��Ŀ�ʼ12���ֽ���������ÿ��ڴ�ʹ����Ϣ�Ľṹ������)
{   
	  struct memblock *curr;
    void *result;
    if(!(memblockList->size))
    {
        FSC_MemoryInit();
    }
    curr=memblockList;
    while((((curr->size)<noOfBytes)||((curr->free)==0))&&(curr->next!=NULL))//���ڴ�ؿ�ʼ��ַ���ҿ����ڴ��
    {
        curr=curr->next;
    }
    if((curr->size)==noOfBytes) //ʣ���ڴ�ǡ�����
    {
        curr->free=0;
        result=(void*)(++curr);//����ָ������(data��)��ʼ��ַ
    }
    else if((curr->size)>(noOfBytes+sizeof(struct memblock)))         //ʣ���ڴ����
    {
			  OS_System.MemFragFlag=OS_TRUE;
        split(curr,noOfBytes);                                        //�ָ�������ڴ��ȥ
        result=(void*)(++curr);                                       //����ָ������(data��)��ʼ��ַ
    }
    else  //ʣ���ڴ治��
    {
        result=NULL;
    }
		return result;
}

void FSC_Merge(void)//�ϲ����ڿ����ڴ�����(С�Ż�)
{
	  struct memblock *curr;//*prev;
    curr=memblockList;
	  if(curr!=NULL)
		{
			while((curr->next)!=NULL)
			{ 
					if((curr->free) && (curr->next->free)) 
					{                                     
							curr->size += (curr->next->size)+sizeof(struct memblock);
							curr->next = curr->next->next;
						  curr->next->next->last=curr;
					}
					curr=curr->next;
			}
		}
}
void FSC_Free(void* ptr)//�ͷ��ڴ�(�����βε�ַ������FSC_Malloc�������صĵ�ַ)
{   
    if(((void*)memorypool<=ptr)&&(ptr<=(void*)(memorypool+OS_MEMORYPOOL_SIZE)))//���Ҫ�ͷŵĵ�ַ�Ƿ����ڴ�ط�Χ��
    {
        struct memblock* curr=ptr;
        --curr;   //��data�����˵�������Ϣ��
        curr->free=1;//�ÿ���
        FSC_Merge();//�ϲ����ڿ�������(С�Ż������Ż���Ҫ�ڴ���Ƭ����)
     }
     else return;
}

OS_BOOL FSC_FragCheck(void)
{
	OS_BOOL result=OS_FALSE;
	struct memblock *curr;
	curr=memblockList;
	while(curr->next!=NULL)
	{
		if(curr->free) 
		{
		  if(!(curr->next->free)) 
			{
				result=OS_TRUE;
				break;
			}
		}
		curr=curr->next;
	}
	return result;
}
void FSC_FragMerge(void)//�ڴ���Ƭ����(ֻ���ڿ���������ʹ�ã��������ط����û�ʹϵͳ�����������ʱ�ǳ��ã������㹻����ʱʹ��)
{
	INT64U i,devi,len=0;
	struct memblock *curr=memblockList;
	struct memblock *le=memblockList;
	struct memblock* mbk;
	OSMEM *ptr_start;
	
	FSC_Merge();//�ϲ����ڿ����ڴ��
	
	OS_INT_ENTER();
	OSSchedLock();
	while(curr->next!=NULL) //���ҳ����һ���ڴ��������ڴ��
	{
		if(curr->free)//���ҿ����ڴ��
		{		
			le=memblockList;
			while(le!=curr)//ͳ���ڴ���׵�ַ����ǰ�ڴ����ڴ泤��
			{
				 len+=le->size+sizeof(len);
				 le=le->next;
			}
			len=OS_MEMORYPOOL_SIZE-len;//����ʣ�೤��
			mbk=curr;//�����ڴ����Ϣ
			curr->next->last=curr->last;
			(*curr)=(*(curr->next));		
			ptr_start=(void*)(++curr);
			devi=mbk->size+sizeof(mbk);
			for(i=0;i<len;i++)   //����(�ƶ�)������(��ʱ�ǳ���)
			{
				ptr_start[i]=ptr_start[i+devi];
			}
			le=curr;
			mbk=le;//�����ڴ����Ϣ
			le->next=le->next-devi;
			le=mbk->next;
			while(le!=NULL)
			{
				mbk=le;//�����ڴ����Ϣ
				le=(struct memblock*)((INT8U*)le-devi);
				le->last=le->last-devi;
				if(le->next!=NULL) le->next=le->next-devi;
			  le=mbk->next;
			}
		}
		curr=curr->next;
	}
	OS_System.MemFragFlag=OS_FALSE;
	OSSchedUnlock();
	OS_INT_EXIT();
}
/*-------------------------------------------�ڴ����(Page Down)----------------------------------------------*/





/******************************************ϵͳ�йز���(Page Up)*******************************************/

OSSTK TASK_IDLE_STK[TaskIDLE_StkSize];    //���������ջ
OSSTK TASK_MANAGER_STK[TaskManager_StkSize];//��������������ջ

#if (OS_CMD_ALL_ENABLE == 1) 
OS_CMD OS_Cmd;
#endif
#if (OS_FAULT_ENABLE == 1) 
OS_FAULT OS_Fault;
#endif

OS_SYSTEM OS_System;
OS_TCB OSTCBTbl[OS_MAX_TASKS];    
OS_TCB *OSTCBRun; 
OS_TCB *OSTCBCur; 
OS_TCB *OSTCBNext;

void OSDispMsg(void){}  //�պ���
/*********************************************�йز���(Page Down)**********************************************/	
	
	
	
	
	
/*------------------------------------------ϵͳָ��ģ��(Page Up)-----------------------------------------------*/
#if (OS_CMD_ALL_ENABLE == 1)	
	
OS_Information_Backup_Struct CPUOccRateMax;	//CPUռ�������ʱ����Ϣ
OS_Information_Backup_Struct UserCPUOccRateMax;	//�û�����CPUռ�������ʱ����Ϣ	
	
#if (OS_CMD_NSY_ENABLE == 1)	
#if (OS_CMD_DISP_NSY_ENABLE == 1)
#define OSDisp_task1open()   OSCMDprintf("\r\n Task1 = RUNNUNG\r\n\r\n");
#define OSDisp_task2open()   OSCMDprintf("\r\n Task2 = RUNNUNG\r\n\r\n");
#define OSDisp_task3open()   OSCMDprintf("\r\n Task3 = RUNNUNG\r\n\r\n");
#define OSDisp_task4open()   OSCMDprintf("\r\n Task4 = RUNNUNG\r\n\r\n");
#define OSDisp_task5open()   OSCMDprintf("\r\n Task5 = RUNNUNG\r\n\r\n");
														 
#define OSDisp_task1close()  OSCMDprintf("\r\n Task1 = PAUSING\r\n\r\n");
#define OSDisp_task2close()  OSCMDprintf("\r\n Task2 = PAUSING\r\n\r\n");
#define OSDisp_task3close()  OSCMDprintf("\r\n Task3 = PAUSING\r\n\r\n");
#define OSDisp_task4close()  OSCMDprintf("\r\n Task4 = PAUSING\r\n\r\n");
#define OSDisp_task5close()  OSCMDprintf("\r\n Task5 = PAUSING\r\n\r\n");
	
#define OSTask1PrioSet()     OSCMDprintf("\r\n Task1 Prio Set OK!\r\n\r\n");
#define OSTask2PrioSet()     OSCMDprintf("\r\n Task2 Prio Set OK!\r\n\r\n");
#define OSTask3PrioSet()     OSCMDprintf("\r\n Task3 Prio Set OK!\r\n\r\n");
#define OSTask4PrioSet()     OSCMDprintf("\r\n Task4 Prio Set OK!\r\n\r\n");
#define OSTask5PrioSet()     OSCMDprintf("\r\n Task5 Prio Set OK!\r\n\r\n");
	
#define OSDisp_Task1TimeSlice()	 OSCMDprintf("\r\n -----   %s TimeSlice = %dms   -----\r\n\r\n",OSTCBTbl[2].TaskStr,OSTCBTbl[2].TaskTimeSlice);
#define OSDisp_Task2TimeSlice()	 OSCMDprintf("\r\n -----   %s TimeSlice = %dms   -----\r\n\r\n",OSTCBTbl[3].TaskStr,OSTCBTbl[3].TaskTimeSlice);
#define OSDisp_Task3TimeSlice()	 OSCMDprintf("\r\n -----   %s TimeSlice = %dms   -----\r\n\r\n",OSTCBTbl[4].TaskStr,OSTCBTbl[4].TaskTimeSlice);
#define OSDisp_Task4TimeSlice()	 OSCMDprintf("\r\n -----   %s TimeSlice = %dms   -----\r\n\r\n",OSTCBTbl[5].TaskStr,OSTCBTbl[5].TaskTimeSlice);
#define OSDisp_Task5TimeSlice()	 OSCMDprintf("\r\n -----   %s TimeSlice = %dms   -----\r\n\r\n",OSTCBTbl[6].TaskStr,OSTCBTbl[6].TaskTimeSlice);

#define OSDisp_systimeset()	     OSCMDprintf("\r\n SetSysTime OK!\r\n\r\n");
#else 
#define OSDisp_task1open()      OSDispMsg();
#define OSDisp_task2open()      OSDispMsg(); 
#define OSDisp_task3open()      OSDispMsg(); 
#define OSDisp_task4open()      OSDispMsg(); 
#define OSDisp_task5open()      OSDispMsg(); 
														    
#define OSDisp_task1close()     OSDispMsg();
#define OSDisp_task2close()     OSDispMsg();
#define OSDisp_task3close()     OSDispMsg();
#define OSDisp_task4close()     OSDispMsg();
#define OSDisp_task5close()     OSDispMsg();
														    
#define OSTask1PrioSet()        OSDispMsg();
#define OSTask2PrioSet()        OSDispMsg();
#define OSTask3PrioSet()        OSDispMsg();
#define OSTask4PrioSet()        OSDispMsg();
#define OSTask5PrioSet()        OSDispMsg();

#define OSDisp_Task1TimeSlice() OSDispMsg();
#define OSDisp_Task2TimeSlice() OSDispMsg();
#define OSDisp_Task3TimeSlice() OSDispMsg();
#define OSDisp_Task4TimeSlice() OSDispMsg();
#define OSDisp_Task5TimeSlice() OSDispMsg();

#define OSDisp_systimeset()     OSDispMsg();
#endif
#endif

#if (OS_CMD_DISP_SYS_ENABLE == 1)
#define OSDisp_runmode_order()       OSCMDprintf("\r\n -----$$ OS RUNNING MODE : ORDER $$-----\r\n\r\n");
#define OSDisp_runmode_prio()        OSCMDprintf("\r\n -----$$ OS RUNNING MODE : PRIO  $$-----\r\n\r\n");
#define OSDisp_runmode_order_prio()  OSCMDprintf("\r\n -----$$ OS RUNNING MODE : ORDER+PRIO  $$-----\r\n\r\n");
#define OSDisp_oson()                OSCMDprintf("\r\n -----$$ OS AWAKEN  $$-----\r\n\r\n");
#define OSDisp_osoff()               OSCMDprintf("\r\n -----$$ OS SLEEP  $$-----\r\n\r\n");
#define OSDisp_hardreset()           OSCMDprintf("\r\n -----$$ HardReset $$-----\r\n\r\n");
#define OSDisp_osoff_delaytime()     OSCMDprintf("\r\n OS ShutDown Time: %d��%d��%d��%dʱ%d��%d��\r\n\r\n",\
																																			 OS_System.ShutDownTime_year,\
																																			 OS_System.ShutDownTime_mouth,\
																																			 OS_System.ShutDownTime_day,\
																																			 OS_System.ShutDownTime_hour,\
																																			 OS_System.ShutDownTime_minute,\
																																			 OS_System.ShutDownTime_second);
#else 
#define OSDisp_runmode_order()      OSDispMsg();
#define OSDisp_runmode_prio()       OSDispMsg();
#define OSDisp_oson()               OSDispMsg();
#define OSDisp_osoff()              OSDispMsg();
#define OSDisp_runmode_order_prio() OSDispMsg();
#define OSDisp_hardreset()          OSDispMsg();
#define OSDisp_osoff_delaytime()    OSDispMsg();
#endif
void OS_CmdHelp_Process(void)//ָ�������ʾ����(�û�������)
{
	OSCMDprintf("=====================================================\r\n");
  OSCMDprintf("------------------FSC Cmd Help System----------------\r\n");
	OSCMDprintf("%s            ָ�����\r\n",cmd_help);
	OSCMDprintf("%s           ��ָ��ϵͳ\r\n",cmd_enter);
	OSCMDprintf("%s            �ر�ָ��ϵͳ\r\n",cmd_exit);
	OSCMDprintf("----------------------ָ��ϵͳָ��-------------------\r\n");	
	OSCMDprintf("*ϵͳ�й�ָ�\r\n");
	OSCMDprintf("%s       ϵͳ״̬��Ϣ\r\n",cmd_osinformation);
	OSCMDprintf("%s       ˳������ģʽ\r\n",cmd_runmode_order);
	OSCMDprintf("%s        ���ȼ�����ģʽ\r\n",cmd_runmode_prio);
	OSCMDprintf("%s  �������ģʽ\r\n",cmd_runmode_order_prio);
	OSCMDprintf("%s                ϵͳ��\r\n",cmd_oson);
	OSCMDprintf("%s               ϵͳ�ر�\r\n",cmd_osoff);
	OSCMDprintf("%s           Ӳ������\r\n",cmd_hardreset);
  OSCMDprintf("%s ����ϵͳ�رյ���ʱʱ��(��/��/��/ʱ.��.��)\r\n",cmd_osoff_delaytime_set);
	OSCMDprintf("-----------------------------------------------------\r\n");
	OSCMDprintf("*ϵͳ�޹�ָ�\r\n");
	OSCMDprintf("������/open//         �����\r\n");
	OSCMDprintf("������/close//        ����ر�\r\n");
	OSCMDprintf("������/prio=%%d//      �������ȼ�����Ϊ%%d(��������)\r\n");
	OSCMDprintf("������/timeslice=%%d// ����ʱ��Ƭ����Ϊ%%d(��������)\r\n");
	OSCMDprintf("=====================================================\r\n");
}
void Cmd_RxBuff_Clean(void)//ָ������
{
  INT8U i;
	for(i=0;i<strlen(OS_Cmd.RX_BUFF);i++)
	{
	  OS_Cmd.RX_BUFF[i]=0;
	}
}
void Cmd_Process(void)//ϵͳָ�����  (�û����ã��ڴ˺��������Զ���ָ��)
{
/*
ʹ�÷�����
1.�������ֲ�����ָ� if(CompareCmd(�Զ���ָ��������)==0) {  ��Ӧ�Ĺ��ܴ������  }	
2.�����ֲ�����ָ�� ��  if(CompareCmdDig('=',�Զ���ָ��������)==0) {  ��Ӧ�Ĺ��ܴ������  + ����=GrabCmdDig(n)  }
  ˵����
       '='�ź���Ϊ���ֲ������Զ���ָ���б�������ַ�'='(Ҳ��Ϊ��������)  	
	     GrabCmdDig(n)��������ָ���еĵ�n�����֣�ֻҪ�����ֶ����ϡ�
	     �� char cmd_lde0_time[]={"cmd/led0/time=%d//"};
	        if(CompareCmdDig('=',cmd_lde0_time)==0) { led0=1; delay(GrabCmdDig(1)); led=0; }
	        ���� cmd/led0/time=20//ʱ��  GrabCmdDig(0)==0��GrabCmdDig(1)==20�����ԣ�������Ҫ����GrabCmdDig(1)��
*/
		if(OS_Cmd.RXOK_Flag==OS_TRUE)
		{	
			OSCMDprintf("%s\r\n",OS_Cmd.RX_BUFF);
			//ϵͳȫ��ָ��:
			if(CompareCmd(cmd_help)==0)		{ OS_CmdHelp_Process(); }			
			if(CompareCmd(cmd_enter)==0)  { OS_Cmd.Enter_Flag=OS_TRUE; }		
      if(CompareCmd(cmd_exit)==0)   { OS_Cmd.Enter_Flag=OS_FALSE; }	
			/**************************************�û��Զ���ȫ��ָ����***********************************/
			//�û�ȫ��ָ��: 
			

			/*******************************************************************************************/			
      if(OS_Cmd.Enter_Flag==OS_TRUE)
			{						
	#if (OS_CMD_NSY_ENABLE == 1)
				/**************************************�û��Զ���ָ����*************************************/
				//�û�����ָ��:
							

				/*****************************************************************************************/
				
				//ϵͳ����ָ��:
				/********************************************�����****************************************/	//����1��ӦTCB[2]		
				if(CompareCmd(cmd_task_open_01)==0) { OSTaskStateSet((void*)OSTCBTbl[2].TaskAdd,TASK_RUNNING);OSDisp_task1open(); }                     
				if(CompareCmd(cmd_task_open_02)==0) { OSTaskStateSet((void*)OSTCBTbl[3].TaskAdd,TASK_RUNNING);OSDisp_task2open(); }																		
				if(CompareCmd(cmd_task_open_03)==0) { OSTaskStateSet((void*)OSTCBTbl[4].TaskAdd,TASK_RUNNING);OSDisp_task3open(); }                                    
				if(CompareCmd(cmd_task_open_04)==0) { OSTaskStateSet((void*)OSTCBTbl[5].TaskAdd,TASK_RUNNING);OSDisp_task4open(); }	                                    
				if(CompareCmd(cmd_task_open_05)==0) { OSTaskStateSet((void*)OSTCBTbl[6].TaskAdd,TASK_RUNNING);OSDisp_task5open(); }			                                  
				//�����񳬹�5������������
				
				/*******************************************************************************************/	
				/********************************************����ر�****************************************/	
				if(CompareCmd(cmd_task_close_01)==0) { OSTaskStateSet((void*)OSTCBTbl[2].TaskAdd,TASK_PAUSING);OSDisp_task1close(); }
				if(CompareCmd(cmd_task_close_02)==0) { OSTaskStateSet((void*)OSTCBTbl[3].TaskAdd,TASK_PAUSING);OSDisp_task2close(); }
				if(CompareCmd(cmd_task_close_03)==0) { OSTaskStateSet((void*)OSTCBTbl[4].TaskAdd,TASK_PAUSING);OSDisp_task3close(); }
				if(CompareCmd(cmd_task_close_04)==0) { OSTaskStateSet((void*)OSTCBTbl[5].TaskAdd,TASK_PAUSING);OSDisp_task4close(); }
				if(CompareCmd(cmd_task_close_05)==0) { OSTaskStateSet((void*)OSTCBTbl[6].TaskAdd,TASK_PAUSING);OSDisp_task5close(); }
				//�����񳬹�5������������
				
				/*******************************************************************************************/		
				/*******************************************�������ȼ�***************************************/	
				if(CompareCmdDig('=',cmd_task_prio_01)==0) { OSTaskPrioSet((void*)OSTCBTbl[2].TaskAdd,GrabCmdDig(1));OSTask1PrioSet(); }	                                                                            
				if(CompareCmdDig('=',cmd_task_prio_02)==0) { OSTaskPrioSet((void*)OSTCBTbl[3].TaskAdd,GrabCmdDig(1));OSTask2PrioSet(); }		                                       
				if(CompareCmdDig('=',cmd_task_prio_03)==0) { OSTaskPrioSet((void*)OSTCBTbl[4].TaskAdd,GrabCmdDig(1));OSTask3PrioSet(); }	                                       
				if(CompareCmdDig('=',cmd_task_prio_04)==0) { OSTaskPrioSet((void*)OSTCBTbl[5].TaskAdd,GrabCmdDig(1));OSTask4PrioSet(); }	                                      
				if(CompareCmdDig('=',cmd_task_prio_05)==0) { OSTaskPrioSet((void*)OSTCBTbl[6].TaskAdd,GrabCmdDig(1));OSTask5PrioSet(); }	                                       
				//�����񳬹�5������������
				/*******************************************************************************************/		
				/*******************************************����ʱ��Ƭ***************************************/	
				if(CompareCmdDig('=',cmd_task_timeslice_01)==0){ OSTCBTbl[2].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task1TimeSlice();}
				if(CompareCmdDig('=',cmd_task_timeslice_02)==0){ OSTCBTbl[3].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task2TimeSlice();}
				if(CompareCmdDig('=',cmd_task_timeslice_03)==0){ OSTCBTbl[4].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task3TimeSlice();}
				if(CompareCmdDig('=',cmd_task_timeslice_04)==0){ OSTCBTbl[5].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task4TimeSlice();}
				if(CompareCmdDig('=',cmd_task_timeslice_05)==0){ OSTCBTbl[6].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task5TimeSlice();}
				//�����񳬹�5������������
	#endif	
				/*******************************************************************************************/
	#if (OS_SYSTIME_ENABLE== 1)			
				if(CompareCmdDig('=',cmd_systime_set)==0) { RTC_SetTimes(GrabCmdDig(0),\
																														GrabCmdDig(1),\
																														GrabCmdDig(2),\
																														GrabCmdDig(3),\
																														GrabCmdDig(4),\
																														GrabCmdDig(5)
																													 );
																														OSDisp_systimeset(); }		
	#endif			
				/*******************************************************************************************/	
	#if (OS_TIMECOUNTSYS_ENABLE == 1)			
				if(CompareCmdDig('=',cmd_osoff_delaytime_set)==0) { OS_ShutDown_DelayTime_Set_YMDHMS(\
																														GrabCmdDig(0),\
																														GrabCmdDig(1),\
																														GrabCmdDig(2),\
																														GrabCmdDig(3),\
																														GrabCmdDig(4),\
																														GrabCmdDig(5)
																													 );
																														OSDisp_osoff_delaytime(); }		
	#endif																													
				/*************************************ϵͳ�й�ָ��*******************************************/	
				if(CompareCmd(cmd_osinformation)==0) { OS_Information_Process();  }		
        if(CompareCmd(cmd_cpu_occrate_max_information)==0) { OS_Information_Backup_Process(&CPUOccRateMax);}	
        if(CompareCmd(cmd_cpu_user_occrate_max_information)==0) { OS_Information_Backup_Process(&UserCPUOccRateMax);}					
				if(CompareCmd(cmd_runmode_order)==0) { OS_System.RuningMode=0; OSDisp_runmode_order(); }
				if(CompareCmd(cmd_runmode_prio)==0)  { OS_System.RuningMode=1; OSDisp_runmode_prio();  }
				if(CompareCmd(cmd_runmode_order_prio)==0){ OS_System.RuningMode=2; OSDisp_runmode_order_prio();  }			
				if(CompareCmd(cmd_oson)==0)          { OSAwaken();  OSDisp_oson(); }
				if(CompareCmd(cmd_osoff)==0)         { OSDisp_osoff();  OSSleep(); }
				if(CompareCmd(cmd_hardreset)==0)     { OSDisp_hardreset();  HardReset(); }
				/*******************************************************************************************/	
				Cmd_RxBuff_Clean();
				OSCMDprintf("cmd->");
			}		
	  }
    OS_Cmd.RX_COUNT=0;
    OS_Cmd.RXOK_Flag=OS_FALSE;		 	
}	
void OS_Information_Backup(OS_Information_Backup_Struct* Struct)//����ϵͳ��Ϣ 
{
	INT16U TaskCount,i;
	//����:
	
	//����ģʽ
  Struct->RuningMode=OS_System.RuningMode;
	
	//����ʱ��
	Struct->RunTime[0]=OS_System.RunTime_year;
	Struct->RunTime[1]=OS_System.RunTime_mouth;
	Struct->RunTime[2]=OS_System.RunTime_day;
	Struct->RunTime[3]=OS_System.RunTime_hour;
	Struct->RunTime[4]=OS_System.RunTime_minute;
	Struct->RunTime[5]=OS_System.RunTime_second;
	Struct->RunTime[6]=OS_System.RunTime_ms; 
	
	//ʱ�ӽ���
	Struct->Clock=OSTIMER_LOAD/OSTIMER_CONT_1us/1000;
	
	//ϵͳ�رյ���ʱʱ��
	Struct->ShutDownTime[0]=OS_System.ShutDownTime_year;
	Struct->ShutDownTime[1]=OS_System.ShutDownTime_mouth;
	Struct->ShutDownTime[2]=OS_System.ShutDownTime_day;
	Struct->ShutDownTime[3]=OS_System.ShutDownTime_hour;
	Struct->ShutDownTime[4]=OS_System.ShutDownTime_minute;
	Struct->ShutDownTime[5]=OS_System.ShutDownTime_second;
	if(Struct->ShutDownTime[5]>0) Struct->ShutDownTime[5]-=1;  
	
	//CPUռ����Ϣ  ���α�ʾ�����ͣ�һλС��
	Struct->CPUOccRate[0]=1000-(OSTCBTbl[0].TaskCPUOccRateCnt*1000.0/OS_System.TaskTimeSliceCnt);
	Struct->CPUOccRate[1]=OS_System.TaskCPUOccRateMax;
	Struct->CPUOccRate[2]=OS_System.UserTaskCPUOccRateMax;
	
	//�ڴ���Ϣ
	Struct->Memory[0]=OS_MEMORYPOOL_SIZE;
	Struct->Memory[1]=OS_MEMORYPOOL_SIZE-FSC_MemoryFreeSizeGet();
	Struct->Memory[2]=FSC_MemoryFreeSizeGet();
	
	//������Ϣ
	for(TaskCount = 0; TaskCount < OS_MAX_TASKS; TaskCount++) 
	{
		if(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)
		{	
			Struct->TaskMsg[TaskCount][0]=OSTCBTbl[TaskCount].TaskCPUOccRateCnt*1000/OS_System.TaskTimeSliceCnt;//���α�ʾ�����ͣ�һλС��
			Struct->TaskMsg[TaskCount][1]=OSTCBTbl[TaskCount].TaskCPUOccRateCnt*1000/OS_System.ClockCnt;
			Struct->TaskMsg[TaskCount][2]=OSTCBTbl[TaskCount].StkUsed;
			Struct->TaskMsg[TaskCount][3]=OSTCBTbl[TaskCount].StkFree;
			Struct->TaskMsg[TaskCount][4]=OSTCBTbl[TaskCount].StkUsed*1000/OSTCBTbl[TaskCount].StkSize;//���α�ʾ�����ͣ�һλС��
			Struct->TaskMsg[TaskCount][5]=OSTCBTbl[TaskCount].TaskTimeSlice;
			Struct->TaskMsg[TaskCount][6]=OSTCBTbl[TaskCount].TaskPrio;
			Struct->TaskMsg[TaskCount][7]=OSTCBTbl[TaskCount].TaskState;
			//������
			for(i = 0; i < TASK_NAME_LEN/2; i++) 
			{
			  Struct->TaskStr[TaskCount][i]=OSTCBTbl[TaskCount].TaskStr[i*2];	
				Struct->TaskStr[TaskCount][i]|=OSTCBTbl[TaskCount].TaskStr[i*2+1]<<8;	
			}				
		}
	}	
}
void OS_Information_Backup_Process(OS_Information_Backup_Struct* Struct)//ϵͳ������Ϣ��ʾ
{
#if (OS_CMD_DISP_SYS_ENABLE == 1)		
#endif	
	INT16U TaskCount;
#if (OS_CMD_DISP_SYS_ENABLE == 1)	
	char *pf;
#endif	

#if (OS_CMD_DISP_SYS_ENABLE == 1)
  OSCMDprintf("+++++++++++++++++++++++++++++++++++ϵͳ״̬��Ϣ++++++++++++++++++++++++++++++++++++\r\n");
	OSCMDprintf("===================================================================================\r\n");
	switch(Struct->RuningMode)
	{
	  case 0:pf="Order     "; break;
		case 1:pf="Prio      "; break;
		case 2:pf="Order+Prio";break;
	}
  OSCMDprintf("  ����ģʽ ��%s                     ����ʱ�䣺%d��%d��%d�� %dʱ%d��%d�� %dms\r\n",
	                                           pf,
#if (OS_TIMECOUNTSYS_ENABLE == 1)	
																						 Struct->RunTime[0],
																						 Struct->RunTime[1],
																						 Struct->RunTime[2],
																						 Struct->RunTime[3],
																						 Struct->RunTime[4],
																						 Struct->RunTime[5],
																						 Struct->RunTime[6]); 
#else
                                             0,
																						 0,
																						 0,
																						 0,
																						 0,
																						 0,
	                                           0);
#endif	
	OSCMDprintf("  ʱ�ӽ��� ��%-3dms                          ϵͳ�رյ���ʱ��%d��%d��%d�� %dʱ%d��%d��\r\n",
	                                           Struct->Clock,
#if (OS_TIMECOUNTSYS_ENABLE == 1)
                                             Struct->ShutDownTime[0],
																						 Struct->ShutDownTime[1],
																						 Struct->ShutDownTime[2],
																						 Struct->ShutDownTime[3],
																						 Struct->ShutDownTime[4],
																						 Struct->ShutDownTime[5]); 
#else
                                             0,
																						 0,
																						 0,
																						 0,
																						 0,
																						 0); 
#endif	
	OSCMDprintf("  CPUռ���ʣ�%04.1f%%                          ���ռ���ʣ�ϵͳ %04.1f%%   �û� %04.1f%%\r\r\n",
																															Struct->CPUOccRate[0]/10.0,
																															Struct->CPUOccRate[1]/10.0,
																															Struct->CPUOccRate[2]/10.0);
	OSCMDprintf("  �ڴ����� ��%d Byte\r\r\n",Struct->Memory[0]);      
	OSCMDprintf("  �����ڴ� ��%d Byte                      ʣ���ڴ棺%d Byte \r\r\n",Struct->Memory[1],Struct->Memory[2]);
	OSCMDprintf("-----------------------------------------------------------------------------------\r\n");
	OSCMDprintf("������   ��Ծ��   ʹ��ջ   ����ջ   �ٷֱ�   ʱ��Ƭ    ���ȼ�    ״̬    ������    \r\n");
	OSCMDprintf("  CPU    Active    Used     Free     Per    TimeSlice   Prio    State   Taskname   \r\n\r\n");
	for(TaskCount = 0; TaskCount < OS_MAX_TASKS; TaskCount++) 
	{
		if(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)
		{	
			OSCMDprintf(" %4.1f%%    %-3d      %-6d   %-6d  %4.1f%%      %-5d     %-5d   %d:%d    %s\r\n",
									Struct->TaskMsg[TaskCount][0]/10.0,
			            Struct->TaskMsg[TaskCount][1],
									Struct->TaskMsg[TaskCount][2],
									Struct->TaskMsg[TaskCount][3],
									Struct->TaskMsg[TaskCount][4]/10.0,
									Struct->TaskMsg[TaskCount][5],
			            Struct->TaskMsg[TaskCount][6],
			            Struct->TaskMsg[TaskCount][7]>>4,Struct->TaskMsg[TaskCount][7]&0x0F,
									(char*)(&Struct->TaskStr[TaskCount][0])  );			                 
		}
	}	
	OSCMDprintf("===================================================================================\r\n");
#endif
}

void OS_Information_Process(void)
{
#if (OS_CMD_DISP_SYS_ENABLE == 1)	
	INT16U TaskCount;
#if (OS_TIMECOUNTSYS_ENABLE == 1)	
	INT8U  ShutDownTime_Second=0;
#endif		
#endif	
	
#if (OS_CMD_DISP_SYS_ENABLE == 1)	
	char *pf;
#endif	

	 OS_TaskStackCounter_Process(); //�����ջͳ��
	
#if (OS_CMD_DISP_SYS_ENABLE == 1)
  OSCMDprintf("+++++++++++++++++++++++++++++++++++ϵͳ״̬��Ϣ++++++++++++++++++++++++++++++++++++\r\n");
	OSCMDprintf("===================================================================================\r\n");
	switch(OS_System.RuningMode)
	{
	  case 0:pf="Order     "; break;
		case 1:pf="Prio      "; break;
		case 2:pf="Order+Prio";break;
	}
#if (OS_TIMECOUNTSYS_ENABLE == 1)	
	if(OS_System.ShutDownTime_second>0) ShutDownTime_Second=OS_System.ShutDownTime_second-1;
#endif	
  OSCMDprintf("  ����ģʽ ��%s                     ����ʱ�䣺%d��%d��%d�� %dʱ%d��%d�� %dms\r\n",
	                                           pf,
#if (OS_TIMECOUNTSYS_ENABLE == 1)	
																						 OS_System.RunTime_year,
																						 OS_System.RunTime_mouth,
																						 OS_System.RunTime_day,
																						 OS_System.RunTime_hour,
																						 OS_System.RunTime_minute,
																						 OS_System.RunTime_second,
																						 OS_System.RunTime_ms); 
#else
                                             0,
																						 0,
																						 0,
																						 0,
																						 0,
																						 0,
	                                           0);
#endif	
	OSCMDprintf("  ʱ�ӽ��� ��%-3dms                          ϵͳ�رյ���ʱ��%d��%d��%d�� %dʱ%d��%d��\r\n",
	                                           OSTIMER_LOAD/OSTIMER_CONT_1us/1000,
#if (OS_TIMECOUNTSYS_ENABLE == 1)
                                             OS_System.ShutDownTime_year,
																						 OS_System.ShutDownTime_mouth,
																						 OS_System.ShutDownTime_day,
																						 OS_System.ShutDownTime_hour,
																						 OS_System.ShutDownTime_minute,
																						 ShutDownTime_Second); 
#else
                                             0,
																						 0,
																						 0,
																						 0,
																						 0,
																						 0); 
#endif	
	OSCMDprintf("  CPUռ���ʣ�%04.1f%%                          ���ռ���ʣ�ϵͳ %04.1f%%   �û� %04.1f%%\r\r\n",
	                               100-(OSTCBTbl[0].TaskCPUOccRateCnt*100.0/OS_System.TaskTimeSliceCnt),
								                 OS_System.TaskCPUOccRateMax/10.0,
																 OS_System.UserTaskCPUOccRateMax/10.0);
	OSCMDprintf("  �ڴ����� ��%d Byte\r\r\n",OS_MEMORYPOOL_SIZE);      
	OSCMDprintf("  �����ڴ� ��%d Byte                      ʣ���ڴ棺%d Byte \r\r\n",
	                               OS_MEMORYPOOL_SIZE-FSC_MemoryFreeSizeGet(),FSC_MemoryFreeSizeGet());
	OSCMDprintf("-----------------------------------------------------------------------------------\r\n");
	OSCMDprintf("������   ��Ծ��   ʹ��ջ   ����ջ   �ٷֱ�   ʱ��Ƭ    ���ȼ�    ״̬    ������    \r\n");
	OSCMDprintf("  CPU    Active    Used     Free     Per    TimeSlice   Prio    State   Taskname   \r\n\r\n");
	for(TaskCount = 0; TaskCount < OS_MAX_TASKS; TaskCount++) 
	{
		if(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)
		{	
			OSCMDprintf(" %4.1f%%    %-3d      %-6d   %-6d  %4.1f%%      %-5d     %-5d   %d:%d    %s\r\n",
									OSTCBTbl[TaskCount].TaskCPUOccRateCnt*100.0/OS_System.TaskTimeSliceCnt,
			            OSTCBTbl[TaskCount].TaskCPUOccRateCnt*1000/OS_System.ClockCnt,
									OSTCBTbl[TaskCount].StkUsed,
									OSTCBTbl[TaskCount].StkFree,
									OSTCBTbl[TaskCount].StkUsed*100.0/OSTCBTbl[TaskCount].StkSize,
									OSTCBTbl[TaskCount].TaskTimeSlice,
			            OSTCBTbl[TaskCount].TaskPrio,
			            OSTCBTbl[TaskCount].TaskState>>4,OSTCBTbl[TaskCount].TaskState&0x0F,
									OSTCBTbl[TaskCount].TaskStr  );			                 
		}
	}	
	OSCMDprintf("===================================================================================\r\n");
#endif
}
void OSTaskCPUOccRateCount_Process(void) //����ռ����ͳ�Ƴ���
{
	 INT16U TaskCount;	
	 OSTCBCur->TaskCPUOccRateCnt++;
	 OS_System.TaskTimeSliceCnt++;
	 if(OS_System.ClockCnt==0) OS_System.TaskTimeSliceCnt=0;
	 if(OS_System.TaskTimeSliceCnt==0)
	 {
		 for(TaskCount = 0; TaskCount < OS_MAX_TASKS; TaskCount++)
		 {
			 if(OSTCBTbl[TaskCount].StkPtr != (STK32U*)0)
			 {
				 OSTCBTbl[TaskCount].TaskCPUOccRateCnt=0;
			 }
		 }
		 OS_System.TaskTimeSliceCnt=1;//��1��ʼ����(0����������)		 
	 }  
}
INT32U OS_GetStringLength(char* p)//��ȡ�ַ���p�ĳ���
{
  INT32U i=0;
	while(*p!='\0')
	{
	  i++;
		p++;
	}
	return i;
}
INT8U CompareCmd(char* p)//�Ƚ��ַ���OS_Perip.OS_USART_RX_BUFF���ַ���p�����Ƿ�һ��
{
  return strncmp(OS_Cmd.RX_BUFF,p,OS_GetStringLength(p));
}

INT32U OS_GetStringLengthWithChar(char* p,char ch)//��ȡ�ַ�ch���ַ���p�е�λ��
{
  INT32U i=0;
	while((*p!=ch)&&(*p!='\0'))
	{
	  i++;
		p++;
	}
	return i;
}
INT32* OS_GetStringNum(char* p)//���ַ���p����ȡ���ֱ��浽ȫ�ֱ���CmdNumBuff[]������
{
	INT8U NegFlag=OS_FALSE;
	INT16U i=0;
  for(i=0;i<OS_CMD_NUM_LEN;i++)
	{
	  OS_Cmd.Rx_Dig_Buff[i]=0;
	}
	for(i=0;i<OS_CMD_NUM_LEN;i++)
	{
	  while(((*p<'0')||(*p>'9'))&&(*p!='\0')) p++;
		if(*(p-1)=='-') NegFlag=OS_TRUE;
		while((*p>='0')&&(*p<='9'))
		{
		  OS_Cmd.Rx_Dig_Buff[i]=OS_Cmd.Rx_Dig_Buff[i]*10+(*p-'0');
			p++;
		}
		if(NegFlag==OS_TRUE) OS_Cmd.Rx_Dig_Buff[i]=~OS_Cmd.Rx_Dig_Buff[i]+1;
	}
	return OS_Cmd.Rx_Dig_Buff;
}
INT8U CompareCmdDig(char ch,char* p)//�Ƚ��ַ���OS_Perip.OS_USART_RX_BUFF���ַ���p�У��ַ�ch��ǰ�������Ƿ�һ��
{
  return strncmp(OS_Cmd.RX_BUFF,p,OS_GetStringLengthWithChar(p,ch));
}
INT32 GrabCmdDig(INT8U n)//��ȡ�ַ���p�еĵ�n������(n=0��ʼ)
{
	INT32* pf;
	pf=OS_GetStringNum(OS_Cmd.RX_BUFF);
  return pf[n];
}
#endif

void OSCmdRxByte(INT8U RxByte)
{
#if (OS_CMD_ALL_ENABLE == 1)	
	if(OS_Cmd.RXOK_Flag==OS_FALSE)
	{
		if(OS_Cmd.RX_COUNT<OS_CMD_STR_LEN-1) 
			{
				OS_Cmd.RX_BUFF[OS_Cmd.RX_COUNT++]=RxByte;
				if((OS_Cmd.RX_BUFF[OS_Cmd.RX_COUNT-1]=='/')&&(OS_Cmd.RX_BUFF[OS_Cmd.RX_COUNT-2]=='/'))
				{
				  OS_Cmd.RXOK_Flag=OS_TRUE;	
					OS_Cmd.RX_BUFF[OS_Cmd.RX_COUNT]='\0';
					OSTCBTbl[1].TaskDelayMs=0;
					if(OS_System.RuningMode>0)
					{
						OSSchedSwitch();  //��������
					}
				}
			}
		else { OS_Cmd.RX_COUNT=0; OS_Cmd.RX_BUFF[OS_Cmd.RX_COUNT]=RxByte; }
	}
#else
  RxByte=RxByte;//��ֹ����
#endif	
}
/*------------------------------------------ϵͳָ��ģ��(Page Down)---------------------------------------------*/


/*-------------------------------------------ϵͳ����ģ��(Page Up)----------------------------------------------*/
void OS_StopRun_Process(void)
{	
	 OSTCBCur = &OSTCBTbl[1];//ָ���������������
	 OSTCBCur->TaskDelayMs=0;//ȡ���ȴ�
	 OS_System.IntNesting = 0;//Ϊ�´���������ʹ��׼��
	 OS_System.LockNesting= 0;//Ϊ�´���������ʹ��׼��
	 OSContextExchange();//�������л� 		 
	 OSTIMER_OFF();//�ر�ostimer��ʱ��
}
/*--------------------------------------ϵͳʹ�ܺ���--------------------------------*/
void OSStopRun(void)//ϵͳֹͣ����(ͣ���ڵ�ǰ����)(�������ģʽ����ǰ��������ͬ��main����)
{
	OS_INT_ENTER();
  OSTIMER_OFF();	
	OS_INT_EXIT();
}
void OSStartRun(void)//ϵͳ��ʼ����(����OSģʽ)
{
	OS_INT_ENTER();
  OSTIMER_ON();
	OS_INT_EXIT();
}
void OSSleep(void)//ϵͳ����
{
  OS_INT_ENTER();
  OS_System.Running=OS_FALSE;
	OS_INT_EXIT();
}
void OSAwaken(void)//ϵͳ����
{
  OS_INT_ENTER();
  OS_System.Running=OS_TRUE;
	OSTIMER_ON();
	OS_INT_EXIT();
}
/*-----------------------------------------ϵͳ����ģ��(Page Down)--------------------------------------------*/

/*------------------------------------------������������(Page Up)---------------------------------------------*/
/*******************************************************
*������  ��OSEnterDriver  ��    OSExitDriver
*ʹ�÷�����������������ͷ���� ��  ������������β����
*����    :����OS��������Ӱ��
*ע��    :����������ֻ��ʹ��delay_us()��delay_ms()����ʱ��
         ����֮�ⲻ��ʹ���κ���OS�йص�������Դ
*******************************************************/
void OSEnterDriver(void)//������������ ����
{
	OS_INT_ENTER();
  OSTIMER_INT_OFF();
	OS_INT_EXIT();
}
void OSExitDriver(void)//�뿪�������� ����
{
	OS_INT_ENTER();
  OSTIMER_INT_ON();
	OS_INT_EXIT();
}
/*-----------------------------------------������������(Page Down)--------------------------------------------*/

/*-------------------------------------FSC���ܾ���ϵͳ������Page Up��------------------------------------------*/
#if (OS_FAULT_ENABLE == 1)
void OSCtxSwTo(INT16U tasknum)
{
	OS_INT_ENTER();
	OSTCBCur=&OSTCBTbl[tasknum];
	OSPendSVPulse();
	OS_INT_EXIT();
}
void OSTaskEndlessLoopsCheck_Process(void) //����������ѭ�����,�����������������N������Ϊ������ѭ��
{
  if(OSTCBCur->TaskNum>1)	//���û�����ʼ���
  {
	  if(OS_Fault.runLastTask==OSTCBCur->TaskNum)	
	  {
			OS_System.TaskContinuRunTimeCnt++;		
			if(OS_System.TaskContinuRunTimeCnt>OS_EndlessLoops_Times) 
			{
				OSTCBCur->TaskState=TASK_PAUSING;
				OS_System.LockNesting=0;			 
#if (OS_FAULT_DISP_ENABLE == 1)					 
				OSprintf("FSC��ʾ������%s��������ͣ���У�\r\n",OSTCBCur->TaskStr); 
#endif		 
			}
	  }
	  else
	  {				
	 	  OS_System.TaskContinuRunTimeCnt=0;					 
	 	  OS_Fault.runLastTask=OSTCBCur->TaskNum;
	  }
  }
}
void OS_RESET_DELAY(INT32U div,INT32U time)
{
	OS_System.Running=OS_FALSE;
	OS_Fault.osresetimecnt=time*div;
	while(OS_System.Running==OS_FALSE)	
	 {
		 if(OS_Fault.osresetimecnt%div==0) 
		 {
			 OSprintf("ϵͳ��������ʱ��   %dS\r\n",OS_Fault.osresetimecnt/div);
		 }
	   if(OS_Fault.osresetimecnt>0) OS_Fault.osresetimecnt--;
		 else 
		 {
		   OS_System.Running=OS_TRUE;
			 OSprintf("------------ϵͳ����------------\r\n");
		 }
	 }
	 HardReset();
}
void OS_SHUTDOWN_DELAY(INT32U div,INT32U time)
{
	OS_System.Running=OS_FALSE;
	OS_Fault.osresetimecnt=time*div;
	while(OS_System.Running==OS_FALSE)	
	 {
		 if(OS_Fault.osresetimecnt%div==0) 
		 {
			 OSprintf("ϵͳ�رյ���ʱ��   %dS\r\n",OS_Fault.osresetimecnt/div);
		 }
	   if(OS_Fault.osresetimecnt>0) OS_Fault.osresetimecnt--;
		 else 
		 {
		   OS_System.Running=OS_TRUE;
			 OSprintf("------------ϵͳ�ر�------------\r\n");
		 }
	 }
   while(1);
}
void HardFault_Handler(void)
{
	OSprintf("FSC��ʾ��ϵͳ����,������Ϣ��ϵͳ״̬��\r\n");
	OSprintf("\r\nHardFault_Handler:\r\n");
	OSprintf("{\r\n");
  OSprintf(" %s error! \r\n",OS_Fault.tcb->TaskStr);
	OSprintf(" %s running last! \r\n",OS_Fault.tcblast->TaskStr);
	OSprintf(" %s running next! \r\n",OS_Fault.tcbnext->TaskStr);
	OSprintf("}\r\n");
	OS_Information_Process();
	OSprintf("\r\n\r\n");
#if (OSFAULT_Reset_Enable	== 1)
	OSprintf("-----------ϵͳ����%d�����������----------- \r\n\r\n",OS_Reset_Delay_Time);
  OS_RESET_DELAY(1000000,OS_Reset_Delay_Time);
#else
	OSprintf("-----------ϵͳ����%d���ر�----------- \r\n\r\n",OS_Reset_Delay_Time);
	OS_SHUTDOWN_DELAY(1000000,OS_Reset_Delay_Time);
#endif
}

INT8U OS_FAULT_STK_CHECK(OS_TCB* tcb)//��ջ������
{
	if(tcb->StkPtr!=(STK32U*)0)
	{
		if(tcb->StkAdd[0]!=0)
		{
			tcb->TaskState=TASK_PAUSING;
#if (OS_FAULT_DISP_ENABLE	==1)		
			OSprintf("FSC��ʾ������%s��ջ�������ͣ���У�\r\n\r\n",tcb->TaskStr);
#endif			
			return OS_TRUE ;
		}
		else return OS_FALSE ;
	}
	else return OS_FALSE ;
}
void FSC_FAULT_SYSTEM(void)
{
	INT16U TaskCount;
	for(TaskCount=0;TaskCount < OS_MAX_TASKS; TaskCount++)
	{
		OS_FAULT_STK_CHECK(&OSTCBTbl[TaskCount]);
	}
}	
void OSTaskPrioBuckup(OS_TCB* tcb) //�������ȼ�����
{
  tcb->TaskPrioBackup=tcb->TaskPrio;
}
void OSTaskPrioRecover(OS_TCB* tcb)  //�������ȼ��ӱ����лָ�
{
  tcb->TaskPrio=tcb->TaskPrioBackup;
}
void FSC_FAULT_SYSTEM_OS_Timer_Handler(INT16U tasknum)
{
	INT16U result;
	if(((tasknum>=2)&&(OSTCBTbl[tasknum].TaskFlagBit&(1<<_BIT_TimeLess))==0)&&\
     (OSTCBTbl[OS_System.TaskNext].StkPtr!=(STK32U*)0)&&(OSTCBTbl[tasknum].TaskState==TASK_RUNNING))//������2��ʼ���
	{		
		result= (OSTCBTbl[tasknum].TaskCPUOccRateCnt*100.0/OS_System.TaskTimeSliceCnt)*10;//ȡһλС��
		if(result>900) //ϵͳռ����(ʵ�⣺����������ʱռ�������Ϊ98.1%)
		 {	
			 if(result<=980) 
			 { 
#if (OS_FAULT_DISP_ENABLE == 1)	
					OSprintf("FSC��ʾ������%s CPUռ���ʹ��ߣ�ռ����:%0.1f%%\r\n",OSTCBTbl[tasknum].TaskStr,result/10.0);
#endif					 
			 }
			 else  
			 {
				 OS_Fault.task[tasknum]=OS_TRUE;	 	 
				 if(OS_System.RuningMode==0)
				 {
					 /*----------------�д�����-------------------*/				 
				 }	
				 else if(OS_System.RuningMode==1)		
				 {
					 if(OSTCBTbl[tasknum].TaskPrio==0)
					 {
			       /*----------------�д�����-------------------*/  
					 }
					 else
					 {
						 OSTCBTbl[tasknum].TaskPrio--;//��������ȼ�
#if (OS_FAULT_DISP_ENABLE == 1)					 
						 OSprintf("FSC��ʾ������%s�������𼶽������ȼ�,��ǰ���ȼ�:%d\r\n",OSTCBTbl[tasknum].TaskStr,OSTCBTbl[tasknum].TaskPrio); 
#endif						 
					 }
				 }					 
			 }		 
		 }
		 else if(result<1) //ϵͳռ����<0.1%
		 {		
			 if(result>0)
			 {
#if (OS_FAULT_DISP_ENABLE == 1)				 
				 OSprintf("FSC��ʾ������%s CPUռ���ʹ��ͣ�ռ����:%0.1f%%\r\n",OSTCBTbl[tasknum].TaskStr,result/10.0);
#endif			 
			 }
			 else
			 {
				 OS_Fault.task[tasknum]=OS_TRUE;			 
				 if(OS_System.RuningMode==0)
				 {
           /*----------------�д�����-------------------*/
				 }
				 else if(OS_System.RuningMode==1)
				 {
					 if(OSTCBTbl[tasknum].TaskPrio==OSRunStateHighestPrioTaskNumGet())
					 {
					   /*----------------�д�����-------------------*/
					 }
					 else
					 {
						 if(OSTCBTbl[tasknum].TaskPrio<OS_MAX_TASKS-1) OSTCBTbl[tasknum].TaskPrio++;//��������ȼ�
#if (OS_FAULT_DISP_ENABLE == 1)					 
						 OSprintf("FSC��ʾ������%sδ���У����������ȼ�,��ǰ���ȼ�:%d\r\n",OSTCBTbl[tasknum].TaskStr,OSTCBTbl[tasknum].TaskPrio); 
#endif			 
					 }
				 }
			 }
		 }
		 else //������Χ
		 {
			 if(OS_Fault.task[tasknum]==OS_TRUE)
			 {
					OS_Fault.task[tasknum]=OS_FALSE;
				  OSTaskPrioRecover(&OSTCBTbl[tasknum]);
					OSTCBTbl[tasknum].TaskState=TASK_RUNNING;
#if (OS_FAULT_DISP_ENABLE == 1)	 
				 OSprintf("FSC��ʾ������%s�������У��ѻָ����ȼ�,��ǰ���ȼ�:%d\r\n",OSTCBTbl[tasknum].TaskStr,OSTCBTbl[tasknum].TaskPrio); 
#endif 
			 }
		 }
	 }
}

#endif
/*-----------------------------------FSC���ܾ���ϵͳ������Page Down��------------------------------------------*/






/*----------------------------------------ϵͳ������Page Up)--------------------------------------------------*/

//ֻҪ����PendSV�ж�,��ת�жϴ�����ǰ xPSR,PC,LR,R12,R3-R0���Զ����浽ϵͳ������ջ��(�˲�����PendSV�ж�Ӳ������)��
//�������ĸ�ջȡ���ڵ�ǰ��SP���ͣ�����MSP�򱣴浽ϵͳջ������PSP�򱣴浽����ջ����OS�Ǳ���������ջ����R11-R4��Ҫ�ֶ����浽����ջ��
//��ջ˳��ջ��->ջβ xPSR,PC,LR,R12,R3-R0,R11-R4��
STK32U* OSTaskStkInit(void (*task),STK32U *p)
{
    STK32U *stk;
    stk = p;
#if (__FPU_PRESENT == 1)&&(__FPU_USED == 1) 
        stk  -= 17;
#endif
#if (__FPU_PRESENT == 1)&&(__FPU_USED == 1) 
    *(--stk)  = (INT32U)0x01000000L;   // xPSR   
#else
    *(  stk)  = (INT32U)0x01000000L;   // xPSR bit[24]������1�������ϵ��ֱ�ӽ���Fault�ж�  
#endif
    *(--stk)  = (INT32U)task;          // R15 (PC)             
    *(--stk)  = (INT32U)0xFFFFFFFEL;   // R14 (LR)��ʹ��Ϊ���4λΪE����һ���Ƿ�ֵ����ҪĿ���ǲ���ʹ��R14���������ǲ��ܷ��ص�         
        stk  -= 4;                     //(R12��R3-R1)
	  *(--stk)  = (INT32U)0;             // R0(����ջ��ʹ��R0������0,SP��R0�ָ�)
#if (__FPU_PRESENT == 1)&&(__FPU_USED == 1) 
        stk  -= 16;
#endif    
	    stk  -=  8;                      // R11-R4
    return stk;
}

void OSInit(void) //ϵͳ��ʹ������ʹ�����������������ƿ�����Ĳ���
{ 
    INT32U i;	
#if (OS_TIM_SYS_ENABLE == 1)	
	  OS_System.TIMER_APPLY_COUNT=0;
	  for(i = 0; i < TIMER_SIZE; i++) {
	      OS_System.TIMER[i]=0;
			  OS_System.TIMER_RELOAD[i]=0;
		}
#endif	
#if (OS_SIGN_PP_ENABLE == 1)
		INT16U j;
		OS_System.FLAG_APPLY_COUNT=0;
		OS_System.FLAGGROUP_APPLY_COUNT=0;
		OS_System.MUTEX_APPLY_COUNT=0;
		OS_System.MBOX_APPLY_COUNT=0;
		
		for(i = 0; i < FLAG_SIZE; i++) {
			  OS_System.POST_FLAG_COUNT[i]=0;	
			  for(j = 0; j < OS_MAX_TASKS; j++) {
					  OS_System.FLAG[i][j]=OS_FALSE;	   				
				}
		}
    for(i = 0; i < FLAG_GROUP_SIZE; i++) {
			  OS_System.FlagGroupNameInTask[i]=0;
			  OS_System.ReturnFlagGroupTable[i]=OS_FALSE;
			  OS_System.POST_FGROUP_COUNT[i]=0;
			  for(j = 0; j < FLAG_SIZE; j++) {		
		      OS_System.FlagGroupTable[i][j]=OS_FALSE;
				}
		}
		for(i = 0; i < MUTEX_SIZE; i++) {
			  OS_System.MUTEX[i]=OS_FALSE;
			  OS_System.MutexTaskNum[i]=0;  		 
		}		
		for(i = 0; i < MBOX_SIZE; i++) {
		    OS_System.MBOX[i]=(void*)0;
			  OS_System.POST_MBOX_COUNT[i]=0;
		}	
#endif			
    for(i = 0; i < OS_MAX_TASKS; i++) { 
        OSTCBTbl[i].StkPtr = (STK32U*)0; 
#if (OS_CMD_ALL_ENABLE == 1) 	
			  OSTCBTbl[i].StkSize=0;		
			  OSTCBTbl[i].StkUsed=0;
			  OSTCBTbl[i].StkFree=0;
			  OSTCBTbl[i].TaskCPUOccRateCnt=0; 
			  OSTCBTbl[i].TaskStr[0]='\0';
#endif		   			
			  OSTCBTbl[i].TaskAdd=0;
			  OSTCBTbl[i].TaskNum=0;
			  OSTCBTbl[i].TaskPrio=0;
			  OSTCBTbl[i].TaskDelayMs=0;
        OSTCBTbl[i].TaskState = TASK_CREATING;
    }
		OS_System.RuningMode=OS_RINNING_MODE;
		OS_System.Error=OS_FALSE;
#if (OS_CMD_ALL_ENABLE== 1)	
    OS_Cmd.Enter_Flag=OS_FALSE;
		OS_Cmd.RXOK_Flag=OS_FALSE;
		OS_System.TaskCPUOccRateMax=0;
		OS_System.UserTaskCPUOccRateMax=0;
		OS_System.TaskTimeSliceCnt=1; //��1��ʼ����(0����������)
		OS_Cmd.RX_COUNT=0;
#endif
#if (OS_GET_SET_SW_ENABLE == 1)			
		OS_System.TaskSwitchBackNum=0;
#endif		
#if (OS_FAULT_ENABLE == 1)			
		OS_Fault.runLastTask=0;
#endif	
#if (OS_TIMECOUNTSYS_ENABLE == 1)		
		OS_ShutDown_DelayTime_Set_YMDHMS(0,0,0,0,0,0);
		OS_System.RunTime_second=0;
		OS_System.RunTime_minute=0;
		OS_System.RunTime_hour=0;
		OS_System.RunTime_day=0;
		OS_System.RunTime_mouth=0;
		OS_System.RunTime_year=0;
#endif		

    OS_System.MemFragFlag=OS_FALSE;
    OS_System.TaskDelayTimeOutFlag=OS_FALSE;
		OS_System.TaskCtxSw=OS_FALSE;
    OS_System.RunDriverCode=OS_FALSE;		
		OS_System.RunOneRound=OS_FALSE;
		OS_System.FirstEnter=OS_TRUE;
		OS_System.TaskNextRunFlag=OS_TRUE;
		OS_System.TaskContinuRunTimeCnt=0;
		OS_System.RunTime_ms=0;
		OS_System.ClockCnt=0;
		OS_System.TimeSliceCnt=0;
		OS_System.RunLastTask=0;//��ʹ��Ϊ����0
		OS_System.TaskHighestPrioLast=0;//��ʹ����������ż�Ϊ����0�����ȼ�
		OS_System.TaskNext=0;    //Next���������ʹ��
		
    OS_System.LockNesting=0;
		OS_System.IntNesting=0;
    OS_System.Running=OS_TRUE;//���б�־
		
		OS_CreatTaskIdle();      //��������0  -��������
    OSCreatTaskManager();    //��������1  -�������������
		
		OSTCBRun = OSTCBCur;     //����TCBָ��Cur
		OSTCBCur = &OSTCBTbl[0]; //������0��ʼ����
		OSTCBNext = &OSTCBTbl[1];//��һ���񸳸�Next
		
		OSScheduler_Process();//����			
}

void OS_CreatTaskIdle(void)//������������(��������)
{ 
#if (OS_CMD_ALL_ENABLE== 1)
    INT8U i;	
	  char taskstr[]={"Task_Idle"};
#endif		
    OSTCBTbl[0].StkPtr = OSTaskStkInit(OS_TaskIdle,(STK32U*)&TASK_IDLE_STK[TaskIDLE_StkSize - 1]);
		OSTCBTbl[0].StkAdd = TASK_IDLE_STK;
    OSTCBTbl[0].TaskState = TASK_RUNNING;
	  OSTCBTbl[0].TaskAdd=(INT32U)OS_TaskIdle;
	  OSTCBTbl[0].TaskPrio=TaskIDLE_Prio;
		OSTCBTbl[0].TaskPrioBackup=OSTCBTbl[0].TaskPrio;
		OSTCBTbl[0].TaskTimeSlice=OS_TIMESLICE_IDLE;
#if (OS_CMD_ALL_ENABLE== 1)		
    OSTCBTbl[0].StkSize=TaskIDLE_StkSize;	
	  for(i=0;i<TASK_NAME_LEN;i++)
		{
			if(*taskstr=='\0') break;
		  OSTCBTbl[0].TaskStr[i]=taskstr[i];
		}
#endif			
}
void OSCreatTaskManager(void)//�����������������(��������)
{
#if (OS_CMD_ALL_ENABLE== 1)	
    INT8U i;	
		char taskstr[]={"Task_Manage"};
#endif			
    OSTCBTbl[1].StkPtr = OSTaskStkInit(OS_TaskManager, (STK32U*)&TASK_MANAGER_STK[TaskManager_StkSize - 1]);
		OSTCBTbl[1].StkAdd = TASK_MANAGER_STK;
    OSTCBTbl[1].TaskState = TASK_RUNNING;
	  OSTCBTbl[1].TaskAdd=(INT32U)OS_TaskManager;
   	OSTCBTbl[1].TaskPrio=TaskManager_Prio;
		OSTCBTbl[1].TaskPrioBackup=OSTCBTbl[1].TaskPrio;
		OSTCBTbl[1].TaskTimeSlice=OS_TIMESLICE_MANAGER;
#if (OS_CMD_ALL_ENABLE== 1)	
    OSTCBTbl[1].StkSize=TaskManager_StkSize;	
		for(i=0;i<TASK_NAME_LEN;i++)
		{
			if(*taskstr=='\0') break;
		  OSTCBTbl[1].TaskStr[i]=taskstr[i];
		}
#endif		
}
void OSTaskCreate(char* taskstr,void (*task),INT32U stksize,INT32U tasktimeslice,INT16U taskprio,TASK_STATE taskstate)//��̬����������
{
	  STK32U *stk;
	  STK32U *stkptr;
    INT16U i;
#if (OS_CMD_ALL_ENABLE== 1)		
	  INT16U j; 
#endif	
	OS_INT_ENTER();
	  stkptr=(STK32U*)FSC_Malloc(4*stksize);
    stk=stkptr+stksize-1;  
	  i=2;         //�������������������������񴴽��û�����
		while(OSTCBTbl[i].StkPtr != (STK32U*)0) {   //���ҿ�TCB
        i++;
    }	 		
		OSTCBTbl[i].StkAdd=stkptr;
		OSTCBTbl[i].StkPtr = OSTaskStkInit(task,stk);
    OSTCBTbl[i].TaskState = taskstate;
		OSTCBTbl[i].TaskAdd=(INT32U)task;
		OSTCBTbl[i].TaskNum=i;
		OSTCBTbl[i].TaskPrio=taskprio;
		OSTCBTbl[i].TaskPrioBackup=OSTCBTbl[i].TaskPrio;
		OSTCBTbl[i].TaskTimeSlice=tasktimeslice;
		OSTCBTbl[i].StkSize=stksize;
#if (OS_CMD_ALL_ENABLE== 1)	
		for(j=0;j<TASK_NAME_LEN;j++)
		{
			if(*taskstr=='\0') break;
		  OSTCBTbl[i].TaskStr[j]=*taskstr++;
		}
#endif	
    OS_INT_EXIT();		
}
void OSTaskCreateStatic(char* taskstr,void (*task),STK32U *stkptr,INT32U stksize,INT32U tasktimeslice,INT16U taskprio,TASK_STATE taskstate)//��̬����������(��������ڴ�����(��ջ)��)
{
    INT16U i;
#if (OS_CMD_ALL_ENABLE== 1)		
	  INT16U j; 
#endif	
    STK32U *stk;
	  stk=(STK32U*)&stkptr[stksize-1];
	  i=2;         //�������������������������񴴽��û�����
		while(OSTCBTbl[i].StkPtr != (STK32U*)0) {   //���ҿ�TCB
        i++;
    }
		OSTCBTbl[i].StkPtr = OSTaskStkInit(task,stk);
		OSTCBTbl[i].StkAdd = stkptr;
    OSTCBTbl[i].TaskState = taskstate;
		OSTCBTbl[i].TaskAdd=(INT32U)task;
		OSTCBTbl[i].TaskNum=i;
		OSTCBTbl[i].TaskPrio=taskprio;
		OSTCBTbl[i].TaskPrioBackup=OSTCBTbl[i].TaskPrio;
		OSTCBTbl[i].TaskTimeSlice=tasktimeslice;
#if (OS_CMD_ALL_ENABLE== 1)	
    OSTCBTbl[i].StkSize=stksize;		
		for(j=0;j<TASK_NAME_LEN;j++)
		{
			if(*taskstr=='\0') break;
		  OSTCBTbl[i].TaskStr[j]=*taskstr++;
		}
#endif		
}
void OSTaskDelete(void (*Taskx))//��̬ɾ��������
{  
	  INT16U i;
	  if(OSTCBCur->TaskAdd==(INT32U)Taskx) 
		{
			OS_INT_ENTER();
		  FSC_Free(OSTCBCur->StkAdd);
			OSTCBCur->StkPtr = (void*)0;
			OSTCBCur->TaskState = TASK_DELETING;
			OSSchedSwitch();
			OS_INT_EXIT();
		}
		else
		{
			for(i = 0; i < OS_MAX_TASKS; i++) {  
			 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx )				 
			 {
				 if((OSTCBTbl[i].StkPtr != (void*)0)&&(OSTCBTbl[i].TaskState != TASK_DELETING))
				 {
					 OS_INT_ENTER();
					 FSC_Free(OSTCBTbl[i].StkAdd);
					 OSTCBTbl[i].StkPtr = (void*)0;
					 OSTCBTbl[i].TaskState = TASK_DELETING;  
					 OS_INT_EXIT();
					 break;
				 }
			 }	
		  }
	  }   
}
void OSTaskResume(void (*Taskx),TASK_STATE taskstate)//��̬�ָ���ɾ����������
{ 
	  STK32U *stkptr;
	  STK32U *stk;
	  INT16U i; 
		for(i = 0; i < OS_MAX_TASKS; i++) {  
		 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
		 {
			 if((OSTCBTbl[i].StkPtr == (void*)0)&&(OSTCBTbl[i].TaskState == TASK_DELETING))
				 {
					 OS_INT_ENTER();
					 stkptr=FSC_Malloc(4*OSTCBTbl[i].StkSize);
					 stk=stkptr+OSTCBTbl[i].StkSize-1;
					 OSTCBTbl[i].StkPtr = OSTaskStkInit((void*)OSTCBTbl[i].TaskAdd,stk);
					 OSTCBTbl[i].StkAdd = stkptr;
					 OSTCBTbl[i].TaskState = taskstate; 
           OS_INT_EXIT();					 
					 break;
				 }
		 }	
		}  
}
/*----------------------------------------ϵͳ������Page Down)-------------------------------------------------*/


/*---------------------------------------ϵͳ״̬������Page Up)------------------------------------------------*/
void OSTaskCPUOccRateCounter_Process(void)
{	
#if (OS_CMD_ALL_ENABLE == 1)	
    INT16U CupOccRate;
    INT16U CupOccRateIdle;	
	  INT16U CupOccRateManage;
		if(OS_System.TaskTimeSliceCnt>OS_StartUpOccRateDlyMs)
		{
			CupOccRateIdle=(INT16U)(OSTCBTbl[0].TaskCPUOccRateCnt*1000/OS_System.TaskTimeSliceCnt);		
			CupOccRateManage=(INT16U)(OSTCBTbl[1].TaskCPUOccRateCnt*1000/OS_System.TaskTimeSliceCnt);
			CupOccRate=1000-CupOccRateIdle;			
			if(CupOccRate>OS_System.TaskCPUOccRateMax)//����CPU���ռ����
			{
				OS_System.TaskCPUOccRateMax=CupOccRate;
				OS_Information_Backup(&CPUOccRateMax);//����һ����Ϣ
			}
			CupOccRate=1000-CupOccRateIdle-CupOccRateManage;			
			if(CupOccRate>OS_System.UserTaskCPUOccRateMax)//�����û�����CPU���ռ����
			{
				OS_System.UserTaskCPUOccRateMax=CupOccRate;
				OS_Information_Backup(&UserCPUOccRateMax);//����һ����Ϣ
			}			
		}
#endif	 
}
/*--------------------------------------ϵͳ״̬������Page Down)-----------------------------------------------*/


/*-------------------------------------�ڴ��Զ���������Page Up)----------------------------------------------*/
void MemoryManage_Process(void)  
{
#if (OS_MEM_IDLE_MERGE_ENABLE == 1)		
	INT16U TaskCount;
	INT8U IdleFlag=OS_TRUE;
	for(TaskCount=1;TaskCount<OS_MAX_TASKS;TaskCount++)
	{
		if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)&&\
				 (OSTCBTbl[TaskCount].TaskDelayMs <= 1)&&((OSTCBTbl[TaskCount].TaskFlagBit&_BIT_TimeLess)==0) )
				{
          if(OSTCBTbl[TaskCount].TaskDelayMs<OS_MEM_IDLE_TIME) 
					{
						IdleFlag=OS_FALSE;
						break;
					}
				}
	}
	if(IdleFlag==OS_TRUE)
	{
		if(OS_System.MemFragFlag==OS_TRUE)
		{
			if(FSC_FragCheck()==OS_TRUE)
			{
				OSprintf("  #ϵͳ����#\r\n");
				OSprintf("��ʼ�ڴ���Ƭ����\r\n");
				OSprintf("---�����Ż�---\r\n");
				FSC_FragMerge();
				OSprintf("***�Ż����***\r\n");
				OSprintf("�����ڴ���Ƭ����\r\n");
			}
		}
	}
#endif	
}
	
/*------------------------------------�ڴ��Զ���������Page Down)---------------------------------------------*/


/*--------------------------------------ϵͳ��������Page Up)------------------------------------------------*/

void OS_TaskIdle(void) //�����������ݺ���(��ֹ����OS_delayMs����)  (���Է�ֹ0���������г���)
{
	OS_TaskStackCounter_Process(); //����һ�������ջͳ�ƺ���
	while(1) //������������ͳ��cpuʹ����
	{
    OSTaskCPUOccRateCounter_Process();//����cpuʹ���ʼ������	
		MemoryManage_Process();           //�ڴ���Ƭ�������	
		if(OS_System.RuningMode==1)//�����ȼ�����ģʽ�����
		{
		  OSSchedSwitch(); //�����л����� 
		}
	}
}
void OS_TaskManager(void) //����������������ݺ���
{	
	INT32U i;
	OSprintf("-@FSC_STOS_%s Inside\r\n",_FSC_STOS_Version);
	OSprintf("OS Cmd System��Please Send: cmd/help//\r\n\r\n");
  while(1) 
	{
		OSSchedLock();
#if (OS_CMD_ALL_ENABLE == 1)		
		Cmd_Process();		
#endif		
#if (OS_FAULT_ENABLE == 1)	
		FSC_FAULT_SYSTEM();
#endif		
		OSSchedUnlock();

		//������ʱ
		if(OS_System.Running==OS_TRUE) 
		{
			OS_delayMs(TaskManager_DelayMs);	
		}
    else if(OS_System.Running==OS_FALSE)
    {
		  i=1000000*1;
		  while(i--);
		}	   	
  }
} 
/*--------------------------------------ϵͳ��������Page Down)----------------------------------------------*/


/*--------------------------------------���������йغ�����Page Up)--------------------------------------------*/

#if (OS_GET_SET_SW_ENABLE == 1)||(OS_CMD_NSY_ENABLE == 1)
void OSLockStateReset(void) //����
{
  OS_System.LockNesting=0;
}
void OSTaskDelayMsSet(void* Taskx,INT32U nms)
{
   INT16U i ;
	 if( OSTCBTbl[0].TaskAdd != (INT32U)Taskx ) //�����������񣬲��ܸ��Ŀ��������״̬
	 {
		 for(i = 0; i < OS_MAX_TASKS; i++) 
			{  
				 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
				 {
					 OSTCBTbl[i].TaskDelayMs=nms;
				 }
			}
	 }
}
OS_BOOL OSTaskStateSet(void* Taskx,TASK_STATE TaskState) //����״̬���ú���(�û��ɵ���) //����OS_FALSE����ʧ�� OS_TRUE���óɹ�
{
	 OS_INT_ENTER();
	 INT16U i ;
#if (OS_SIGN_PP_ENABLE == 1)	
	 INT16U j;
#endif	
	 OS_BOOL  result=OS_FALSE;  
	 if(OSTCBCur->TaskAdd==(INT32U)Taskx) //��������ñ�����ֹͣ�����������л���������������
		{
#if (OS_SIGN_PP_ENABLE == 1)			
			 for(j=0;j<MUTEX_SIZE;j++)//���ҵ�ǰ�����Ƿ���Mutexռ����
			 {
				 if(OS_System.MutexTaskNum[j]==OSTCBCur->TaskNum)
				 {
					 OS_INT_EXIT();  
           return OS_FALSE;
				 } 
			 }
#endif			
			 OSTCBTbl[OSTCBCur->TaskNum].TaskState &= 0xF0;
			 OSTCBTbl[OSTCBCur->TaskNum].TaskState |= TaskState&0x0F; 
			 OSTCBCur = OSTCBNext;	 
			 OSPendSVPulse();
			 result= OS_TRUE;		 
		}
	 else
	 {
		 if( OSTCBTbl[0].TaskAdd != (INT32U)Taskx ) //�����������񣬲��ܸ��Ŀ��������״̬
		 {
			 for(i = 0; i < OS_MAX_TASKS; i++) 
				{  
					 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
					 {
#if (OS_SIGN_PP_ENABLE == 1)							 
						 do
						 {						 
							 for(j=0;j<MUTEX_SIZE;j++) //��������Ŀ�������Ƿ���Mutexռ����
							 {
								 if(OS_System.MutexTaskNum[j]==OSTCBTbl[i].TaskNum)
								 { 
									 OS_INT_EXIT();
									 OSPendSVPulse();//�������л���ȥ�ȴ�Ŀ�������ͷ�Mutex
									 OS_INT_ENTER(); 
									 if(OS_System.MutexTaskNum[j]==OSTCBTbl[i].TaskNum)//����ȷ�� 
								   {						  
									   j--; //���û���ͷ������µȴ�
								   } 
									 else
									 {
										 break; //����ͷ��ˣ����´�0��Mutexɨ�裬��֤����Mutex�������ڱ�Ŀ������ʹ��״̬
									 } 
								 }
							 } 
						 }
						 while(j<MUTEX_SIZE);
#endif						 
						 OSTCBTbl[i].TaskState &= 0xF0;
						 OSTCBTbl[i].TaskState |= TaskState&0x0F; 
						 result= OS_TRUE;
					 }
				}
		 }
	 }
	 OS_INT_EXIT(); 
	 return result;
}
void OSTaskPrioBackup(void* Taskx) //�������ȼ�����
{
	 INT16U i ;
	 OS_INT_ENTER();  	
	 for(i = 0; i < OS_MAX_TASKS; i++) 
		{  
			 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
			 {
				  OSTCBTbl[i].TaskPrioBackup=OSTCBTbl[i].TaskPrio;
					break;
			 }
		}
	 OS_INT_EXIT(); 
}
void OSTaskPrioSet(void* Taskx,INT16U Taskprio) //�������ȼ����ú���(ÿ����һ�ξͻ��Զ�����һ��)
{
	 INT16U i ;
	 OS_INT_ENTER();  	
	 for(i = 0; i < OS_MAX_TASKS; i++) 
		{  
			 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
			 {
				  OSTCBTbl[i].TaskPrioBackup=OSTCBTbl[i].TaskPrio;
					OSTCBTbl[i].TaskPrio = Taskprio;
					break;
			 }
		}
	 OS_INT_EXIT(); 
}
void OSTaskPrioBackupSet(void* Taskx,INT16U Taskprio) //���񱸷����ȼ����ú���(�û��ɵ���)
{
	 INT16U i ;
	 OS_INT_ENTER();  	
	 for(i = 0; i < OS_MAX_TASKS; i++) 
		{  
			 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
			 {
					OSTCBTbl[i].TaskPrioBackup = Taskprio;
					break;
			 }
		}
	 OS_INT_EXIT(); 
}
void OSTimeSliceValSet(INT32U time) //����ʱ����Ƭ���ļ���ֵ
{
   OS_System.TaskTimeSliceCnt=time;
}
void OSClockValSet(INT32U time)    //����ϵͳ���ļ���ֵ
{
  OS_System.ClockCnt=time;
}
#endif
/*-------------------------------------���������йغ�����Page Down)-------------------------------------------*/


/*-----------------------------------ϵͳ��ȡ����ת�йغ�����Page Up)------------------------------------------*/
#if (OS_GET_SET_SW_ENABLE == 1)
OS_BOOL OSLockStateGet(void) //ϵͳ���Ƿ�������������״̬������OS_TRUE�ǣ�OS_FALSE��
{
  if(OS_System.LockNesting>0) return OS_TRUE;
	else return OS_FALSE;
}
INT16U* OSRunTimeGet(void) //��ȡϵͳ�ۼ�����ʱ��,ָ���Ա�ֱ�Ϊ��ms��s��m��h��d��m��y
{
  return (INT16U*)&OS_System.RunTime_ms;
}
INT8U* OSFlagGroupPendTableGet(INT16U FGNum) //��ȡȺ�ȴ���Ա
{
#if (OS_SIGN_PP_ENABLE == 1)	
	INT16U i;
	for(i=0;i<FLAG_SIZE;i++)
	{
		OS_System.ReturnFlagGroupTable[i]=OS_System.FlagGroupTable[FGNum][i];//���Ƴ�Ա�����ز�ѯȺ�б�
	}
	return (INT8U*)OS_System.ReturnFlagGroupTable;
#else 
	return (INT8U*)0;
#endif	
}
INT16U OSMutexBlockTaskGet(INT16U MNum)//��ȡMutex��ǰ����������
{
#if (OS_SIGN_PP_ENABLE == 1)	
	return OS_System.MutexTaskNum[MNum];
#else 
	return 0;	
#endif	
}
INT8U OSTaskStateGet(void* Taskx) //��ȡ����״̬ (��4λΪ����״̬����4λΪ����״̬)
{
	 INT16U i ;
	 INT8U stat;
	 for(i = 0; i < OS_MAX_TASKS; i++) {  
			 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
			 {
				 stat= OSTCBTbl[i].TaskState;
				 break;
			 }
		}
   return stat;		
}
INT16U OSTaskPrioGet(void* Taskx) //��ȡ�������ȼ�
{
	 INT16U i ;
	 INT16U prio;
	 for(i = 0; i < OS_MAX_TASKS; i++) {  
			 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
			 {
				 prio= OSTCBTbl[i].TaskPrio;
				 break;
			 }
		}
   return prio;		
}
INT16U OSTaskPrioBackupGet(void* Taskx) //��ȡ���񱸷����ȼ�
{
	 INT16U i ;
	 INT16U prio;
	 for(i = 0; i < OS_MAX_TASKS; i++) {  
			 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
			 {
				 prio= OSTCBTbl[i].TaskPrioBackup;
				 break;
			 }
		}
   return prio;		
}
INT32U OSTimeSliceValGet(void) //��ȡʱ����Ƭ���ļ���ֵ
{
    return OS_System.TaskTimeSliceCnt;
}
INT32U OSClockValGet(void)    //��ȡϵͳ���ļ���ֵ
{
  return OS_System.ClockCnt;
}
INT32U OSTimerValGet(void) //��ȡϵͳ��ʱ����ǰ����ֵ(Ĭ��Ϊ9MHz��ÿ�μ�����ʱ1/9 us)
{
   return SysTick->VAL;  //����ϵͳ��ʱ����ǰ����ֵ(�����û�us����ȷ��ʱ��������λΪϵͳ��ʱ����Ƶ���˴�ʹ����શ�ʱ������ƵΪ72MhzʱΪ9MHz��������Ƶʱ����8��Ƶ����)
}
void OSTaskSwitchBack(void* Taskx) //������ת ������
{
	 OS_INT_ENTER(); 
   INT16U i;	
	 if(OSTCBCur->TaskAdd!=(INT32U)Taskx) 
	 {
		 for(i = 0; i < OS_MAX_TASKS; i++) {  
				 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
				 {  
					 if(OSTCBTbl[i].TaskState==TASK_RUNNING)
					 {
						 OS_System.TaskSwitchBackNum=OSTCBCur->TaskNum;
						 OSTCBCur=&OSTCBTbl[i]; 
						 OSTimeSliceCounterReset(); 
             OSContextExchange();				 
						 break;
					 }
				 }
		 }
	 }
	 OS_INT_EXIT(); 
}
void OSTaskSwitch(void* Taskx) //������ת  ��������
{
	 OS_INT_ENTER(); 
   INT16U i;	
	 if(OSTCBCur->TaskAdd!=(INT32U)Taskx) 
	 {
		 for(i = 0; i < OS_MAX_TASKS; i++) {  
				 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
				 { 
           if(OSTCBTbl[i].TaskState==TASK_RUNNING)
					 {					 
						 OS_System.TaskNext=i;
						 OSTCBNext = &OSTCBTbl[OS_System.TaskNext];
						 OSTCBCur=&OSTCBTbl[i]; 	
						 OSTimeSliceCounterReset(); 
				     OSContextExchange();
						 break;
					 }
				 }
		 }
	 }
	 OS_INT_EXIT(); 
}
#endif
INT8U OSTaskSwitchCheck_Order(void)
{
#if (OS_GET_SET_SW_ENABLE == 1)	
	 if(OS_System.TaskSwitchBackNum>0) //�Ƿ�����ת�������
	 {
		 OS_System.TaskNext=OS_System.TaskSwitchBackNum;	//����˳�����жϵ�
		 OSTCBNext = &OSTCBTbl[OS_System.TaskSwitchBackNum]; //������תǰ������
		 OS_System.TaskSwitchBackNum=0;                      //�����־
		 OS_System.TaskNextRunFlag=OS_TRUE; //˳�����б�־��TRUE	
     return OS_TRUE;		 
	 }	
	 else return OS_FALSE;
#else 	 
	 return OS_FALSE;
#endif	 
}
void OSTaskSwitchCheck_Prio(void)
{
#if (OS_GET_SET_SW_ENABLE == 1)		
	 if(OS_System.TaskSwitchBackNum>0) //�Ƿ�����ת�������
	 {
		 if(OSTCBCur->TaskPrio<=OSTCBTbl[OS_System.TaskSwitchBackNum].TaskPrio)
		 {
		   OSTCBCur=&OSTCBTbl[OS_System.TaskSwitchBackNum];//������תǰ������
			 OS_System.TaskSwitchBackNum=0;//�����־
		 } 		 
	 }	
#endif	 
}
/*----------------------------------ϵͳ��ȡ����ת�йغ�����Page Down)-----------------------------------------*/


/*-------------------------------------ϵͳ�������йغ�����Page Up)--------------------------------------------*/
#if (OS_REMOTE_ENABLE == 1)


#endif
/*------------------------------------ϵͳ�������йغ�����Page Down)-------------------------------------------*/


/*-----------------------------------ϵͳ���ⶨʱ���йغ�����Page Up)-------------------------------------------*/
#if (OS_TIM_SYS_ENABLE == 1)
INT16U OSTimerCreate(void)   //����OSTimer������Timer_ID
{
	INT16U Timer_ID=0;
  Timer_ID=OS_System.TIMER_APPLY_COUNT;
	OS_System.TIMER_APPLY_COUNT++;
	return Timer_ID;
}
void OSTimerReloadSet(INT16U TNum,INT32U time)//��װ�ض�ʱֵ���� 
{
  OS_System.TIMER[TNum]=time;
}
void OSTimerAutoReloadSet(INT16U TNum,INT32U time)//�Զ���װ�ض�ʱֵ���� (time= 0-�ر��Զ���װ�� �� >0�����Զ���װ��)
{
  OS_System.TIMER_RELOAD[TNum]=time;
}
void OSTimerValueSet(INT16U TNum,INT32U time)//��ʱֵ����
{
  OS_System.TIMER[TNum]=time;
}
void OSTimerReloadSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec)//<=49��17ʱ2��47��
{
	INT32U time;
	time=(day*24*3600+hour*3600+min*60+sec)*1000;
  OS_System.TIMER[TNum]=time;
}
void OSTimerAutoReloadSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec)//<=49��17ʱ2��47��
{
	INT32U time;
	time=(day*24*3600+hour*3600+min*60+sec)*1000;
  OS_System.TIMER_RELOAD[TNum]=time;
}
void OSTimerValueSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec)//<=49��17ʱ2��47��
{
	INT32U time;
	time=(day*24*3600+hour*3600+min*60+sec)*1000;
  OS_System.TIMER[TNum]=time;
}
INT32U OSTimerValueGet(INT16U TNum) //���ص�ǰ����ֵ
{
  return OS_System.TIMER[TNum];
}
OS_BOOL OSTimerStateGet(INT16U TNum) //����OS_TRUE ����ʱ��ɣ�  OS_FALSE��δ���
{
  if(OS_System.TIMER[TNum]==0) return OS_TRUE;
	else return OS_FALSE;
}
void OSTimerCount_Process(void)
{	
	INT16U Count;
	for(Count = 0; Count < OS_System.TIMER_APPLY_COUNT; Count++)
	{
		 if(OS_System.TIMER[Count]>0) OS_System.TIMER[Count]--;
		 if(OS_System.TIMER[Count]==0) 
		 {
			 if(OS_System.TIMER_RELOAD[Count]!=0) 
			 {
				 OS_System.TIMER[Count]=OS_System.TIMER_RELOAD[Count];
			 }
		 }
	}
}
#endif
/*----------------------------------ϵͳ���ⶨʱ���йغ�����Page Down)------------------------------------------*/



/*--------------------------------------ϵͳ�ź����йغ�����Page Up)--------------------------------------------*/
#if (OS_SIGN_PP_ENABLE == 1)
INT16U OSFlagCreate(void)     //����OSFlag������Flag_ID
{
	INT16U Flag_ID=0;
	Flag_ID=OS_System.FLAG_APPLY_COUNT;
  OS_System.FLAG_APPLY_COUNT++;
	return Flag_ID;
}	
INT16U OSFlagGroupCreate(void)     //����OSFlagGroup������FlagGroup_ID
{
	INT16U FlagGroup_ID=0;
	FlagGroup_ID=OS_System.FLAGGROUP_APPLY_COUNT;
  OS_System.FLAGGROUP_APPLY_COUNT++;
	return FlagGroup_ID;
}	
INT16U OSMutexCreate(void)     //����OSMutex������Mutex_ID
{
	INT16U Mutex_ID=0;
	Mutex_ID=OS_System.MUTEX_APPLY_COUNT;
  OS_System.MUTEX_APPLY_COUNT++;
	return Mutex_ID;
}	
INT16U OSMBoxCreate(void)     //����OSMBox������MBox_ID
{
	INT16U MBox_ID=0;
	MBox_ID=OS_System.MBOX_APPLY_COUNT;
  OS_System.MBOX_APPLY_COUNT++;
	return MBox_ID;
}	
void OSFlagPost(INT16U FNum) //���ͱ�־��
{
	OSSchedLock();
	INT16U i,j;
	for(i=0;i<OS_MAX_TASKS;i++)
	{ 
    if(OSTCBTbl[i].TaskFlagBit&_BIT_Flag)//����OSFlag���ڵȴ��źű�־״̬
		{ 
			if(OSTCBTbl[i].FlagName==FNum)//�жϸõȴ�����ȴ���OSFlag�Ƿ��ǵ�ǰ׼�����͵�OSFlag
			{ 
				OS_System.FLAG[FNum][i]=OS_TRUE; //���ȴ�OSFlag�������ͱ�־��
				OS_System.POST_FLAG_COUNT[FNum]++;//�ȴ������źŴ����ۼ�
				if((OS_System.FLAGPendType[FNum][i]!=OSFlag_NBPC)&&(OS_System.FLAGPendType[FNum][i]!=OSFlag_NBPN))
				{
					OSTCBTbl[i].TaskFlagBit&=~_BIT_Flag;//����źű�־״̬
				  OSTCBTbl[i].TaskDelayMs=1;  //�ȴ���ʱ��1��ȡ���ȴ�OSFlag����ĵȴ���ʱ 
				}
			}
		}
		if(OSTCBTbl[i].TaskFlagBit&_BIT_FlagGroup)//ȺOSFlag���ڵȴ��źű�־״̬
		{
			for(j=0;j<OS_System.FLAGGROUP_APPLY_COUNT;j++)
			{
				if(OS_System.FlagGroupTable[j][FNum]==OS_TRUE)//�ж��Ƿ�ΪȺ��Ա
				{
					OS_System.FLAG[FNum][i]=OS_TRUE; //���ȴ�OSFlagȺ�������ͳ�Ա��־��		
				}
			}
		}
	}
	OSSchedUnlock();
}

INT8U OSFlagPend(OSFlagPendMode pendtype,INT16U FNum,INT32U timeout_ms) //�ȴ���־�� ����ʱʱ��  ������OS_TRUE���ȴ�δ��ʱ  OS_FALSE���ȴ���ʱ (NBPCģʽʱ�����ź����ۼ�����)
{
	INT8U result;
	OSSchedLock();
	OSTCBCur->FlagName=FNum;
	OSTCBCur->TaskFlagBit|=_BIT_Flag;
	OS_System.FLAGPendType[FNum][OSTCBCur->TaskNum]=pendtype;
	OSSchedUnlock();
	switch(pendtype)
	{
		case OSFlag_BPN:
		{
			OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//����ɱ�־��,�ȴ��µı�־������
			OS_delayMs(timeout_ms);
			OSTCBCur->TaskFlagBit&=~_BIT_Flag;
			if(OSTCBCur->TaskFlagBit&_BIT_TimeOut) //��ʱ���
			{
				OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//���������ı�־��
				OSTCBCur->TaskFlagBit&=~_BIT_TimeOut;//�����ʱ��־λ
				result = OS_FALSE;
			}
			else
			{	
				if(OS_System.FLAG[FNum][OSTCBCur->TaskNum]==OS_TRUE)
				{
					OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//���������ı�־��
					result = OS_TRUE; 
				}
				else
				{
					result = OS_FALSE;
				}
			}
		} 
		break;
		case OSFlag_NBPN:
		{
			if(OS_System.FLAG[FNum][OSTCBCur->TaskNum]==OS_TRUE)
			{
				OSTCBCur->TaskFlagBit&=~_BIT_Flag;
				OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//���������ı�־��
				result = OS_TRUE; 
			}
			else
			{
				result = OS_FALSE;
			}
		}
		break;
		case OSFlag_BPO:
		{
		  if(OS_System.FLAG[FNum][OSTCBCur->TaskNum]==OS_FALSE)//���ɱ�־
			{
				OS_delayMs(timeout_ms);
				OSTCBCur->TaskFlagBit&=~_BIT_Flag;
				if(OSTCBCur->TaskFlagBit&_BIT_TimeOut) //��ʱ���
				{
					OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//���������ı�־��
					OSTCBCur->TaskFlagBit&=~_BIT_TimeOut;//�����ʱ��־λ
					result = OS_FALSE;
				}
				else
				{	
					OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//���������ı�־��
					result = OS_TRUE; 
				}		
			}
			else
			{
				OSTCBCur->TaskFlagBit&=~_BIT_Flag;
				OSTCBCur->TaskDelayMs=0;//���䴦�ھ���״̬
				OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//���������ı�־��
				result = OS_TRUE;
			}
		}
		break;
		case OSFlag_BPC:
		{
		  if(OS_System.POST_FLAG_COUNT[FNum]==0)//����־�ۼ�����
			{
				OS_delayMs(timeout_ms);
				OSTCBCur->TaskFlagBit&=~_BIT_Flag;
				if(OSTCBCur->TaskFlagBit&_BIT_TimeOut) //��ʱ���
				{
					OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//���������ı�־��
					OSTCBCur->TaskFlagBit&=~_BIT_TimeOut;//�����ʱ��־λ
					result = OS_FALSE;
				}
				else
				{	
					OS_System.POST_FLAG_COUNT[FNum]--;//�ȴ������źŴ���-1(�Ѵ���)
					OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//���������ı�־��
					result = OS_TRUE; 
				}		
			}
			else
			{
				OSTCBCur->TaskFlagBit&=~_BIT_Flag;
				OS_System.POST_FLAG_COUNT[FNum]--;//�ȴ������źŴ���-1(�Ѵ���)
				OSTCBCur->TaskDelayMs=0;//���䴦�ھ���״̬
				OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//���������ı�־��
				result = OS_TRUE;
			}
		}
		break;
		case OSFlag_NBPC:
		{
			if(OS_System.POST_FLAG_COUNT[FNum]>0)
			{
				result=OS_System.POST_FLAG_COUNT[FNum];//����ʣ�ڵȴ��������
				OS_System.POST_FLAG_COUNT[FNum]--;//�ȴ������źŴ���-1(�Ѵ���)
			}
			else
			{
			  result = OS_FALSE;
			}
		}
		break;
	}
	return result;
}

void OSFlagAddToGroup(INT16U FGNum,INT16U FNum)  //��ӱ�־������־��Ⱥ
{
	OSSchedLock();
  OS_System.FlagGroupTable[FGNum][FNum]=OS_TRUE;
	OSSchedUnlock();
}

OS_BOOL OSFlagGroupPend(OSFGroupPendMode pendtype,INT16U FGNum,INT32U timeout_ms) //�ȴ���־��Ⱥ ����ʱʱ��  ������OS_TRUE���ȴ�δ��ʱ  OS_FALSE���ȴ���ʱ
{
	OS_BOOL result;
	OSSchedLock();
	OSTCBCur->FlagGroupName=FGNum;
	OS_System.FlagGroupNameInTask[OSTCBCur->TaskNum]=FGNum;
	OSTCBCur->TaskFlagBit|=_BIT_FlagGroup;
	OSSchedUnlock();
	switch(pendtype)
	{
		case OSFGroup_BPN:
		{
			OS_delayMs(timeout_ms);	
			OSTCBCur->TaskFlagBit&=~_BIT_FlagGroup;	
			if(OSTCBCur->TaskFlagBit&_BIT_TimeOut) //��ʱ���
			{
				OSTCBCur->TaskFlagBit&=~_BIT_TimeOut;//�����ʱ��־λ
				result = OS_FALSE;
			}
			else
			{
				OS_System.POST_FGROUP_COUNT[FGNum]--;
				OSTCBCur->TaskFlagBit&=~_BIT_FlagGroup;//���������FlagGroupȺ�ȴ���־
				result = OS_TRUE;
			}
		}
		break;
		case OSFGroup_NBPN:
		{
			if(OS_System.GROUPRXFLAG[FGNum][OSTCBCur->TaskNum]==OS_TRUE)
			{
				OS_System.POST_FGROUP_COUNT[FGNum]--;
				OSTCBCur->TaskFlagBit&=~_BIT_FlagGroup;
				OS_System.GROUPRXFLAG[FGNum][OSTCBCur->TaskNum]=OS_FALSE;
				result = OS_TRUE;
			}
			else
			{
				result = OS_FALSE;
			}
		}
		break;
	}
	return result;
}

void OSMutexPost(INT16U MNum) //���ͻ����� ����Ҫ���ڽ��mutex����״̬,���������mutex�ȴ��������¾���mutexȨ�ޣ�
{
	OSSchedLock();
	if(OSTCBCur->TaskNum==OS_System.MutexTaskNum[MNum])//�жϵ�ǰ�����Ƿ����PostȨ��
	{
		if((OSTCBCur->TaskFlagBit&_BIT_Mutex)==0)//�жϵ�ǰ�����Ƿ��ڷǵȴ�mutex״̬��
		{
			if(OS_System.MUTEX[MNum]==OS_TRUE) //�ж�mutex�Ƿ�Ϊtrue(æ)
			{
				OS_System.MutexTaskNum[MNum]=0;//����PostȨ��
				OS_System.MUTEX[MNum]=OS_FALSE;//��mutexΪOS_FALSE��ʾmutex����
			}
		}
	}
	OSSchedUnlock();
}
OS_BOOL OSMutexPend(OSMutexPendMode pendtype,INT16U MNum,INT32U timeout_ms) //�ȴ������� ����ʱʱ��  ������OS_TRUE���ȴ�δ��ʱ  OS_FALSE���ȴ���ʱ
{
	OS_BOOL result;
	OSSchedLock();
	OSTCBCur->MutexName=MNum;
	OS_System.MutexNameInTask[OSTCBCur->TaskNum]=MNum;
	if(OS_System.MutexTaskNum[MNum]==0) OS_System.MutexTaskNum[MNum]=OSTCBCur->TaskNum;//������PostȨ��
	OSTCBCur->TaskFlagBit|=_BIT_Mutex;//��λ�ȴ�mutex��־
	OSSchedUnlock();
	switch(pendtype)
	{
		case OSMutex_BPN:
		{
			OS_delayMs(timeout_ms);
			OSTCBCur->TaskFlagBit&=~_BIT_Mutex;//����ȴ�mutex��־
			if(OSTCBCur->TaskFlagBit&_BIT_TimeOut) //��ʱ���
			{	
				OSTCBCur->TaskFlagBit&=~_BIT_TimeOut;//�����ʱ��־λ
				result = OS_FALSE;
			}
			else
			{
				OSTCBCur->TaskFlagBit&=~_BIT_TimeLess;//ȡ�����޵ȴ���־
				OS_System.MutexTaskNum[MNum]=OSTCBCur->TaskNum;//������PostȨ��(�������)
				result = OS_TRUE;
			}
		}
		break;
		case OSMutex_NBPN:
		{
		  if(OS_System.MutexTaskNum[MNum]==OSTCBCur->TaskNum)
			{
				OSTCBCur->TaskFlagBit&=~_BIT_Mutex;//����ȴ�mutex��־
				OS_System.MUTEX[MNum]=OS_TRUE;
			  result = OS_TRUE;
			}
			else
			{
			  result = OS_FALSE;
			}
		}
		break;
	}
	return result;
}
void OSMBoxPost(INT16U MNum,void* fp)  //�����ʼ�
{
	OSSchedLock();
	INT16U i;
	for(i=0;i<OS_MAX_TASKS;i++)
	{
		if(OSTCBTbl[i].TaskFlagBit&_BIT_MBox)//�жϸ������Ƿ��ڵȴ�mbox״̬
		{
			if(OSTCBTbl[i].MBoxName==MNum)//�жϵȴ���mbox�Ƿ��ǵ�ǰ׼�����͵�MNum
			{
				OS_System.MBOX[MNum*OS_MAX_TASKS+i]=fp;//���ڵȴ��ʼ����������ʼ�
				OS_System.POST_MBOX_COUNT[MNum]++;
				OS_System.MBQueue[OS_System.POST_MBQ_COUNT[MNum]+MNum*MBQ_SIZE]=fp;
				if(OS_System.POST_MBQ_COUNT[MNum]<MBQ_SIZE-1) OS_System.POST_MBQ_COUNT[MNum]++;
				if((OS_System.MBOXPendType[MNum][i]!=OSMBox_NBPN)&&(OS_System.MBOXPendType[MNum][i]!=OSMBox_NBPQ))
				{
					OSTCBCur->TaskFlagBit&=~_BIT_MBox;
				  OSTCBTbl[i].TaskDelayMs=0;//�ȴ���ʱ��0
				}
			}
		}
	}
	OSSchedUnlock();
} 

void* OSMBoxPend(OSMBoxPendMode pendtype,INT16U MNum,INT32U timeout_ms) //�ȴ����� ����ʱʱ��  �����ط�(void*)0���ȴ�δ��ʱ  (void*)0���ȴ���ʱ
{
	OSSchedLock();
	INT16U MBQCount;
	INT32U* _mbox;	
	OSTCBCur->MBoxName=MNum;
	OSTCBCur->TaskFlagBit|=_BIT_MBox;
	OS_System.MBOXPendType[MNum][OSTCBCur->TaskNum]=pendtype;
	OSSchedUnlock();
	switch(pendtype)
	{
		case OSMBox_BPN:
		{
			OS_delayMs(timeout_ms);	
			OSTCBCur->TaskFlagBit&=~_BIT_MBox;
			_mbox=(INT32U*)OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum];
			if(OSTCBCur->TaskFlagBit&_BIT_TimeOut) //��ʱ���
			{
				OSTCBCur->TaskFlagBit&=~_BIT_TimeOut;//�����ʱ��־λ
				_mbox=(void*)0;
			}
			else
			{  
				if(_mbox!=(void*)0)
				{
					OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum]=(void*)0;  //��ձ���������Ϊ(void*)0	
				}
				else
				{
				  OS_System.POST_MBOX_COUNT[MNum]--;
				}
			}
		}
		break;
		case OSMBox_BPQ:
		{
		  if(OS_System.POST_MBQ_COUNT[MNum]==0)
			{
				OS_delayMs(timeout_ms);	
				OSTCBCur->TaskFlagBit&=~_BIT_MBox;
				_mbox=(INT32U*)OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum];
				if(OSTCBCur->TaskFlagBit&_BIT_TimeOut) //��ʱ���
				{
					OSTCBCur->TaskFlagBit&=~_BIT_TimeOut;//�����ʱ��־λ
					_mbox=(void*)0;
				}
				else
				{  
					if(_mbox!=(void*)0)
					{
						OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum]=(void*)0;  //��ձ���������Ϊ(void*)0	
					}
					else
					{
						_mbox=(INT32U*)OS_System.MBQueue[0+MNum*MBQ_SIZE];
						for(MBQCount=0;MBQCount<OS_System.POST_MBQ_COUNT[MNum]-1;MBQCount++)
						{
							OS_System.MBQueue[MBQCount+MNum*MBQ_SIZE]=OS_System.MBQueue[MBQCount+1+MNum*MBQ_SIZE];
						}
						OS_System.POST_MBQ_COUNT[MNum]--;
					}
				}
			}
			else
			{
				_mbox=(INT32U*)OS_System.MBQueue[0+MNum*MBQ_SIZE];
				for(MBQCount=0;MBQCount<OS_System.POST_MBQ_COUNT[MNum]-1;MBQCount++)
				{
				  OS_System.MBQueue[MBQCount+MNum*MBQ_SIZE]=OS_System.MBQueue[MBQCount+1+MNum*MBQ_SIZE];
				}
				OS_System.POST_MBQ_COUNT[MNum]--;
			}
		}
		break;
		case OSMBox_NBPN:
		{
			_mbox=(INT32U*)OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum];
			if(_mbox!=(void*)0)
			{
				OS_System.POST_MBOX_COUNT[MNum]--;
				OSTCBCur->TaskFlagBit&=~_BIT_MBox;
				OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum]=(void*)0;  //��ձ���������Ϊ(void*)0	
			}
			else
			{
				_mbox=(void*)0;
			}
		}
		break;
		case OSMBox_NBPQ:
		{
		  if(OS_System.POST_MBQ_COUNT[MNum]>0)
			{
			  _mbox=(INT32U*)OS_System.MBQueue[0+MNum*MBQ_SIZE];
				for(MBQCount=0;MBQCount<OS_System.POST_MBQ_COUNT[MNum]-1;MBQCount++)
				{
				  OS_System.MBQueue[MBQCount+MNum*MBQ_SIZE]=OS_System.MBQueue[MBQCount+1+MNum*MBQ_SIZE];
				}
				OS_System.POST_MBQ_COUNT[MNum]--;
			}
			else
			{
			  _mbox=(void*)0;
			}
		}
		break;
	}
	return _mbox;
}
#endif
/*-------------------------------------ϵͳ�ź����йغ�����Page Down)-------------------------------------------*/



/*------------------------------------ϵͳ�ź��������йغ�����Page Up)-------------------------------------------*/
#if (OS_SIGN_PP_ENABLE == 1)
void OSFlagGroupHandler(void)
{
	INT16U TaskCount,Count,NCount;
	for(TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++)
	{
		if(OSTCBTbl[TaskCount].TaskFlagBit&_BIT_FlagGroup)//�����FlagGroup�ȴ�������
		{	
			NCount=0;
			for(Count=0; Count < OS_System.FLAG_APPLY_COUNT; Count++)
			{
				if(OS_System.FlagGroupTable[OS_System.FlagGroupNameInTask[TaskCount]][Count]==OS_TRUE)
				{
					if(OS_System.FLAG[Count][TaskCount]==OS_TRUE) NCount++;
					else break;
				}
			}
			if((Count==OS_System.FLAG_APPLY_COUNT)&&(NCount!=0))
			{
				for(Count=0; Count < OS_System.FLAG_APPLY_COUNT; Count++)
				{
					if(OS_System.FlagGroupTable[OS_System.FlagGroupNameInTask[TaskCount]][Count]==OS_TRUE)
					{
						if(OS_System.FLAG[Count][TaskCount]==OS_TRUE) OS_System.FLAG[Count][TaskCount]=OS_FALSE;
					}
				}
				OSTCBTbl[TaskCount].TaskDelayMs=1;//׼���˳���ʱ������ȴ����е�״̬(�����ȼ�����)
				OSTCBTbl[TaskCount].TaskFlagBit&=~_BIT_TimeOut;//�����ʱ��־
			}
		}
	}
}
void OSMutexHandler_Order(void)
{
  INT16U TaskCount;	
	for(TaskCount=OS_System.TaskNext; TaskCount < OS_MAX_TASKS; TaskCount++)//���NextTask�Ժ���ǰ�������Ƿ��еȴ�mutex
	{
		if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)&&\
			 (OSTCBTbl[TaskCount].TaskFlagBit&_BIT_Mutex)&&(OS_System.MUTEX[OS_System.MutexNameInTask[TaskCount]]==OS_FALSE) )
			{
				if(OS_System.MutexPendType[OS_System.MutexNameInTask[TaskCount]][TaskCount]==OSMutex_BPN)
				{				
					OSTCBTbl[TaskCount].TaskFlagBit&=~_BIT_TimeLess;//ȡ�����޵ȴ���־
					OSTCBTbl[TaskCount].TaskFlagBit&=~_BIT_TimeOut;//ȡ����ʱ��־
					OSTCBTbl[TaskCount].TaskDelayMs=1;//����׼������״̬(�ܷ�����������ȡ�������ȼ�)
				  OSTCBTbl[TaskCount].TaskFlagBit&=~_BIT_Mutex;//ȡ���ȴ�mutex��־
				  OS_System.MUTEX[OS_System.MutexNameInTask[TaskCount]]=OS_TRUE;//��ǰ�����mutex��æ					
				}
				else
				{
					if(OSTCBTbl[TaskCount].TaskDelayMs<=1)
					{
						 OS_System.MutexTaskNum[OS_System.MutexNameInTask[TaskCount]]=OSTCBCur->TaskNum;//������PostȨ��				
					}
				}
				break;
			}		
	}
}
void OSMutexHandler_Prio(void)
{
  INT16U TaskCount,Count;	
	for(Count=0,TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++)
	{
		if(OSTCBTbl[Count].TaskPrio<OSTCBTbl[TaskCount].TaskPrio) 
		{
			if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)&&\
				 (OSTCBTbl[TaskCount].TaskFlagBit&_BIT_Mutex)&&(OS_System.MUTEX[OS_System.MutexNameInTask[TaskCount]]==OS_FALSE) )
				{
					Count=TaskCount; //Count������¼��mutex�ȴ���������ȼ�����
				}
		}			
	}
	if(Count!=0)
	{
		if(OS_System.MutexPendType[OS_System.MutexNameInTask[Count]][Count]==OSMutex_BPN)
		{				
			OSTCBTbl[Count].TaskFlagBit&=~_BIT_TimeLess;//ȡ�����޵ȴ���־
			OSTCBTbl[Count].TaskFlagBit&=~_BIT_TimeOut;//ȡ����ʱ��־
			OSTCBTbl[Count].TaskDelayMs=1;//����׼������״̬(�ܷ�����������ȡ�������ȼ�)
			OSTCBTbl[Count].TaskFlagBit&=~_BIT_Mutex;//ȡ���ȴ�mutex��־
			OS_System.MUTEX[OS_System.MutexNameInTask[Count]]=OS_TRUE;//��ǰ�����mutex��æ					
		}
		else
		{
			if(OSTCBTbl[Count].TaskDelayMs<=1)
			{
				 OS_System.MutexTaskNum[OS_System.MutexNameInTask[Count]]=OSTCBCur->TaskNum;//������PostȨ��				
			}
		}
	}
}
#endif
/*-----------------------------------ϵͳ�ź��������йغ�����Page Down)----------------------------------------*/



/*------------------------------------ϵͳʱ��ͳ���йغ�����Page Up)-------------------------------------------*/
void OS_TaskStackCounter_Process(void)
{
	INT16U TaskCount;
	INT32U Count;
	INT32U StkFreeCount;	
	OS_INT_ENTER();
	for(TaskCount=0;TaskCount<OS_MAX_TASKS;TaskCount++)
	{
		if(OSTCBTbl[TaskCount].StkPtr != (STK32U*)0)
		{
			StkFreeCount=0;
			for(Count=0;Count<OSTCBTbl[TaskCount].StkSize;Count++)
			{
				if(OSTCBTbl[TaskCount].StkAdd[Count]==0)
				{
					StkFreeCount++;		
				}
				else break;
			}
			OSTCBTbl[TaskCount].StkFree=StkFreeCount;
			if(OSTCBTbl[TaskCount].StkSize>OSTCBTbl[TaskCount].StkFree)
			{
				OSTCBTbl[TaskCount].StkUsed=OSTCBTbl[TaskCount].StkSize-OSTCBTbl[TaskCount].StkFree;
			}
			else
			{
				OS_System.Error=OS_TRUE;
			}
		}
	}
	OS_INT_EXIT();
}
#if (OS_TIMECOUNTSYS_ENABLE == 1)
void OS_ShutDown_DelayTime_Set_YMDHMS(INT32U year,INT8U mouth,INT16U day,INT8U hour,INT8U minute,INT8U second) //ϵͳ�رն�ʱʱ������(�û�����)
{			 
	OS_System.ShutDownTime_second=second;
  OS_System.ShutDownTime_minute=minute;
	OS_System.ShutDownTime_hour  =hour;
	OS_System.ShutDownTime_day   =day;	
	OS_System.ShutDownTime_mouth =mouth;
	OS_System.ShutDownTime_year  =year;
}
void OSRunTimeCount_Process(void) //ϵͳ����ʱ��ͳ��(�û�������)
{
	 if(OS_System.RunTime_ms<1000) OS_System.RunTime_ms++;
	 else
	 {
		 OS_System.RunTime_ms=0;
		 OS_System.RunTime_second++;
		 if(OS_System.RunTime_second>=60) {
  		  OS_System.RunTime_second=0;OS_System.RunTime_minute++;
		 if(OS_System.RunTime_minute>=60) { 
		    OS_System.RunTime_minute=0;OS_System.RunTime_hour++;
		 if(OS_System.RunTime_hour>=24)   { 
		    OS_System.RunTime_hour=0;OS_System.RunTime_day++;
		 if(OS_System.RunTime_hour>=24)   { 
		    OS_System.RunTime_hour=0;OS_System.RunTime_day++;
		 if(OS_System.RunTime_day>=30)    { 
		    OS_System.RunTime_day=0;OS_System.RunTime_mouth++;
		 if(OS_System.RunTime_mouth>=12)  { 
		    OS_System.RunTime_mouth=0;OS_System.RunTime_year++;
		 }}}}}} 
  
		 if((OS_System.ShutDownTime_second==0)&&(\
        (OS_System.ShutDownTime_minute!=0)||\
				(OS_System.ShutDownTime_hour  !=0)||\
				(OS_System.ShutDownTime_day   !=0)||\
				(OS_System.ShutDownTime_mouth !=0)||\
				(OS_System.ShutDownTime_year  !=0)))
		 {	
       OS_System.ShutDownTime_second=1;
		 }				
		 if((OS_System.ShutDownTime_second==2)&&\
        (OS_System.ShutDownTime_minute==0)&&\
				(OS_System.ShutDownTime_hour  ==0)&&\
				(OS_System.ShutDownTime_day   ==0)&&\
				(OS_System.ShutDownTime_mouth ==0)&&\
				(OS_System.ShutDownTime_year  ==0))
			{
				OS_System.ShutDownTime_second=0;
				OS_System.Running=OS_FALSE;	
				OSprintf("��ʱ�ر���ɣ�ϵͳֹͣ����\r\n");
			}
			else
			{
			 if(OS_System.ShutDownTime_second>1) OS_System.ShutDownTime_second--;	
			 if(OS_System.ShutDownTime_second==1) 
				 {
					OS_System.ShutDownTime_second=60;
				  if(OS_System.ShutDownTime_minute>0) 
					{
						OS_System.ShutDownTime_minute--;
					}
					else if(OS_System.ShutDownTime_hour>0) 
					{
					  OS_System.ShutDownTime_hour--;
						OS_System.ShutDownTime_minute=59;
					}
					else if(OS_System.ShutDownTime_day>1) 
					{
						OS_System.ShutDownTime_day--;
					  OS_System.ShutDownTime_hour=23;
						OS_System.ShutDownTime_minute=59;
					}		
					else if(OS_System.ShutDownTime_mouth>1) 
					{
						OS_System.ShutDownTime_mouth--;
						OS_System.ShutDownTime_day=30;
					  OS_System.ShutDownTime_hour=23;
						OS_System.ShutDownTime_minute=59;
					}		
					else if(OS_System.ShutDownTime_year>1) 
					{
						OS_System.ShutDownTime_year--;
						OS_System.ShutDownTime_mouth=12;
						OS_System.ShutDownTime_day=30;
					  OS_System.ShutDownTime_hour=23;
						OS_System.ShutDownTime_minute=59;
					}												
				}			
			}
	 }
}
#endif
/*-----------------------------------ϵͳʱ��ͳ���йغ�����Page Down)------------------------------------------*/


/*----------------------------------------ϵͳ���ĺ�����Page Up)--------=-------------------------------------*/

void OSTimeSliceCounterReset(void) //ʱ����Ƭ��������0
{
	OS_System.TimeSliceCnt=0; //ʱ����Ƭ��������0��׼�����¼���
}
/*--------------------------------------�����л�����--------------------------------*/
void OSSchedSwitch(void) //������Ȳ��л�����
{
	OSScheduler_Process();
	if(OSTCBRun!=OSTCBCur) 
	{
		OSContextExchange();  
	}
}
void OSContextExchangeToTask(OS_TCB* tcb)//�л���ָ������
{
	OSTCBCur=tcb;
  OSContextExchange(); 
}
void OSSchedLock(void)   //�����л�����(�û��ɵ���) �����󲻻��л�����cpu��һֱ���е�ǰ����ֱ������
{
	if(OS_System.Running == OS_TRUE)  
		{                                
			OS_INT_ENTER();  
			if (OS_System.IntNesting == 0) 
				{                                          
					if (OS_System.LockNesting < 255) 
						{         
							OS_System.LockNesting++;
              OSTCBCur->TaskState&=0x0F;
							OSTCBCur->TaskState|=0x10;
					  }
			  }
		  OS_INT_EXIT();   
	  }
}
void OSSchedUnlock(void)   //���������OSSchedLock��OSSchedUnlock����ɶԳ��֣�(�û��ɵ���)
{
	if (OS_System.Running == OS_TRUE)
		{                            
			OS_INT_ENTER();   
			if (OS_System.LockNesting > 0){                       
			OS_System.LockNesting--;   
			if (OS_System.LockNesting == 0){
          OSTCBCur->TaskState&=0x0F;			
			if (OS_System.IntNesting == 0){                
					OS_INT_EXIT();  
					OSPendSVPulse();  
				}else OS_INT_EXIT();           	
				}else OS_INT_EXIT(); 
				}else OS_INT_EXIT();
		}
}
/*---------------------------------------------------------------------------------*/
/*------------------------------ϵͳ�ں��йغ���-------------------------------------*/
void OSRunModeProcess_Order(void) //˳��ģʽ�������
{
	INT16U TaskCount;
	for(TaskCount = 1; TaskCount < OS_MAX_TASKS; TaskCount++) //��������������ʱʱ���Ƿ��еĵ�ʱ
	{ 
		if(OSTCBTbl[TaskCount].TaskState==TASK_RUNNING) //����������̬������
		{
			if( OSTCBTbl[TaskCount].TaskDelayMs ==1 )//�����ʱʱ���Ƿ�ʱ
			 {
				 OSTCBCur=&OSTCBTbl[TaskCount]; 	        //��ת����ʱ��ɵ�����
				 OSTCBTbl[TaskCount].TaskDelayMs=0;	  //��ʱ��0
				 OS_System.TaskNextRunFlag=OS_FALSE;    //˳�����б�־��FALSE	
				 break;
			 }
		 }	 
	}	
	if(TaskCount==OS_MAX_TASKS) OS_System.TaskNextRunFlag=OS_TRUE;	//˳�����б�־��TRUE			 
	if(OS_System.TaskNextRunFlag==OS_TRUE)	//�ж��Ƿ���԰�����˳������
	{
		OSTCBCur = OSTCBNext;
		if(OS_System.TaskNext<OS_MAX_TASKS-1) OS_System.TaskNext++;
		else OS_System.TaskNext = 0;						
		while((OSTCBTbl[OS_System.TaskNext].StkPtr == (STK32U*)0)||(OSTCBTbl[OS_System.TaskNext].TaskState!=TASK_RUNNING)\
				 ||OSTCBTbl[OS_System.TaskNext].TaskDelayMs > 1) //�����Ƿ�״̬����
		{ 
			OS_System.TaskNext++;
			if(OS_System.TaskNext == OS_MAX_TASKS) 
			{
				OS_System.TaskNext = 0;
				OS_System.RunOneRound=OS_TRUE;
			}
		}
		OSTCBNext = &OSTCBTbl[OS_System.TaskNext];	//����Next����
	}	
}
INT16U OSHighestPrioTaskNumGet(void)//��ȡ�Ѵ���������״̬�����е�������ȼ�����
{
  INT16U TaskCount,Count;	
	for(Count=0,TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++) //�����������еĳ���������ȼ�����
	{ 
		if(OSTCBTbl[Count].TaskPrio<OSTCBTbl[TaskCount].TaskPrio) 
		{
			if(OSTCBTbl[Count].StkPtr!=(STK32U*)0)
				{
					Count=TaskCount; //Count������¼������ȼ�
				}
		}				
	}	
	return Count;
}
INT16U OSRunStateHighestPrioTaskNumGet(void)//��ȡ��������̬��������ȼ�����
{
  INT16U TaskCount,HighestPrioTask;	
	for(HighestPrioTask=0,TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++) //�����������еĳ���������ȼ�����
	{ 
		if(OSTCBTbl[HighestPrioTask].TaskPrio<OSTCBTbl[TaskCount].TaskPrio) 
		{
			if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0))
				{
					HighestPrioTask=TaskCount; //Count������¼������ȼ�
				}
		}				
	}	
	return HighestPrioTask;
}
INT16U OSRdyHighestPrioTaskNumGet(void)//��ȡ׼�����о���������ȼ�����(�����ȴ�����������ȼ�����(����ͬ������))
{
  INT16U TaskCount,HighestPrioTask;	
	for(HighestPrioTask=0,TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++) //�����������еĳ���������ȼ�����
	{ 
		if(OSTCBTbl[HighestPrioTask].TaskPrio<OSTCBTbl[TaskCount].TaskPrio) 
		{
			if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)&&\
				 (OSTCBTbl[TaskCount].TaskDelayMs <= 1)&&((OSTCBTbl[TaskCount].TaskFlagBit&_BIT_TimeLess)==0) )
				{
					HighestPrioTask=TaskCount; //HighestPrio������¼������ȼ�
				}
		}				
	}	
	return HighestPrioTask;
}

/***************************************************************************************
*��������OSNextRdyHighestPrioTaskNumGet
*����  ����ȡ���о���������ȼ����񣬾�����ͬ���ȼ��Ķ��������ѯ
*�ص�  �����ɨ�赽����������ȼ����ϴ���ͬ�Ҿ��и����ȼ���������ϴθ����й����������⻹����������
        �򷵻���������(�����������2��������ʱ����˳�򷵻أ�ʵ�־�����ͬ���ȼ��Ķ��������ѯ)
***************************************************************************************/
INT16U OSNextRdyHighestPrioTaskNumGet(void)//��ȡ���о���������ȼ�����(����δ���й���������ȼ�����(�����ȼ�һ����ѯ������)))
{
  INT16U TaskCount,HighestPrioTask,LastPrioTaskCount=0,SecdyHigestPrioTask,SecdyCheckFlag=0,MainCheckFlag=0;	
	for(HighestPrioTask=0,TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++) //�����������еĳ���������ȼ�����
	{ 
		if(OSTCBTbl[HighestPrioTask].TaskPrio<OSTCBTbl[TaskCount].TaskPrio) 
		{
			if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)&&\
				 (OSTCBTbl[TaskCount].TaskDelayMs <= 1)&&((OSTCBTbl[TaskCount].TaskFlagBit&_BIT_TimeLess)==0) )
				{
					HighestPrioTask=TaskCount; //HighestPrioTask������¼ɨ�赽��������ȼ�����
				}
		}			
	}	
	if(OSTCBTbl[HighestPrioTask].TaskPrio>OSTCBTbl[OS_System.RunLastTask].TaskPrio)
	{
	  
	}
	else if(OSTCBTbl[HighestPrioTask].TaskPrio==OSTCBTbl[OS_System.RunLastTask].TaskPrio)
	{
		for(TaskCount=0; TaskCount < OS_MAX_TASKS; TaskCount++) //ͳ���������еĳ���������ȼ���������(�ϴ����й��ĳ���)
		{ 
			if(TaskCount==OS_System.RunLastTask) continue ;
			if(OSTCBTbl[HighestPrioTask].TaskPrio==OSTCBTbl[TaskCount].TaskPrio) 
			{
				if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)&&\
					 (OSTCBTbl[TaskCount].TaskDelayMs <= 1)&&((OSTCBTbl[TaskCount].TaskFlagBit&(1<<_BIT_TimeLess))==0) )
					{
						LastPrioTaskCount++; //LastPrioTaskCount������¼������ȼ���������(�ϴ����й��ĳ���)
					}
			}			
		}	
		if(LastPrioTaskCount>0) //���ɨ�赽���ϴ����е��������ȼ���ͬ�����񳬹�0��(�������ϴ�������)
		{
		
			for(TaskCount=0; TaskCount < OS_MAX_TASKS; TaskCount++) //�����������еĳ���������ȼ�����
			{ 
				if(TaskCount==OS_System.RunLastTask) continue ;
				if(OSTCBTbl[HighestPrioTask].TaskPrio==OSTCBTbl[TaskCount].TaskPrio) 
				{
					if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[OS_System.TaskNext].StkPtr!=(STK32U*)0)&&\
						 (OSTCBTbl[TaskCount].TaskDelayMs <= 1)&&((OSTCBTbl[TaskCount].TaskFlagBit&_BIT_TimeLess)==0) )
						{
							if(SecdyCheckFlag==0)
							{
								if(TaskCount<OS_System.RunLastTask) 
								{
									SecdyCheckFlag=1;
									SecdyHigestPrioTask=TaskCount;
								}
							}
							if(TaskCount>OS_System.RunLastTask) 
							{
								MainCheckFlag=1;
								HighestPrioTask=TaskCount;
								break;
							}
						}
				 }			
			}	 
			if(MainCheckFlag==0)  HighestPrioTask=SecdyHigestPrioTask;
		}
	}
	return HighestPrioTask;
}
void OSRunModeProcess_Prio(void)//���ȼ�ģʽ�������
{
	INT16U HighestPrioTask;
	HighestPrioTask=OSNextRdyHighestPrioTaskNumGet();
	if(OS_System.TaskHighestPrioLast!=OSTCBTbl[HighestPrioTask].TaskPrio)
	{
		OS_System.TaskHighestPrioLast=OSTCBTbl[HighestPrioTask].TaskPrio;
	  OS_System.RunLastTask=OS_System.TaskHighestRunLast[OS_System.TaskHighestPrioLast];
		HighestPrioTask=OSNextRdyHighestPrioTaskNumGet();	
	}
	if(OS_System.RunLastTask!=HighestPrioTask)
	{
	  OS_System.RunLastTask=HighestPrioTask;
		OS_System.TaskHighestRunLast[OS_System.TaskHighestPrioLast]=OS_System.RunLastTask;
	}
	OSTCBCur=&OSTCBTbl[HighestPrioTask];//OSTCBCurָ��������ȼ�����
	OSTCBTbl[HighestPrioTask].TaskDelayMs=0;//��ʱ��0	
	if(HighestPrioTask==0) OS_System.RunOneRound=OS_TRUE;
}

/*---------------------------------�����л��ؼ�����(Page Down)---------------------------------------*/
void OSContextExchange(void) //�������л�(���񻷾��л�,�¸���������OSTCBCurָ�������)
{
	OS_INT_ENTER();
	/*Enter PendSV_Handler*/ 
  OSPendSVPulse();
	/*Enter SVC_Handler*/
  //OSSVCPulse();
	OS_INT_EXIT();
}

void OSScheduler_Process(void) //�����������ֻ���Ȳ��л���
{		 
	 if((OS_System.LockNesting == 0) && (OS_System.IntNesting == 0))
	 {			 
#if (OS_FAULT_ENABLE == 1)				 
RESCHEDULE:	
#endif					 
		 {	
#if (OS_SIGN_PP_ENABLE == 1)     					
			 OSFlagGroupHandler();//OSFlagGroup�й�
#endif
			 if(OS_System.RuningMode==0)//����ģʽ:Order	
			 {		 
				 /*******************************************������˳������*******************************************/
#if (OS_SIGN_PP_ENABLE == 1) 				 
				 OSMutexHandler_Order();//OSMutex�йغ���
#endif			
         if(OSTaskSwitchCheck_Order()==OS_FALSE) //�Ƿ�����ת�������	
				 {					 
				   OSRunModeProcess_Order();
				 }
			 }
			 else				
				{		
					/*******************************************����ʽ���ȼ�******************************************/	
#if (OS_SIGN_PP_ENABLE == 1)     					
					OSMutexHandler_Prio();//OSMutex�йغ���
#endif					
					OSRunModeProcess_Prio();
					OSTaskSwitchCheck_Prio();
				}		
		 }	

	 }	
#if (OS_FAULT_ENABLE == 1)	
	 OS_Fault.tcblast=OSTCBCur;	 
	 OS_Fault.tcbnext=OSTCBNext;	 
	 if(OS_FAULT_STK_CHECK(OSTCBCur)==OS_TRUE)
	 {
		 goto RESCHEDULE;
	 }
#endif  
}
/*---------------------------------�����л��ؼ�����(Page Up)---------------------------------------*/

void OSTaskTimeDelayCount_Process(void) //������ʱ��������
{
	INT16U TaskCount;	
	for(TaskCount = 1; TaskCount < OS_MAX_TASKS; TaskCount++) 
	 {   
#if (OS_FAULT_ENABLE == 1)		 
		 FSC_FAULT_SYSTEM_OS_Timer_Handler(TaskCount);
#endif		 
		 if(( OSTCBTbl[TaskCount].TaskDelayMs >1 )&&((OSTCBTbl[TaskCount].TaskFlagBit&(1<<_BIT_TimeLess))==0)) 
		 {
			 OSTCBTbl[TaskCount].TaskDelayMs--;
			 if(OSTCBTbl[TaskCount].TaskDelayMs==1) 
			 {
				 OSTCBTbl[TaskCount].TaskFlagBit|=_BIT_TimeOut;//�����ʱ���
				 OS_System.TaskDelayTimeOutFlag=OS_TRUE;
			 }
		 }
	 }	
}
void OS_SysTick_Handler(void) //�����л����ĺ���
{
	 if(OS_System.Running==OS_TRUE)//ϵͳ״̬���
	 {
		 OS_System.ClockCnt++;       //ϵͳʱ�ӽ��ļ������ۼ�
		 
     OSTaskTimeDelayCount_Process();//������ʱ��������
		 
#if (OS_TIMECOUNTSYS_ENABLE == 1)		 
		 OSRunTimeCount_Process();   //ϵͳ����ʱ��ͳ�Ƴ���
#endif			 
#if (OS_TIM_SYS_ENABLE == 1)
     OSTimerCount_Process();     //���ⶨʱ����������
#endif	
		 
		 OS_System.TimeSliceCnt++;   //ʱ����Ƭ������(��ʱ�ӽ���Ϊ��׼)
		 if((OS_System.TimeSliceCnt>=OSTCBCur->TaskTimeSlice) //ʱ����Ƭ����ж�(����һ��ʱ����Ƭ���ڶ��ٸ�ʱ�ӽ���)
			  ||(OS_System.TaskDelayTimeOutFlag==OS_TRUE)) //������ʱ��ɱ�־(ÿ����������ʱ���ʱ��������һ��)
		 { 
			 if(OS_System.RuningMode!=1)//�Ǵ����ȼ�����ģʽ�����
			 {
         OSSchedSwitch(); //���Ȳ��л����� 
				 if(OS_System.TaskDelayTimeOutFlag==OS_TRUE)
				 {
					 OS_System.TaskDelayTimeOutFlag=OS_FALSE;
					 if(OSTCBRun!=OSTCBCur)
					 {
						 OS_System.TimeSliceCnt=0; //ʱ����Ƭ��������0��׼�����¼���
					 }
				 }
				 else
				 {
					 OS_System.TimeSliceCnt=0; //ʱ����Ƭ��������0��׼�����¼���
				 }
			 }
#if (OS_FAULT_ENABLE == 1) 				 
			 OSTaskEndlessLoopsCheck_Process();//������������(ԭ������������������N������Ϊ����)
#endif			 	 
#if (OS_CMD_ALL_ENABLE == 1) 		 
       OSTaskCPUOccRateCount_Process();//����ռ����ͳ�Ƴ���
#endif   		 
		 }	 
	 }
	 else if(OS_System.Running==OS_FALSE)
	 {	
     OS_StopRun_Process();//ϵͳ�رճ���
	 }
}
/*---------------------------------------ϵͳ���ĺ�����Page Down)-------=-------------------------------------*/



/*----------------------------------------ϵͳ��ʱ������Page Up)--------=-------------------------------------*/

void delay_us(INT32U nus) //΢����ʱ���� (�û��ɵ���,�����ж���ʹ��)
{   	
	OSTIMER_INT_OFF();//�ر�ϵͳ��ʱ���ж�
	INT32U Ticks,TCntOld,TCntNow,TCnt=0;
	INT32U ReloadTimerValue=OSTIMER_LOAD;       
	Ticks=OSTIMER_CONT_1us*nus;   
	TCntOld=OSTIMER_VAL;
	while(1)
	{
			TCntNow=OSTIMER_VAL;  
			if(TCntNow!=TCntOld)
			{       
					if(TCntNow<TCntOld) TCnt += TCntOld-TCntNow;
					else TCnt += ReloadTimerValue-TCntNow+TCntOld;   
					TCntOld = TCntNow;
					if(TCnt>=Ticks) break;
			} 			
	} 
  if((OS_System.RunDriverCode==OS_FALSE)&&(OS_System.Running==OS_TRUE))
	{
		OSTIMER_INT_ON(); 	//��ϵͳ��ʱ���ж�
	}
}
void delay_ms(INT32U nms)  //������ʱ���� (�û��ɵ���,�ж��п���)
{     
	delay_us(nms*1000);
}
void OS_delayMs(volatile INT32U nms) //OS��ʱ����(�û��ɵ���,�ж��в�����)
{
	while((OS_System.IntNesting != 0) && (OS_System.LockNesting != 0));
	if(nms==0){nms=2;//��һ����1���ֵ����
	OSTCBCur->TaskFlagBit|=(1<<_BIT_TimeLess);} //��λ���޵ȴ���־
  else OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeLess);//ȡ�����޵ȴ���־	
	OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//ȡ����ʱ��ɱ�־
	OSTCBCur->TaskDelayMs=nms+1;
	OSSchedSwitch(); //�����л����� 
	while(OSTCBCur->TaskDelayMs);
}
void OS_delayDHMS(INT8U day,INT8U hour,INT8U min,INT8U sec)//����ʱ������ʱ��<=49��17ʱ2��47�� (�û��ɵ���,�ж��в�����)
{
	INT32U time;
	if(day>49) day=49;
	time=(day*24*3600+hour*3600+min*60+sec)*1000;
	OS_delayMs(time);
}
/*---------------------------------------ϵͳ��ʱ������Page Down)-------=-------------------------------------*/

/**************************************************������ˮ*****************************************************/
