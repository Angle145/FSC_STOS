/*--------------------------------------------版本: V4.9.7--------------------------------------------*/
#include "fsc_stos.h" 
char _FSC_STOS_Version[]={"V4.9.7"};//版本号

/*---------------------------------------全局变量和声明区(Page Up)--------------------------------------*/

/*--------------------------------------------系统区-----------------------------------------*/
#if (OS_SYSTIME_ENABLE== 1) //时间设置函数声明
extern u8 RTC_SetTimes(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//设置时间函数年月日时分秒(从外部引进)
#endif
/*-------------------------------------------------------------------------------------------*/
/*--------------------------------------------用户区-----------------------------------------*/


/*-------------------------------------------------------------------------------------------*/
/*---------------------------------------全局变量和声明区(Page Down)--------------------------------------*/


/*-------------------------------------自定义系统指令区(Page Up)------------------------------------------*/
/*使用方法：
1.自定义数组指令;
2.转到本页Cmd_Process()函数中增加相应的代码(在本页约560行)。
*/
/*+++++++++++++++++++系统缺省指令+++++++++++++*/

//或选中Cmd_Process右键goto Definition ""方式进入到Cmd_Process()函数。
#if (OS_CMD_ALL_ENABLE == 1)
#if (OS_CMD_NSY_ENABLE == 1)
/*----------------------------------全局指令----------------------------------*/
/*
*全局指令任何时刻都响应，目前本OS只有3条全局指令。
*除全局指令外，其他均为受限指令,在进入指令系统后才能响应。
*/
char cmd_help[]={"cmd/help//"};  //指令帮助
char cmd_enter[]={"cmd/enter//"};//进入指令系统
char cmd_exit[]={"cmd/exit//"};  //退出指令系统
/*--------------------------------------------------------------------------*/

/*----------------------------------系统指令----------------------------------*/
//1.系统无关：   （如任务超过5个，请依葫芦画瓢自行增加指令.（此处和Cmd_Process()函数里都需要增加））

/*---------------任务开关----------------*/
char cmd_task_open_01[]={"Task1/open//"};//任务1打开  只要保证指令以//结尾即可，如{"打开任务1//"}
char cmd_task_open_02[]={"Task2/open//"};//任务2打开 
char cmd_task_open_03[]={"Task3/open//"};//任务3打开 
char cmd_task_open_04[]={"Task4/open//"};//任务4打开 
char cmd_task_open_05[]={"Task5/open//"};//任务5打开 

char cmd_task_close_01[]={"Task1/close//"};//任务1关闭
char cmd_task_close_02[]={"Task2/close//"};//任务2关闭
char cmd_task_close_03[]={"Task3/close//"};//任务3关闭
char cmd_task_close_04[]={"Task4/close//"};//任务4关闭
char cmd_task_close_05[]={"Task5/close//"};//任务5关闭
/*-------------------------------------------*/
/*-----------------任务优先级-----------------*/
char cmd_task_prio_01[]={"Task1/prio=%d//"};//%d表示整型数字,例：cmd/Task1/prio=8//
char cmd_task_prio_02[]={"Task2/prio=%d//"};
char cmd_task_prio_03[]={"Task3/prio=%d//"};
char cmd_task_prio_04[]={"Task4/prio=%d//"};
char cmd_task_prio_05[]={"Task5/prio=%d//"};
/*-------------------------------------------*/
/*---------------任务时间切片-----------------*/
char cmd_task_timeslice_01[]={"Task1/timeslice=%d//"}; //设置任务1时间切片时间(一个任务连续运行的时间)
char cmd_task_timeslice_02[]={"Task2/timeslice=%d//"}; //
char cmd_task_timeslice_03[]={"Task3/timeslice=%d//"}; //
char cmd_task_timeslice_04[]={"Task4/timeslice=%d//"}; //
char cmd_task_timeslice_05[]={"Task5/timeslice=%d//"}; //
/*-------------------------------------------*/
#if (OS_SYSTIME_ENABLE ==1 )
/*----------------设置系统时间----------------*/
char cmd_systime_set[]={"setsystime=%d/%d/%d/%d.%d.%d//"};//年/月/日/时.分.秒
/*------------------------------------------*/
#endif
#endif
//2.系统有关：
/*------------------------------------------*/
char cmd_osinformation[]={"osinformation//"};//查看系统状态
char cmd_cpu_occrate_max_information[]={"cpu_occrate_max_information//"};
char cmd_cpu_user_occrate_max_information[]={"cpu_user_occrate_max_information//"};
char cmd_runmode_order[]={"runmode/order//"};//时间片运行模式
char cmd_runmode_prio[]={"runmode/prio//"};  //优先级运行模式
char cmd_runmode_order_prio[]={"runmode/order+prio//"};//时间片+优先级运行模式
char cmd_oson[]={"oson//"};                  //系统关闭
char cmd_osoff[]={"osoff//"};                //系统打开
char cmd_hardreset[]={"hardreset//"};        //硬件重启
char cmd_osoff_delaytime_set[]={"setosofftime=%d/%d/%d/%d.%d.%d//"};//年/月/日/时.分.秒  设置系统关闭时间
/*------------------------------------------*/

/*+++++++++++++++++++用户自定义指令+++++++++++++*/
//1.全局指令：


//2.受限指令： 


//3.定义好指令后请转到Cmd_Process()函数里添加响应代码
/*--------------------------------------------*/
#endif
/*------------------------------------自定义系统指令区(Page Down)-----------------------------------------*/






/*------------------------------------移植硬件有关(Page Up)----------------------------------------------*/
/*1.系统时钟节拍*/
/*默认使用SysTick定时器   
*SysTick->CTRL: bit0-定时器使能 bit1-中断使能 bit2-时钟源选择(=1系统主频，=0系统主频/8)  bit16-计数溢出位*/

#ifndef SystemCoreClock
#define SystemCoreClock 72000000  //如为其他主频，请设置好对应的主频(如果相关文件已定义则无需定义)
#endif
#define OSTIMER_OFF()      SysTick->CTRL&=~(1<<0)     //关闭系统定时器中断
#define OSTIMER_ON()       SysTick->CTRL|=(1<<0)      //打开系统定时器中断
#define OSTIMER_INT_OFF()  SysTick->CTRL&=~(1<<1)     //关闭系统定时器中断
#define OSTIMER_INT_ON()   SysTick->CTRL|=(1<<1)      //打开系统定时器中断
#define OSTIMER_CONT_1us   (SystemCoreClock/8/1000000)//系统定时器1us计数次数 
#define OSTIMER_LOAD       SysTick->LOAD              //系统定时器重装载值寄存器
#define OSTIMER_VAL        SysTick->VAL               //系统定时器当前值寄存器
void OSSysTickInit(void)//用作OS的计时脉冲
{ 
	//Systick定时器初使化(使用其他定时器时，请修改为其他定时器)
	char * Systick_priority = (char *)0xe000ed23;       //Systick中断优先级寄存器
	SysTick->LOAD  = OSTIMER_CONT_1us* 1000;            //Systick定时器重装载计数值:1ms(固定值)
	*Systick_priority = 0x00;                           //Systick定时器中断优先级
	SysTick->VAL   = 0;                                 //Systick定时器计数器清0
	SysTick->CTRL = 0x3;//Systick打开并使能中断且使用外部晶振时钟,8分频  72MHz/8=9MHz  计数9000次=1ms  计数9次=1us
}
void SysTick_Handler(void) //Systick定时器中断函数(使用其他定时器时,请修改为其他定时器的中断函数名)
{
  OS_SysTick_Handler();
}
/*----------------------------------------------------------------------------------*/
/*2.任务调度*/
/*方式一：可悬挂中断PendSV*/
void OSPendSVInit(void)//PendSV初使化
{
	char* NVIC_SYSPRI14= (char *)0xE000ED22;  //优先级寄存器地址
	*NVIC_SYSPRI14=0xFF;//设置PendSV中断优先级最低 
}
int* NVIC_INT_CTRL= (int *)0xE000ED04;  //中断控制寄存器地址
void OSPendSVPulse(void)//触发PendSV中断
{
	OS_INT_ENTER();
  *NVIC_INT_CTRL=0x10000000; 
	OS_INT_EXIT();	
}
/*方式二：软件中断SVC*/
/*---------------------------------------------------------------------------------*/
void HardReset(void) //硬件重启
{
	__disable_fault_irq();
  NVIC_SystemReset();
}
/*-----------------------------------移植硬件有关(Page Down)---------------------------------------------*/






/*--------------------------------------多任务刷uCGUI支持(Page Up)----------------------------------------------*/
/*-------uCGUI支持操作系统需要添加的额外4个函数-------*/
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
/*-------------------------------------------uCGUI支持(Page Down)----------------------------------------------*/




/*-------------------------------------------内存管理(Page Up)----------------------------------------------*/
/*
内存管理设计：
将内存池视为一个线性区块，区块的开头4byte(32bit)*4=16byte用于记录该区块的信息,其中第二个数据用于标志该区块是否空闲，如下图：
Block:[byte0][byte1][byte2][byte3][byte4]--[byte7][byte8]--[byte11][byte12]--[byte15][byte16]--[byteN-2][byteN-1][byteN]
      {        4byte(32bit)      }{ 4byte(32bit) }{  4byte(32bit) }{  4byte(32bit)  }{  address:(N-16)byte(=struct+1)  }
                 size(N)            1(==1空闲标志)        last             next                     data
一、申请内存时，从内存池开始地址开始查找空闲内存区块，如查找到的空闲内存区块size大于所申请的内存size，则分割出所申请内存大小内存区块(从前面开始分)，
           分割出去的内存空间=区块信息长度+申请内存大小。如申请4*64byte=256byte内存，则分割出去的内存=16+256=272。例：

1.第一次申请4*64byte内存，分割出去的内存=16+256=272，得到的内存区块：
						[byte0][byte1][byte2][byte3][byte4]--[byte7][byte8]--[byte11][byte12]--[byte15][byte16]         --       [byte271]
						{        4byte(32bit)      }{ 4byte(32bit) }{  4byte(32bit) }{  4byte(32bit)  }{  address:(N-12)byte(=struct+1)  }
												256                0(==0使用标志)       last             next                      data
剩余内存区块：[byte272]--[byte275][byte276]--[byte279][byte280]--[byte283][byte284]--[byte287][byte288]   --   [byteN-(16+256)]
            {   4byte(32bit)   }{   4byte(32bit)    }{    4byte(32bit) } {  4byte(32bit)  }{  address:(N-12)byte(=struct+1)  }
             size(N-(16+256))       1(==1空闲标志)            last               next                      data

2.第二次申请4*32byte内存，分割出去的内存=16+128=144，得到的内存区块：
						[byte272]--[byte275][byte276]--[byte279][byte280]--[byte283][byte284]--[byte287][byte288]         --       [byte415]
						{   4byte(32bit)    }{   4byte(32bit)   }{    4byte(32bit) }{   4byte(32bit)   }{   address:(N-16)byte(=struct+1)  }
								   256              0(==0使用标志)            last               next                     data
剩余内存区块：[byte416]--[byte419][byte420]--[byte423][byte424]--[byte427][byte428]--[byte431][byte432]--[byteN-(16+256)-(16+128)]
            {   4byte(32bit)   }{   4byte(32bit)    }{    4byte(32bit)  }{   4byte(32bit)   }{  address:(N-16)byte(=struct+1)  }
       size(N-(16+256)-(16+128))    1(==1空闲标志)            last                next                       data
			 
二、内存释放时，只是将该内存区块的标志位置1(空闲)，并没有整理把该区域与所有空闲区块合并在一起。释放内存后再申请内存容易造成内存碎片。
              OS有提供专门的内存碎片整理函数。

三、由于没有对内存池进行事先分区，当申请的内存过多时，申请的速度会越来越慢，不过对单片机小容量内存池来说影响不大，如果内存池比较大
             则必须事先对内存池进行分区，这样申请内存时高效快捷，也易于内存管理。
*/        
struct memblock{  
    INT32U size;                  //内存区块大小  
    INT32U free;                  //是否已使用 
    struct memblock *last;        //指向上一个内存区块	
    struct memblock *next;        //指向下一个内存区块  
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
    memblockList->size=OS_MEMORYPOOL_SIZE-sizeof(struct memblock);//可用内存空间大小  
    memblockList->free=1;                                      //1：内存空闲 0：内存已使用 
    memblockList->last=NULL;                                   //指向空内存 	
    memblockList->next=NULL;                                   //指向空内存  
}

void split(struct memblock *fitting_slot,size_t size)//内存分割(从大区块分割出一部分作为申请到的内存，实质是把空闲信息结构体往后移动,再将前面的信息结构体的标志改为使用)
{
    struct memblock *memnew=(void*)((INT8U*)fitting_slot+size+sizeof(struct memblock));  //重新定位剩余空闲的地址
    memnew->size=(fitting_slot->size)-size-sizeof(struct memblock);                   //重新定义剩余空闲size大小
    memnew->free=1;                                                        //设置空闲
    memnew->last=fitting_slot;
   	memnew->next=fitting_slot->next;                                       //插入到fitting_slot与fitting_slot->next中间
    fitting_slot->size=size;
    fitting_slot->free=0;
    fitting_slot->next=memnew;                                             //插入到fitting_slot与fitting_slot->next中间
}
void *FSC_Malloc(size_t noOfBytes)//内存申请(每个内存块的开始12个字节用来保存该块内存使用信息的结构体数据)
{   
	  struct memblock *curr;
    void *result;
    if(!(memblockList->size))
    {
        FSC_MemoryInit();
    }
    curr=memblockList;
    while((((curr->size)<noOfBytes)||((curr->free)==0))&&(curr->next!=NULL))//从内存池开始地址查找空闲内存块
    {
        curr=curr->next;
    }
    if((curr->size)==noOfBytes) //剩余内存恰好相等
    {
        curr->free=0;
        result=(void*)(++curr);//返回指向数据(data区)起始地址
    }
    else if((curr->size)>(noOfBytes+sizeof(struct memblock)))         //剩余内存充足
    {
			  OS_System.MemFragFlag=OS_TRUE;
        split(curr,noOfBytes);                                        //分割所需的内存出去
        result=(void*)(++curr);                                       //返回指向数据(data区)起始地址
    }
    else  //剩余内存不足
    {
        result=NULL;
    }
		return result;
}

void FSC_Merge(void)//合并相邻空闲内存区块(小优化)
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
void FSC_Free(void* ptr)//释放内存(输入形参地址必须是FSC_Malloc函数返回的地址)
{   
    if(((void*)memorypool<=ptr)&&(ptr<=(void*)(memorypool+OS_MEMORYPOOL_SIZE)))//检查要释放的地址是否在内存池范围内
    {
        struct memblock* curr=ptr;
        --curr;   //从data区回退到区块信息区
        curr->free=1;//置空闲
        FSC_Merge();//合并相邻空闲区块(小优化，大优化需要内存碎片整理)
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
void FSC_FragMerge(void)//内存碎片整理(只能在空闲任务中使用，在其他地方调用会使系统崩溃。处理耗时非常久，请在足够空闲时使用)
{
	INT64U i,devi,len=0;
	struct memblock *curr=memblockList;
	struct memblock *le=memblockList;
	struct memblock* mbk;
	OSMEM *ptr_start;
	
	FSC_Merge();//合并相邻空闲内存块
	
	OS_INT_ENTER();
	OSSchedLock();
	while(curr->next!=NULL) //查找除最后一个内存块的所有内存块
	{
		if(curr->free)//查找空闲内存块
		{		
			le=memblockList;
			while(le!=curr)//统计内存池首地址到当前内存块的内存长度
			{
				 len+=le->size+sizeof(len);
				 le=le->next;
			}
			len=OS_MEMORYPOOL_SIZE-len;//计算剩余长度
			mbk=curr;//备份内存块信息
			curr->next->last=curr->last;
			(*curr)=(*(curr->next));		
			ptr_start=(void*)(++curr);
			devi=mbk->size+sizeof(mbk);
			for(i=0;i<len;i++)   //复制(移动)数据区(耗时非常久)
			{
				ptr_start[i]=ptr_start[i+devi];
			}
			le=curr;
			mbk=le;//备份内存块信息
			le->next=le->next-devi;
			le=mbk->next;
			while(le!=NULL)
			{
				mbk=le;//备份内存块信息
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
/*-------------------------------------------内存管理(Page Down)----------------------------------------------*/





/******************************************系统有关参数(Page Up)*******************************************/

OSSTK TASK_IDLE_STK[TaskIDLE_StkSize];    //空闲任务堆栈
OSSTK TASK_MANAGER_STK[TaskManager_StkSize];//任务管理器任务堆栈

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

void OSDispMsg(void){}  //空函数
/*********************************************有关参数(Page Down)**********************************************/	
	
	
	
	
	
/*------------------------------------------系统指令模块(Page Up)-----------------------------------------------*/
#if (OS_CMD_ALL_ENABLE == 1)	
	
OS_Information_Backup_Struct CPUOccRateMax;	//CPU占用率最大时的信息
OS_Information_Backup_Struct UserCPUOccRateMax;	//用户任务CPU占用率最大时的信息	
	
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
#define OSDisp_osoff_delaytime()     OSCMDprintf("\r\n OS ShutDown Time: %d年%d月%d日%d时%d分%d秒\r\n\r\n",\
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
void OS_CmdHelp_Process(void)//指令帮助显示程序(用户不可用)
{
	OSCMDprintf("=====================================================\r\n");
  OSCMDprintf("------------------FSC Cmd Help System----------------\r\n");
	OSCMDprintf("%s            指令帮助\r\n",cmd_help);
	OSCMDprintf("%s           打开指令系统\r\n",cmd_enter);
	OSCMDprintf("%s            关闭指令系统\r\n",cmd_exit);
	OSCMDprintf("----------------------指令系统指令-------------------\r\n");	
	OSCMDprintf("*系统有关指令：\r\n");
	OSCMDprintf("%s       系统状态信息\r\n",cmd_osinformation);
	OSCMDprintf("%s       顺序运行模式\r\n",cmd_runmode_order);
	OSCMDprintf("%s        优先级运行模式\r\n",cmd_runmode_prio);
	OSCMDprintf("%s  混合运行模式\r\n",cmd_runmode_order_prio);
	OSCMDprintf("%s                系统打开\r\n",cmd_oson);
	OSCMDprintf("%s               系统关闭\r\n",cmd_osoff);
	OSCMDprintf("%s           硬件重启\r\n",cmd_hardreset);
  OSCMDprintf("%s 设置系统关闭倒计时时间(年/月/日/时.分.秒)\r\n",cmd_osoff_delaytime_set);
	OSCMDprintf("-----------------------------------------------------\r\n");
	OSCMDprintf("*系统无关指令：\r\n");
	OSCMDprintf("任务名/open//         任务打开\r\n");
	OSCMDprintf("任务名/close//        任务关闭\r\n");
	OSCMDprintf("任务名/prio=%%d//      任务优先级设置为%%d(代表数字)\r\n");
	OSCMDprintf("任务名/timeslice=%%d// 任务时间片设置为%%d(代表数字)\r\n");
	OSCMDprintf("=====================================================\r\n");
}
void Cmd_RxBuff_Clean(void)//指令缓存清除
{
  INT8U i;
	for(i=0;i<strlen(OS_Cmd.RX_BUFF);i++)
	{
	  OS_Cmd.RX_BUFF[i]=0;
	}
}
void Cmd_Process(void)//系统指令处理函数  (用户可用，在此函数增减自定义指令)
{
/*
使用方法：
1.不带数字参数的指令： if(CompareCmd(自定义指令数组名)==0) {  对应的功能代码或函数  }	
2.带数字参数的指令 ：  if(CompareCmdDig('=',自定义指令数组名)==0) {  对应的功能代码或函数  + 参数=GrabCmdDig(n)  }
  说明：
       '='号后面为数字参数，自定义指令中必须包含字符'='(也可为其他符号)  	
	     GrabCmdDig(n)函数返回指令中的第n个数字，只要是数字都符合。
	     如 char cmd_lde0_time[]={"cmd/led0/time=%d//"};
	        if(CompareCmdDig('=',cmd_lde0_time)==0) { led0=1; delay(GrabCmdDig(1)); led=0; }
	        发送 cmd/led0/time=20//时，  GrabCmdDig(0)==0，GrabCmdDig(1)==20，明显，这里需要的是GrabCmdDig(1)。
*/
		if(OS_Cmd.RXOK_Flag==OS_TRUE)
		{	
			OSCMDprintf("%s\r\n",OS_Cmd.RX_BUFF);
			//系统全局指令:
			if(CompareCmd(cmd_help)==0)		{ OS_CmdHelp_Process(); }			
			if(CompareCmd(cmd_enter)==0)  { OS_Cmd.Enter_Flag=OS_TRUE; }		
      if(CompareCmd(cmd_exit)==0)   { OS_Cmd.Enter_Flag=OS_FALSE; }	
			/**************************************用户自定义全局指令区***********************************/
			//用户全局指令: 
			

			/*******************************************************************************************/			
      if(OS_Cmd.Enter_Flag==OS_TRUE)
			{						
	#if (OS_CMD_NSY_ENABLE == 1)
				/**************************************用户自定义指令区*************************************/
				//用户受限指令:
							

				/*****************************************************************************************/
				
				//系统受限指令:
				/********************************************任务打开****************************************/	//任务1对应TCB[2]		
				if(CompareCmd(cmd_task_open_01)==0) { OSTaskStateSet((void*)OSTCBTbl[2].TaskAdd,TASK_RUNNING);OSDisp_task1open(); }                     
				if(CompareCmd(cmd_task_open_02)==0) { OSTaskStateSet((void*)OSTCBTbl[3].TaskAdd,TASK_RUNNING);OSDisp_task2open(); }																		
				if(CompareCmd(cmd_task_open_03)==0) { OSTaskStateSet((void*)OSTCBTbl[4].TaskAdd,TASK_RUNNING);OSDisp_task3open(); }                                    
				if(CompareCmd(cmd_task_open_04)==0) { OSTaskStateSet((void*)OSTCBTbl[5].TaskAdd,TASK_RUNNING);OSDisp_task4open(); }	                                    
				if(CompareCmd(cmd_task_open_05)==0) { OSTaskStateSet((void*)OSTCBTbl[6].TaskAdd,TASK_RUNNING);OSDisp_task5open(); }			                                  
				//如任务超过5个请自行增加
				
				/*******************************************************************************************/	
				/********************************************任务关闭****************************************/	
				if(CompareCmd(cmd_task_close_01)==0) { OSTaskStateSet((void*)OSTCBTbl[2].TaskAdd,TASK_PAUSING);OSDisp_task1close(); }
				if(CompareCmd(cmd_task_close_02)==0) { OSTaskStateSet((void*)OSTCBTbl[3].TaskAdd,TASK_PAUSING);OSDisp_task2close(); }
				if(CompareCmd(cmd_task_close_03)==0) { OSTaskStateSet((void*)OSTCBTbl[4].TaskAdd,TASK_PAUSING);OSDisp_task3close(); }
				if(CompareCmd(cmd_task_close_04)==0) { OSTaskStateSet((void*)OSTCBTbl[5].TaskAdd,TASK_PAUSING);OSDisp_task4close(); }
				if(CompareCmd(cmd_task_close_05)==0) { OSTaskStateSet((void*)OSTCBTbl[6].TaskAdd,TASK_PAUSING);OSDisp_task5close(); }
				//如任务超过5个请自行增加
				
				/*******************************************************************************************/		
				/*******************************************任务优先级***************************************/	
				if(CompareCmdDig('=',cmd_task_prio_01)==0) { OSTaskPrioSet((void*)OSTCBTbl[2].TaskAdd,GrabCmdDig(1));OSTask1PrioSet(); }	                                                                            
				if(CompareCmdDig('=',cmd_task_prio_02)==0) { OSTaskPrioSet((void*)OSTCBTbl[3].TaskAdd,GrabCmdDig(1));OSTask2PrioSet(); }		                                       
				if(CompareCmdDig('=',cmd_task_prio_03)==0) { OSTaskPrioSet((void*)OSTCBTbl[4].TaskAdd,GrabCmdDig(1));OSTask3PrioSet(); }	                                       
				if(CompareCmdDig('=',cmd_task_prio_04)==0) { OSTaskPrioSet((void*)OSTCBTbl[5].TaskAdd,GrabCmdDig(1));OSTask4PrioSet(); }	                                      
				if(CompareCmdDig('=',cmd_task_prio_05)==0) { OSTaskPrioSet((void*)OSTCBTbl[6].TaskAdd,GrabCmdDig(1));OSTask5PrioSet(); }	                                       
				//如任务超过5个请自行增加
				/*******************************************************************************************/		
				/*******************************************任务时间片***************************************/	
				if(CompareCmdDig('=',cmd_task_timeslice_01)==0){ OSTCBTbl[2].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task1TimeSlice();}
				if(CompareCmdDig('=',cmd_task_timeslice_02)==0){ OSTCBTbl[3].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task2TimeSlice();}
				if(CompareCmdDig('=',cmd_task_timeslice_03)==0){ OSTCBTbl[4].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task3TimeSlice();}
				if(CompareCmdDig('=',cmd_task_timeslice_04)==0){ OSTCBTbl[5].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task4TimeSlice();}
				if(CompareCmdDig('=',cmd_task_timeslice_05)==0){ OSTCBTbl[6].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task5TimeSlice();}
				//如任务超过5个请自行增加
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
				/*************************************系统有关指令*******************************************/	
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
void OS_Information_Backup(OS_Information_Backup_Struct* Struct)//备份系统信息 
{
	INT16U TaskCount,i;
	//保存:
	
	//运行模式
  Struct->RuningMode=OS_System.RuningMode;
	
	//运行时间
	Struct->RunTime[0]=OS_System.RunTime_year;
	Struct->RunTime[1]=OS_System.RunTime_mouth;
	Struct->RunTime[2]=OS_System.RunTime_day;
	Struct->RunTime[3]=OS_System.RunTime_hour;
	Struct->RunTime[4]=OS_System.RunTime_minute;
	Struct->RunTime[5]=OS_System.RunTime_second;
	Struct->RunTime[6]=OS_System.RunTime_ms; 
	
	//时钟节拍
	Struct->Clock=OSTIMER_LOAD/OSTIMER_CONT_1us/1000;
	
	//系统关闭倒计时时间
	Struct->ShutDownTime[0]=OS_System.ShutDownTime_year;
	Struct->ShutDownTime[1]=OS_System.ShutDownTime_mouth;
	Struct->ShutDownTime[2]=OS_System.ShutDownTime_day;
	Struct->ShutDownTime[3]=OS_System.ShutDownTime_hour;
	Struct->ShutDownTime[4]=OS_System.ShutDownTime_minute;
	Struct->ShutDownTime[5]=OS_System.ShutDownTime_second;
	if(Struct->ShutDownTime[5]>0) Struct->ShutDownTime[5]-=1;  
	
	//CPU占用信息  整形表示浮点型，一位小数
	Struct->CPUOccRate[0]=1000-(OSTCBTbl[0].TaskCPUOccRateCnt*1000.0/OS_System.TaskTimeSliceCnt);
	Struct->CPUOccRate[1]=OS_System.TaskCPUOccRateMax;
	Struct->CPUOccRate[2]=OS_System.UserTaskCPUOccRateMax;
	
	//内存信息
	Struct->Memory[0]=OS_MEMORYPOOL_SIZE;
	Struct->Memory[1]=OS_MEMORYPOOL_SIZE-FSC_MemoryFreeSizeGet();
	Struct->Memory[2]=FSC_MemoryFreeSizeGet();
	
	//任务信息
	for(TaskCount = 0; TaskCount < OS_MAX_TASKS; TaskCount++) 
	{
		if(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)
		{	
			Struct->TaskMsg[TaskCount][0]=OSTCBTbl[TaskCount].TaskCPUOccRateCnt*1000/OS_System.TaskTimeSliceCnt;//整形表示浮点型，一位小数
			Struct->TaskMsg[TaskCount][1]=OSTCBTbl[TaskCount].TaskCPUOccRateCnt*1000/OS_System.ClockCnt;
			Struct->TaskMsg[TaskCount][2]=OSTCBTbl[TaskCount].StkUsed;
			Struct->TaskMsg[TaskCount][3]=OSTCBTbl[TaskCount].StkFree;
			Struct->TaskMsg[TaskCount][4]=OSTCBTbl[TaskCount].StkUsed*1000/OSTCBTbl[TaskCount].StkSize;//整形表示浮点型，一位小数
			Struct->TaskMsg[TaskCount][5]=OSTCBTbl[TaskCount].TaskTimeSlice;
			Struct->TaskMsg[TaskCount][6]=OSTCBTbl[TaskCount].TaskPrio;
			Struct->TaskMsg[TaskCount][7]=OSTCBTbl[TaskCount].TaskState;
			//任务名
			for(i = 0; i < TASK_NAME_LEN/2; i++) 
			{
			  Struct->TaskStr[TaskCount][i]=OSTCBTbl[TaskCount].TaskStr[i*2];	
				Struct->TaskStr[TaskCount][i]|=OSTCBTbl[TaskCount].TaskStr[i*2+1]<<8;	
			}				
		}
	}	
}
void OS_Information_Backup_Process(OS_Information_Backup_Struct* Struct)//系统备份信息显示
{
#if (OS_CMD_DISP_SYS_ENABLE == 1)		
#endif	
	INT16U TaskCount;
#if (OS_CMD_DISP_SYS_ENABLE == 1)	
	char *pf;
#endif	

#if (OS_CMD_DISP_SYS_ENABLE == 1)
  OSCMDprintf("+++++++++++++++++++++++++++++++++++系统状态信息++++++++++++++++++++++++++++++++++++\r\n");
	OSCMDprintf("===================================================================================\r\n");
	switch(Struct->RuningMode)
	{
	  case 0:pf="Order     "; break;
		case 1:pf="Prio      "; break;
		case 2:pf="Order+Prio";break;
	}
  OSCMDprintf("  运行模式 ：%s                     运行时间：%d年%d月%d日 %d时%d分%d秒 %dms\r\n",
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
	OSCMDprintf("  时钟节拍 ：%-3dms                          系统关闭倒计时：%d年%d月%d日 %d时%d分%d秒\r\n",
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
	OSCMDprintf("  CPU占用率：%04.1f%%                          最大占用率：系统 %04.1f%%   用户 %04.1f%%\r\r\n",
																															Struct->CPUOccRate[0]/10.0,
																															Struct->CPUOccRate[1]/10.0,
																															Struct->CPUOccRate[2]/10.0);
	OSCMDprintf("  内存容量 ：%d Byte\r\r\n",Struct->Memory[0]);      
	OSCMDprintf("  已用内存 ：%d Byte                      剩余内存：%d Byte \r\r\n",Struct->Memory[1],Struct->Memory[2]);
	OSCMDprintf("-----------------------------------------------------------------------------------\r\n");
	OSCMDprintf("利用率   活跃度   使用栈   空闲栈   百分比   时间片    优先级    状态    任务名    \r\n");
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

	 OS_TaskStackCounter_Process(); //任务堆栈统计
	
#if (OS_CMD_DISP_SYS_ENABLE == 1)
  OSCMDprintf("+++++++++++++++++++++++++++++++++++系统状态信息++++++++++++++++++++++++++++++++++++\r\n");
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
  OSCMDprintf("  运行模式 ：%s                     运行时间：%d年%d月%d日 %d时%d分%d秒 %dms\r\n",
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
	OSCMDprintf("  时钟节拍 ：%-3dms                          系统关闭倒计时：%d年%d月%d日 %d时%d分%d秒\r\n",
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
	OSCMDprintf("  CPU占用率：%04.1f%%                          最大占用率：系统 %04.1f%%   用户 %04.1f%%\r\r\n",
	                               100-(OSTCBTbl[0].TaskCPUOccRateCnt*100.0/OS_System.TaskTimeSliceCnt),
								                 OS_System.TaskCPUOccRateMax/10.0,
																 OS_System.UserTaskCPUOccRateMax/10.0);
	OSCMDprintf("  内存容量 ：%d Byte\r\r\n",OS_MEMORYPOOL_SIZE);      
	OSCMDprintf("  已用内存 ：%d Byte                      剩余内存：%d Byte \r\r\n",
	                               OS_MEMORYPOOL_SIZE-FSC_MemoryFreeSizeGet(),FSC_MemoryFreeSizeGet());
	OSCMDprintf("-----------------------------------------------------------------------------------\r\n");
	OSCMDprintf("利用率   活跃度   使用栈   空闲栈   百分比   时间片    优先级    状态    任务名    \r\n");
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
void OSTaskCPUOccRateCount_Process(void) //任务占用率统计程序
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
		 OS_System.TaskTimeSliceCnt=1;//从1开始计数(0不能作除数)		 
	 }  
}
INT32U OS_GetStringLength(char* p)//获取字符串p的长度
{
  INT32U i=0;
	while(*p!='\0')
	{
	  i++;
		p++;
	}
	return i;
}
INT8U CompareCmd(char* p)//比较字符串OS_Perip.OS_USART_RX_BUFF和字符串p内容是否一致
{
  return strncmp(OS_Cmd.RX_BUFF,p,OS_GetStringLength(p));
}

INT32U OS_GetStringLengthWithChar(char* p,char ch)//获取字符ch在字符串p中的位置
{
  INT32U i=0;
	while((*p!=ch)&&(*p!='\0'))
	{
	  i++;
		p++;
	}
	return i;
}
INT32* OS_GetStringNum(char* p)//从字符串p中提取数字保存到全局变量CmdNumBuff[]数组中
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
INT8U CompareCmdDig(char ch,char* p)//比较字符串OS_Perip.OS_USART_RX_BUFF和字符串p中，字符ch以前的内容是否一致
{
  return strncmp(OS_Cmd.RX_BUFF,p,OS_GetStringLengthWithChar(p,ch));
}
INT32 GrabCmdDig(INT8U n)//获取字符串p中的第n个数字(n=0开始)
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
						OSSchedSwitch();  //主动调度
					}
				}
			}
		else { OS_Cmd.RX_COUNT=0; OS_Cmd.RX_BUFF[OS_Cmd.RX_COUNT]=RxByte; }
	}
#else
  RxByte=RxByte;//防止警告
#endif	
}
/*------------------------------------------系统指令模块(Page Down)---------------------------------------------*/


/*-------------------------------------------系统函数模块(Page Up)----------------------------------------------*/
void OS_StopRun_Process(void)
{	
	 OSTCBCur = &OSTCBTbl[1];//指向任务管理器任务
	 OSTCBCur->TaskDelayMs=0;//取消等待
	 OS_System.IntNesting = 0;//为下次启动作初使化准备
	 OS_System.LockNesting= 0;//为下次启动作初使化准备
	 OSContextExchange();//上下文切换 		 
	 OSTIMER_OFF();//关闭ostimer定时器
}
/*--------------------------------------系统使能函数--------------------------------*/
void OSStopRun(void)//系统停止运行(停留在当前任务)(进入裸机模式，当前任务函数等同于main函数)
{
	OS_INT_ENTER();
  OSTIMER_OFF();	
	OS_INT_EXIT();
}
void OSStartRun(void)//系统开始运行(进入OS模式)
{
	OS_INT_ENTER();
  OSTIMER_ON();
	OS_INT_EXIT();
}
void OSSleep(void)//系统休眠
{
  OS_INT_ENTER();
  OS_System.Running=OS_FALSE;
	OS_INT_EXIT();
}
void OSAwaken(void)//系统唤醒
{
  OS_INT_ENTER();
  OS_System.Running=OS_TRUE;
	OSTIMER_ON();
	OS_INT_EXIT();
}
/*-----------------------------------------系统函数模块(Page Down)--------------------------------------------*/

/*------------------------------------------驱动保护函数(Page Up)---------------------------------------------*/
/*******************************************************
*函数名  ：OSEnterDriver  、    OSExitDriver
*使用方法：在驱动函数开头调用 、  在驱动函数结尾调用
*作用    :屏蔽OS对驱动的影响
*注意    :驱动程序里只能使用delay_us()、delay_ms()来延时，
         除此之外不能使用任何与OS有关的其他资源
*******************************************************/
void OSEnterDriver(void)//进入驱动程序 调用
{
	OS_INT_ENTER();
  OSTIMER_INT_OFF();
	OS_INT_EXIT();
}
void OSExitDriver(void)//离开驱动程序 调用
{
	OS_INT_ENTER();
  OSTIMER_INT_ON();
	OS_INT_EXIT();
}
/*-----------------------------------------驱动保护函数(Page Down)--------------------------------------------*/

/*-------------------------------------FSC智能纠错系统函数（Page Up）------------------------------------------*/
#if (OS_FAULT_ENABLE == 1)
void OSCtxSwTo(INT16U tasknum)
{
	OS_INT_ENTER();
	OSTCBCur=&OSTCBTbl[tasknum];
	OSPendSVPulse();
	OS_INT_EXIT();
}
void OSTaskEndlessLoopsCheck_Process(void) //任务陷入死循环检测,任务连续不间断运行N次则认为陷入死循环
{
  if(OSTCBCur->TaskNum>1)	//从用户任务开始检测
  {
	  if(OS_Fault.runLastTask==OSTCBCur->TaskNum)	
	  {
			OS_System.TaskContinuRunTimeCnt++;		
			if(OS_System.TaskContinuRunTimeCnt>OS_EndlessLoops_Times) 
			{
				OSTCBCur->TaskState=TASK_PAUSING;
				OS_System.LockNesting=0;			 
#if (OS_FAULT_DISP_ENABLE == 1)					 
				OSprintf("FSC提示：任务%s卡死，暂停运行！\r\n",OSTCBCur->TaskStr); 
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
			 OSprintf("系统重启倒计时：   %dS\r\n",OS_Fault.osresetimecnt/div);
		 }
	   if(OS_Fault.osresetimecnt>0) OS_Fault.osresetimecnt--;
		 else 
		 {
		   OS_System.Running=OS_TRUE;
			 OSprintf("------------系统启动------------\r\n");
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
			 OSprintf("系统关闭倒计时：   %dS\r\n",OS_Fault.osresetimecnt/div);
		 }
	   if(OS_Fault.osresetimecnt>0) OS_Fault.osresetimecnt--;
		 else 
		 {
		   OS_System.Running=OS_TRUE;
			 OSprintf("------------系统关闭------------\r\n");
		 }
	 }
   while(1);
}
void HardFault_Handler(void)
{
	OSprintf("FSC提示：系统奔溃,错误信息及系统状态：\r\n");
	OSprintf("\r\nHardFault_Handler:\r\n");
	OSprintf("{\r\n");
  OSprintf(" %s error! \r\n",OS_Fault.tcb->TaskStr);
	OSprintf(" %s running last! \r\n",OS_Fault.tcblast->TaskStr);
	OSprintf(" %s running next! \r\n",OS_Fault.tcbnext->TaskStr);
	OSprintf("}\r\n");
	OS_Information_Process();
	OSprintf("\r\n\r\n");
#if (OSFAULT_Reset_Enable	== 1)
	OSprintf("-----------系统将在%d秒后重新启动----------- \r\n\r\n",OS_Reset_Delay_Time);
  OS_RESET_DELAY(1000000,OS_Reset_Delay_Time);
#else
	OSprintf("-----------系统将在%d秒后关闭----------- \r\n\r\n",OS_Reset_Delay_Time);
	OS_SHUTDOWN_DELAY(1000000,OS_Reset_Delay_Time);
#endif
}

INT8U OS_FAULT_STK_CHECK(OS_TCB* tcb)//堆栈溢出检测
{
	if(tcb->StkPtr!=(STK32U*)0)
	{
		if(tcb->StkAdd[0]!=0)
		{
			tcb->TaskState=TASK_PAUSING;
#if (OS_FAULT_DISP_ENABLE	==1)		
			OSprintf("FSC提示：任务%s堆栈溢出，暂停运行！\r\n\r\n",tcb->TaskStr);
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
void OSTaskPrioBuckup(OS_TCB* tcb) //任务优先级备份
{
  tcb->TaskPrioBackup=tcb->TaskPrio;
}
void OSTaskPrioRecover(OS_TCB* tcb)  //任务优先级从备份中恢复
{
  tcb->TaskPrio=tcb->TaskPrioBackup;
}
void FSC_FAULT_SYSTEM_OS_Timer_Handler(INT16U tasknum)
{
	INT16U result;
	if(((tasknum>=2)&&(OSTCBTbl[tasknum].TaskFlagBit&(1<<_BIT_TimeLess))==0)&&\
     (OSTCBTbl[OS_System.TaskNext].StkPtr!=(STK32U*)0)&&(OSTCBTbl[tasknum].TaskState==TASK_RUNNING))//从任务2开始检测
	{		
		result= (OSTCBTbl[tasknum].TaskCPUOccRateCnt*100.0/OS_System.TaskTimeSliceCnt)*10;//取一位小数
		if(result>900) //系统占用率(实测：当有任务卡死时占用率最高为98.1%)
		 {	
			 if(result<=980) 
			 { 
#if (OS_FAULT_DISP_ENABLE == 1)	
					OSprintf("FSC提示：任务%s CPU占用率过高，占用率:%0.1f%%\r\n",OSTCBTbl[tasknum].TaskStr,result/10.0);
#endif					 
			 }
			 else  
			 {
				 OS_Fault.task[tasknum]=OS_TRUE;	 	 
				 if(OS_System.RuningMode==0)
				 {
					 /*----------------有待开发-------------------*/				 
				 }	
				 else if(OS_System.RuningMode==1)		
				 {
					 if(OSTCBTbl[tasknum].TaskPrio==0)
					 {
			       /*----------------有待开发-------------------*/  
					 }
					 else
					 {
						 OSTCBTbl[tasknum].TaskPrio--;//逐级提高优先级
#if (OS_FAULT_DISP_ENABLE == 1)					 
						 OSprintf("FSC提示：任务%s卡死，逐级降低优先级,当前优先级:%d\r\n",OSTCBTbl[tasknum].TaskStr,OSTCBTbl[tasknum].TaskPrio); 
#endif						 
					 }
				 }					 
			 }		 
		 }
		 else if(result<1) //系统占用率<0.1%
		 {		
			 if(result>0)
			 {
#if (OS_FAULT_DISP_ENABLE == 1)				 
				 OSprintf("FSC提示：任务%s CPU占用率过低，占用率:%0.1f%%\r\n",OSTCBTbl[tasknum].TaskStr,result/10.0);
#endif			 
			 }
			 else
			 {
				 OS_Fault.task[tasknum]=OS_TRUE;			 
				 if(OS_System.RuningMode==0)
				 {
           /*----------------有待开发-------------------*/
				 }
				 else if(OS_System.RuningMode==1)
				 {
					 if(OSTCBTbl[tasknum].TaskPrio==OSRunStateHighestPrioTaskNumGet())
					 {
					   /*----------------有待开发-------------------*/
					 }
					 else
					 {
						 if(OSTCBTbl[tasknum].TaskPrio<OS_MAX_TASKS-1) OSTCBTbl[tasknum].TaskPrio++;//逐级提高优先级
#if (OS_FAULT_DISP_ENABLE == 1)					 
						 OSprintf("FSC提示：任务%s未运行，逐级增大优先级,当前优先级:%d\r\n",OSTCBTbl[tasknum].TaskStr,OSTCBTbl[tasknum].TaskPrio); 
#endif			 
					 }
				 }
			 }
		 }
		 else //正常范围
		 {
			 if(OS_Fault.task[tasknum]==OS_TRUE)
			 {
					OS_Fault.task[tasknum]=OS_FALSE;
				  OSTaskPrioRecover(&OSTCBTbl[tasknum]);
					OSTCBTbl[tasknum].TaskState=TASK_RUNNING;
#if (OS_FAULT_DISP_ENABLE == 1)	 
				 OSprintf("FSC提示：任务%s正常运行，已恢复优先级,当前优先级:%d\r\n",OSTCBTbl[tasknum].TaskStr,OSTCBTbl[tasknum].TaskPrio); 
#endif 
			 }
		 }
	 }
}

#endif
/*-----------------------------------FSC智能纠错系统函数（Page Down）------------------------------------------*/






/*----------------------------------------系统函数（Page Up)--------------------------------------------------*/

//只要产生PendSV中断,跳转中断处理函数前 xPSR,PC,LR,R12,R3-R0被自动保存到系统或任务栈中(此步聚是PendSV中断硬件机制)，
//保存在哪个栈取决于当前的SP类型，如是MSP则保存到系统栈，如是PSP则保存到任务栈。本OS是保存于任务栈。而R11-R4需要手动保存到任务栈中
//入栈顺序：栈顶->栈尾 xPSR,PC,LR,R12,R3-R0,R11-R4。
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
    *(  stk)  = (INT32U)0x01000000L;   // xPSR bit[24]必须置1，否则上电会直接进入Fault中断  
#endif
    *(--stk)  = (INT32U)task;          // R15 (PC)             
    *(--stk)  = (INT32U)0xFFFFFFFEL;   // R14 (LR)初使化为最低4位为E，是一个非法值，主要目的是不让使用R14，即任务是不能返回的         
        stk  -= 4;                     //(R12、R3-R1)
	  *(--stk)  = (INT32U)0;             // R0(任务栈初使化R0必须置0,SP从R0恢复)
#if (__FPU_PRESENT == 1)&&(__FPU_USED == 1) 
        stk  -= 16;
#endif    
	    stk  -=  8;                      // R11-R4
    return stk;
}

void OSInit(void) //系统初使化，初使化各个任务的任务控制块变量的参数
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
		OS_System.TaskTimeSliceCnt=1; //从1开始计数(0不能作除数)
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
		OS_System.RunLastTask=0;//初使化为任务0
		OS_System.TaskHighestPrioLast=0;//初使化最高任务优级为任务0的优先级
		OS_System.TaskNext=0;    //Next任务计数初使化
		
    OS_System.LockNesting=0;
		OS_System.IntNesting=0;
    OS_System.Running=OS_TRUE;//运行标志
		
		OS_CreatTaskIdle();      //创建任务0  -空闲任务
    OSCreatTaskManager();    //创建任务1  -任务管理器任务
		
		OSTCBRun = OSTCBCur;     //运行TCB指向Cur
		OSTCBCur = &OSTCBTbl[0]; //从任务0开始运行
		OSTCBNext = &OSTCBTbl[1];//下一任务赋给Next
		
		OSScheduler_Process();//调度			
}

void OS_CreatTaskIdle(void)//创建空闲任务(独立创建)
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
void OSCreatTaskManager(void)//创建任务管理器任务(独立创建)
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
void OSTaskCreate(char* taskstr,void (*task),INT32U stksize,INT32U tasktimeslice,INT16U taskprio,TASK_STATE taskstate)//动态创建任务函数
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
	  i=2;         //跳过空闲任务和任务管理器任务创建用户函数
		while(OSTCBTbl[i].StkPtr != (STK32U*)0) {   //查找空TCB
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
void OSTaskCreateStatic(char* taskstr,void (*task),STK32U *stkptr,INT32U stksize,INT32U tasktimeslice,INT16U taskprio,TASK_STATE taskstate)//静态创建任务函数(创建存放内存区域(堆栈)等)
{
    INT16U i;
#if (OS_CMD_ALL_ENABLE== 1)		
	  INT16U j; 
#endif	
    STK32U *stk;
	  stk=(STK32U*)&stkptr[stksize-1];
	  i=2;         //跳过空闲任务和任务管理器任务创建用户函数
		while(OSTCBTbl[i].StkPtr != (STK32U*)0) {   //查找空TCB
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
void OSTaskDelete(void (*Taskx))//动态删除任务函数
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
void OSTaskResume(void (*Taskx),TASK_STATE taskstate)//动态恢复已删除的任务函数
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
/*----------------------------------------系统函数（Page Down)-------------------------------------------------*/


/*---------------------------------------系统状态函数（Page Up)------------------------------------------------*/
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
			if(CupOccRate>OS_System.TaskCPUOccRateMax)//更新CPU最大占用率
			{
				OS_System.TaskCPUOccRateMax=CupOccRate;
				OS_Information_Backup(&CPUOccRateMax);//保存一次信息
			}
			CupOccRate=1000-CupOccRateIdle-CupOccRateManage;			
			if(CupOccRate>OS_System.UserTaskCPUOccRateMax)//更新用户任务CPU最大占用率
			{
				OS_System.UserTaskCPUOccRateMax=CupOccRate;
				OS_Information_Backup(&UserCPUOccRateMax);//保存一次信息
			}			
		}
#endif	 
}
/*--------------------------------------系统状态函数（Page Down)-----------------------------------------------*/


/*-------------------------------------内存自动整理函数（Page Up)----------------------------------------------*/
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
				OSprintf("  #系统空闲#\r\n");
				OSprintf("开始内存碎片整理\r\n");
				OSprintf("---正在优化---\r\n");
				FSC_FragMerge();
				OSprintf("***优化完成***\r\n");
				OSprintf("结束内存碎片整理\r\n");
			}
		}
	}
#endif	
}
	
/*------------------------------------内存自动整理函数（Page Down)---------------------------------------------*/


/*--------------------------------------系统任务函数（Page Up)------------------------------------------------*/

void OS_TaskIdle(void) //空闲任务内容函数(禁止调用OS_delayMs函数)  (用以防止0个任务运行出错)
{
	OS_TaskStackCounter_Process(); //运行一次任务堆栈统计函数
	while(1) //空闲任务用于统计cpu使用率
	{
    OSTaskCPUOccRateCounter_Process();//任务cpu使用率计算程序	
		MemoryManage_Process();           //内存碎片整理程序	
		if(OS_System.RuningMode==1)//纯优先级运行模式情况下
		{
		  OSSchedSwitch(); //调度切换任务 
		}
	}
}
void OS_TaskManager(void) //任务管理器任务内容函数
{	
	INT32U i;
	OSprintf("-@FSC_STOS_%s Inside\r\n",_FSC_STOS_Version);
	OSprintf("OS Cmd System，Please Send: cmd/help//\r\n\r\n");
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

		//任务延时
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
/*--------------------------------------系统任务函数（Page Down)----------------------------------------------*/


/*--------------------------------------任务设置有关函数（Page Up)--------------------------------------------*/

#if (OS_GET_SET_SW_ENABLE == 1)||(OS_CMD_NSY_ENABLE == 1)
void OSLockStateReset(void) //解锁
{
  OS_System.LockNesting=0;
}
void OSTaskDelayMsSet(void* Taskx,INT32U nms)
{
   INT16U i ;
	 if( OSTCBTbl[0].TaskAdd != (INT32U)Taskx ) //跳过空闲任务，不能更改空闲任务的状态
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
OS_BOOL OSTaskStateSet(void* Taskx,TASK_STATE TaskState) //任务状态设置函数(用户可调用) //返回OS_FALSE设置失败 OS_TRUE设置成功
{
	 OS_INT_ENTER();
	 INT16U i ;
#if (OS_SIGN_PP_ENABLE == 1)	
	 INT16U j;
#endif	
	 OS_BOOL  result=OS_FALSE;  
	 if(OSTCBCur->TaskAdd==(INT32U)Taskx) //如果是设置本任务，停止本任务，立即切换到其他任务运行
		{
#if (OS_SIGN_PP_ENABLE == 1)			
			 for(j=0;j<MUTEX_SIZE;j++)//查找当前任务是否处于Mutex占用中
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
		 if( OSTCBTbl[0].TaskAdd != (INT32U)Taskx ) //跳过空闲任务，不能更改空闲任务的状态
		 {
			 for(i = 0; i < OS_MAX_TASKS; i++) 
				{  
					 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
					 {
#if (OS_SIGN_PP_ENABLE == 1)							 
						 do
						 {						 
							 for(j=0;j<MUTEX_SIZE;j++) //查找设置目标任务是否处于Mutex占用中
							 {
								 if(OS_System.MutexTaskNum[j]==OSTCBTbl[i].TaskNum)
								 { 
									 OS_INT_EXIT();
									 OSPendSVPulse();//把任务切换出去等待目标任务释放Mutex
									 OS_INT_ENTER(); 
									 if(OS_System.MutexTaskNum[j]==OSTCBTbl[i].TaskNum)//重新确认 
								   {						  
									   j--; //如果没有释放则重新等待
								   } 
									 else
									 {
										 break; //如果释放了，重新从0号Mutex扫描，保证所有Mutex都不处于被目标任务使用状态
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
void OSTaskPrioBackup(void* Taskx) //任务优先级备份
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
void OSTaskPrioSet(void* Taskx,INT16U Taskprio) //任务优先级设置函数(每调用一次就会自动备份一次)
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
void OSTaskPrioBackupSet(void* Taskx,INT16U Taskprio) //任务备份优先级设置函数(用户可调用)
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
void OSTimeSliceValSet(INT32U time) //设置时间切片节拍计数值
{
   OS_System.TaskTimeSliceCnt=time;
}
void OSClockValSet(INT32U time)    //设置系统节拍计数值
{
  OS_System.ClockCnt=time;
}
#endif
/*-------------------------------------任务设置有关函数（Page Down)-------------------------------------------*/


/*-----------------------------------系统获取与跳转有关函数（Page Up)------------------------------------------*/
#if (OS_GET_SET_SW_ENABLE == 1)
OS_BOOL OSLockStateGet(void) //系统中是否有任务处于锁定状态，返回OS_TRUE是，OS_FALSE否
{
  if(OS_System.LockNesting>0) return OS_TRUE;
	else return OS_FALSE;
}
INT16U* OSRunTimeGet(void) //获取系统累计运行时间,指针成员分别为：ms、s、m、h、d、m、y
{
  return (INT16U*)&OS_System.RunTime_ms;
}
INT8U* OSFlagGroupPendTableGet(INT16U FGNum) //获取群等待成员
{
#if (OS_SIGN_PP_ENABLE == 1)	
	INT16U i;
	for(i=0;i<FLAG_SIZE;i++)
	{
		OS_System.ReturnFlagGroupTable[i]=OS_System.FlagGroupTable[FGNum][i];//复制成员到返回查询群列表
	}
	return (INT8U*)OS_System.ReturnFlagGroupTable;
#else 
	return (INT8U*)0;
#endif	
}
INT16U OSMutexBlockTaskGet(INT16U MNum)//获取Mutex当前阻塞的任务
{
#if (OS_SIGN_PP_ENABLE == 1)	
	return OS_System.MutexTaskNum[MNum];
#else 
	return 0;	
#endif	
}
INT8U OSTaskStateGet(void* Taskx) //获取任务状态 (高4位为锁定状态，低4位为运行状态)
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
INT16U OSTaskPrioGet(void* Taskx) //获取任务优先级
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
INT16U OSTaskPrioBackupGet(void* Taskx) //获取任务备份优先级
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
INT32U OSTimeSliceValGet(void) //获取时间切片节拍计数值
{
    return OS_System.TaskTimeSliceCnt;
}
INT32U OSClockValGet(void)    //获取系统节拍计数值
{
  return OS_System.ClockCnt;
}
INT32U OSTimerValGet(void) //获取系统定时器当前计数值(默认为9MHz，每次计数用时1/9 us)
{
   return SysTick->VAL;  //返回系统定时器当前计数值(用于用户us级精确延时，计数单位为系统定时器主频，此处使用嘀嗒定时器，主频为72Mhz时为9MHz，其他主频时，按8分频计算)
}
void OSTaskSwitchBack(void* Taskx) //任务跳转 带返回
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
void OSTaskSwitch(void* Taskx) //任务跳转  不带返回
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
	 if(OS_System.TaskSwitchBackNum>0) //是否有跳转发生完成
	 {
		 OS_System.TaskNext=OS_System.TaskSwitchBackNum;	//更新顺序运行断点
		 OSTCBNext = &OSTCBTbl[OS_System.TaskSwitchBackNum]; //返回跳转前的任务
		 OS_System.TaskSwitchBackNum=0;                      //清除标志
		 OS_System.TaskNextRunFlag=OS_TRUE; //顺序运行标志置TRUE	
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
	 if(OS_System.TaskSwitchBackNum>0) //是否有跳转发生完成
	 {
		 if(OSTCBCur->TaskPrio<=OSTCBTbl[OS_System.TaskSwitchBackNum].TaskPrio)
		 {
		   OSTCBCur=&OSTCBTbl[OS_System.TaskSwitchBackNum];//返回跳转前的任务
			 OS_System.TaskSwitchBackNum=0;//清除标志
		 } 		 
	 }	
#endif	 
}
/*----------------------------------系统获取与跳转有关函数（Page Down)-----------------------------------------*/


/*-------------------------------------系统不常用有关函数（Page Up)--------------------------------------------*/
#if (OS_REMOTE_ENABLE == 1)


#endif
/*------------------------------------系统不常用有关函数（Page Down)-------------------------------------------*/


/*-----------------------------------系统虚拟定时器有关函数（Page Up)-------------------------------------------*/
#if (OS_TIM_SYS_ENABLE == 1)
INT16U OSTimerCreate(void)   //创建OSTimer，返回Timer_ID
{
	INT16U Timer_ID=0;
  Timer_ID=OS_System.TIMER_APPLY_COUNT;
	OS_System.TIMER_APPLY_COUNT++;
	return Timer_ID;
}
void OSTimerReloadSet(INT16U TNum,INT32U time)//重装载定时值设置 
{
  OS_System.TIMER[TNum]=time;
}
void OSTimerAutoReloadSet(INT16U TNum,INT32U time)//自动重装载定时值设置 (time= 0-关闭自动重装载 ， >0启用自动重装载)
{
  OS_System.TIMER_RELOAD[TNum]=time;
}
void OSTimerValueSet(INT16U TNum,INT32U time)//定时值设置
{
  OS_System.TIMER[TNum]=time;
}
void OSTimerReloadSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec)//<=49天17时2分47秒
{
	INT32U time;
	time=(day*24*3600+hour*3600+min*60+sec)*1000;
  OS_System.TIMER[TNum]=time;
}
void OSTimerAutoReloadSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec)//<=49天17时2分47秒
{
	INT32U time;
	time=(day*24*3600+hour*3600+min*60+sec)*1000;
  OS_System.TIMER_RELOAD[TNum]=time;
}
void OSTimerValueSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec)//<=49天17时2分47秒
{
	INT32U time;
	time=(day*24*3600+hour*3600+min*60+sec)*1000;
  OS_System.TIMER[TNum]=time;
}
INT32U OSTimerValueGet(INT16U TNum) //返回当前计数值
{
  return OS_System.TIMER[TNum];
}
OS_BOOL OSTimerStateGet(INT16U TNum) //返回OS_TRUE ：定时完成，  OS_FALSE：未完成
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
/*----------------------------------系统虚拟定时器有关函数（Page Down)------------------------------------------*/



/*--------------------------------------系统信号量有关函数（Page Up)--------------------------------------------*/
#if (OS_SIGN_PP_ENABLE == 1)
INT16U OSFlagCreate(void)     //创建OSFlag，返回Flag_ID
{
	INT16U Flag_ID=0;
	Flag_ID=OS_System.FLAG_APPLY_COUNT;
  OS_System.FLAG_APPLY_COUNT++;
	return Flag_ID;
}	
INT16U OSFlagGroupCreate(void)     //创建OSFlagGroup，返回FlagGroup_ID
{
	INT16U FlagGroup_ID=0;
	FlagGroup_ID=OS_System.FLAGGROUP_APPLY_COUNT;
  OS_System.FLAGGROUP_APPLY_COUNT++;
	return FlagGroup_ID;
}	
INT16U OSMutexCreate(void)     //创建OSMutex，返回Mutex_ID
{
	INT16U Mutex_ID=0;
	Mutex_ID=OS_System.MUTEX_APPLY_COUNT;
  OS_System.MUTEX_APPLY_COUNT++;
	return Mutex_ID;
}	
INT16U OSMBoxCreate(void)     //创建OSMBox，返回MBox_ID
{
	INT16U MBox_ID=0;
	MBox_ID=OS_System.MBOX_APPLY_COUNT;
  OS_System.MBOX_APPLY_COUNT++;
	return MBox_ID;
}	
void OSFlagPost(INT16U FNum) //发送标志量
{
	OSSchedLock();
	INT16U i,j;
	for(i=0;i<OS_MAX_TASKS;i++)
	{ 
    if(OSTCBTbl[i].TaskFlagBit&_BIT_Flag)//单个OSFlag处于等待信号标志状态
		{ 
			if(OSTCBTbl[i].FlagName==FNum)//判断该等待任务等待的OSFlag是否是当前准备发送的OSFlag
			{ 
				OS_System.FLAG[FNum][i]=OS_TRUE; //给等待OSFlag的任务发送标志量
				OS_System.POST_FLAG_COUNT[FNum]++;//等待处理信号次数累加
				if((OS_System.FLAGPendType[FNum][i]!=OSFlag_NBPC)&&(OS_System.FLAGPendType[FNum][i]!=OSFlag_NBPN))
				{
					OSTCBTbl[i].TaskFlagBit&=~_BIT_Flag;//清除信号标志状态
				  OSTCBTbl[i].TaskDelayMs=1;  //等待延时置1，取消等待OSFlag任务的等待延时 
				}
			}
		}
		if(OSTCBTbl[i].TaskFlagBit&_BIT_FlagGroup)//群OSFlag处于等待信号标志状态
		{
			for(j=0;j<OS_System.FLAGGROUP_APPLY_COUNT;j++)
			{
				if(OS_System.FlagGroupTable[j][FNum]==OS_TRUE)//判断是否为群成员
				{
					OS_System.FLAG[FNum][i]=OS_TRUE; //给等待OSFlag群的任务发送成员标志量		
				}
			}
		}
	}
	OSSchedUnlock();
}

INT8U OSFlagPend(OSFlagPendMode pendtype,INT16U FNum,INT32U timeout_ms) //等待标志量 带超时时间  ，返回OS_TRUE：等待未超时  OS_FALSE：等待超时 (NBPC模式时返回信号量累计数次)
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
			OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除旧标志量,等待新的标志量发送
			OS_delayMs(timeout_ms);
			OSTCBCur->TaskFlagBit&=~_BIT_Flag;
			if(OSTCBCur->TaskFlagBit&_BIT_TimeOut) //超时检测
			{
				OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
				OSTCBCur->TaskFlagBit&=~_BIT_TimeOut;//清除超时标志位
				result = OS_FALSE;
			}
			else
			{	
				if(OS_System.FLAG[FNum][OSTCBCur->TaskNum]==OS_TRUE)
				{
					OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
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
				OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
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
		  if(OS_System.FLAG[FNum][OSTCBCur->TaskNum]==OS_FALSE)//检测旧标志
			{
				OS_delayMs(timeout_ms);
				OSTCBCur->TaskFlagBit&=~_BIT_Flag;
				if(OSTCBCur->TaskFlagBit&_BIT_TimeOut) //超时检测
				{
					OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
					OSTCBCur->TaskFlagBit&=~_BIT_TimeOut;//清除超时标志位
					result = OS_FALSE;
				}
				else
				{	
					OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
					result = OS_TRUE; 
				}		
			}
			else
			{
				OSTCBCur->TaskFlagBit&=~_BIT_Flag;
				OSTCBCur->TaskDelayMs=0;//让其处于就绪状态
				OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
				result = OS_TRUE;
			}
		}
		break;
		case OSFlag_BPC:
		{
		  if(OS_System.POST_FLAG_COUNT[FNum]==0)//检测标志累计数量
			{
				OS_delayMs(timeout_ms);
				OSTCBCur->TaskFlagBit&=~_BIT_Flag;
				if(OSTCBCur->TaskFlagBit&_BIT_TimeOut) //超时检测
				{
					OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
					OSTCBCur->TaskFlagBit&=~_BIT_TimeOut;//清除超时标志位
					result = OS_FALSE;
				}
				else
				{	
					OS_System.POST_FLAG_COUNT[FNum]--;//等待处理信号次数-1(已处理)
					OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
					result = OS_TRUE; 
				}		
			}
			else
			{
				OSTCBCur->TaskFlagBit&=~_BIT_Flag;
				OS_System.POST_FLAG_COUNT[FNum]--;//等待处理信号次数-1(已处理)
				OSTCBCur->TaskDelayMs=0;//让其处于就绪状态
				OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
				result = OS_TRUE;
			}
		}
		break;
		case OSFlag_NBPC:
		{
			if(OS_System.POST_FLAG_COUNT[FNum]>0)
			{
				result=OS_System.POST_FLAG_COUNT[FNum];//返回剩于等待处理次数
				OS_System.POST_FLAG_COUNT[FNum]--;//等待处理信号次数-1(已处理)
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

void OSFlagAddToGroup(INT16U FGNum,INT16U FNum)  //添加标志量到标志量群
{
	OSSchedLock();
  OS_System.FlagGroupTable[FGNum][FNum]=OS_TRUE;
	OSSchedUnlock();
}

OS_BOOL OSFlagGroupPend(OSFGroupPendMode pendtype,INT16U FGNum,INT32U timeout_ms) //等待标志量群 带超时时间  ，返回OS_TRUE：等待未超时  OS_FALSE：等待超时
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
			if(OSTCBCur->TaskFlagBit&_BIT_TimeOut) //超时检测
			{
				OSTCBCur->TaskFlagBit&=~_BIT_TimeOut;//清除超时标志位
				result = OS_FALSE;
			}
			else
			{
				OS_System.POST_FGROUP_COUNT[FGNum]--;
				OSTCBCur->TaskFlagBit&=~_BIT_FlagGroup;//清除本任务FlagGroup群等待标志
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

void OSMutexPost(INT16U MNum) //发送互斥量 （主要用于解除mutex锁定状态,解除后所有mutex等待任务将重新竞争mutex权限）
{
	OSSchedLock();
	if(OSTCBCur->TaskNum==OS_System.MutexTaskNum[MNum])//判断当前任务是否具有Post权限
	{
		if((OSTCBCur->TaskFlagBit&_BIT_Mutex)==0)//判断当前任务是否处于非等待mutex状态中
		{
			if(OS_System.MUTEX[MNum]==OS_TRUE) //判断mutex是否为true(忙)
			{
				OS_System.MutexTaskNum[MNum]=0;//放弃Post权限
				OS_System.MUTEX[MNum]=OS_FALSE;//置mutex为OS_FALSE表示mutex空闲
			}
		}
	}
	OSSchedUnlock();
}
OS_BOOL OSMutexPend(OSMutexPendMode pendtype,INT16U MNum,INT32U timeout_ms) //等待互斥量 带超时时间  ，返回OS_TRUE：等待未超时  OS_FALSE：等待超时
{
	OS_BOOL result;
	OSSchedLock();
	OSTCBCur->MutexName=MNum;
	OS_System.MutexNameInTask[OSTCBCur->TaskNum]=MNum;
	if(OS_System.MutexTaskNum[MNum]==0) OS_System.MutexTaskNum[MNum]=OSTCBCur->TaskNum;//任务获得Post权限
	OSTCBCur->TaskFlagBit|=_BIT_Mutex;//置位等待mutex标志
	OSSchedUnlock();
	switch(pendtype)
	{
		case OSMutex_BPN:
		{
			OS_delayMs(timeout_ms);
			OSTCBCur->TaskFlagBit&=~_BIT_Mutex;//清除等待mutex标志
			if(OSTCBCur->TaskFlagBit&_BIT_TimeOut) //超时检测
			{	
				OSTCBCur->TaskFlagBit&=~_BIT_TimeOut;//清除超时标志位
				result = OS_FALSE;
			}
			else
			{
				OSTCBCur->TaskFlagBit&=~_BIT_TimeLess;//取消无限等待标志
				OS_System.MutexTaskNum[MNum]=OSTCBCur->TaskNum;//任务获得Post权限(必须存在)
				result = OS_TRUE;
			}
		}
		break;
		case OSMutex_NBPN:
		{
		  if(OS_System.MutexTaskNum[MNum]==OSTCBCur->TaskNum)
			{
				OSTCBCur->TaskFlagBit&=~_BIT_Mutex;//清除等待mutex标志
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
void OSMBoxPost(INT16U MNum,void* fp)  //发送邮件
{
	OSSchedLock();
	INT16U i;
	for(i=0;i<OS_MAX_TASKS;i++)
	{
		if(OSTCBTbl[i].TaskFlagBit&_BIT_MBox)//判断该任务是否处于等待mbox状态
		{
			if(OSTCBTbl[i].MBoxName==MNum)//判断等待的mbox是否是当前准备发送的MNum
			{
				OS_System.MBOX[MNum*OS_MAX_TASKS+i]=fp;//给在等待邮件的任务发送邮件
				OS_System.POST_MBOX_COUNT[MNum]++;
				OS_System.MBQueue[OS_System.POST_MBQ_COUNT[MNum]+MNum*MBQ_SIZE]=fp;
				if(OS_System.POST_MBQ_COUNT[MNum]<MBQ_SIZE-1) OS_System.POST_MBQ_COUNT[MNum]++;
				if((OS_System.MBOXPendType[MNum][i]!=OSMBox_NBPN)&&(OS_System.MBOXPendType[MNum][i]!=OSMBox_NBPQ))
				{
					OSTCBCur->TaskFlagBit&=~_BIT_MBox;
				  OSTCBTbl[i].TaskDelayMs=0;//等待延时清0
				}
			}
		}
	}
	OSSchedUnlock();
} 

void* OSMBoxPend(OSMBoxPendMode pendtype,INT16U MNum,INT32U timeout_ms) //等待邮箱 带超时时间  ，返回非(void*)0：等待未超时  (void*)0：等待超时
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
			if(OSTCBCur->TaskFlagBit&_BIT_TimeOut) //超时检测
			{
				OSTCBCur->TaskFlagBit&=~_BIT_TimeOut;//清除超时标志位
				_mbox=(void*)0;
			}
			else
			{  
				if(_mbox!=(void*)0)
				{
					OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum]=(void*)0;  //清空本任务邮箱为(void*)0	
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
				if(OSTCBCur->TaskFlagBit&_BIT_TimeOut) //超时检测
				{
					OSTCBCur->TaskFlagBit&=~_BIT_TimeOut;//清除超时标志位
					_mbox=(void*)0;
				}
				else
				{  
					if(_mbox!=(void*)0)
					{
						OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum]=(void*)0;  //清空本任务邮箱为(void*)0	
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
				OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum]=(void*)0;  //清空本任务邮箱为(void*)0	
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
/*-------------------------------------系统信号量有关函数（Page Down)-------------------------------------------*/



/*------------------------------------系统信号量处理有关函数（Page Up)-------------------------------------------*/
#if (OS_SIGN_PP_ENABLE == 1)
void OSFlagGroupHandler(void)
{
	INT16U TaskCount,Count,NCount;
	for(TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++)
	{
		if(OSTCBTbl[TaskCount].TaskFlagBit&_BIT_FlagGroup)//检测有FlagGroup等待的任务
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
				OSTCBTbl[TaskCount].TaskDelayMs=1;//准备退出延时，进入等待运行的状态(由优先级控制)
				OSTCBTbl[TaskCount].TaskFlagBit&=~_BIT_TimeOut;//清除超时标志
			}
		}
	}
}
void OSMutexHandler_Order(void)
{
  INT16U TaskCount;	
	for(TaskCount=OS_System.TaskNext; TaskCount < OS_MAX_TASKS; TaskCount++)//检测NextTask以后的最靠前的任务是否有等待mutex
	{
		if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)&&\
			 (OSTCBTbl[TaskCount].TaskFlagBit&_BIT_Mutex)&&(OS_System.MUTEX[OS_System.MutexNameInTask[TaskCount]]==OS_FALSE) )
			{
				if(OS_System.MutexPendType[OS_System.MutexNameInTask[TaskCount]][TaskCount]==OSMutex_BPN)
				{				
					OSTCBTbl[TaskCount].TaskFlagBit&=~_BIT_TimeLess;//取消无限等待标志
					OSTCBTbl[TaskCount].TaskFlagBit&=~_BIT_TimeOut;//取消延时标志
					OSTCBTbl[TaskCount].TaskDelayMs=1;//进入准备运行状态(能否能立即运行取决于优先级)
				  OSTCBTbl[TaskCount].TaskFlagBit&=~_BIT_Mutex;//取消等待mutex标志
				  OS_System.MUTEX[OS_System.MutexNameInTask[TaskCount]]=OS_TRUE;//当前任务的mutex置忙					
				}
				else
				{
					if(OSTCBTbl[TaskCount].TaskDelayMs<=1)
					{
						 OS_System.MutexTaskNum[OS_System.MutexNameInTask[TaskCount]]=OSTCBCur->TaskNum;//任务获得Post权限				
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
					Count=TaskCount; //Count用来记录有mutex等待的最大优先级任务
				}
		}			
	}
	if(Count!=0)
	{
		if(OS_System.MutexPendType[OS_System.MutexNameInTask[Count]][Count]==OSMutex_BPN)
		{				
			OSTCBTbl[Count].TaskFlagBit&=~_BIT_TimeLess;//取消无限等待标志
			OSTCBTbl[Count].TaskFlagBit&=~_BIT_TimeOut;//取消延时标志
			OSTCBTbl[Count].TaskDelayMs=1;//进入准备运行状态(能否能立即运行取决于优先级)
			OSTCBTbl[Count].TaskFlagBit&=~_BIT_Mutex;//取消等待mutex标志
			OS_System.MUTEX[OS_System.MutexNameInTask[Count]]=OS_TRUE;//当前任务的mutex置忙					
		}
		else
		{
			if(OSTCBTbl[Count].TaskDelayMs<=1)
			{
				 OS_System.MutexTaskNum[OS_System.MutexNameInTask[Count]]=OSTCBCur->TaskNum;//任务获得Post权限				
			}
		}
	}
}
#endif
/*-----------------------------------系统信号量处理有关函数（Page Down)----------------------------------------*/



/*------------------------------------系统时间统计有关函数（Page Up)-------------------------------------------*/
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
void OS_ShutDown_DelayTime_Set_YMDHMS(INT32U year,INT8U mouth,INT16U day,INT8U hour,INT8U minute,INT8U second) //系统关闭定时时间设置(用户可用)
{			 
	OS_System.ShutDownTime_second=second;
  OS_System.ShutDownTime_minute=minute;
	OS_System.ShutDownTime_hour  =hour;
	OS_System.ShutDownTime_day   =day;	
	OS_System.ShutDownTime_mouth =mouth;
	OS_System.ShutDownTime_year  =year;
}
void OSRunTimeCount_Process(void) //系统运行时间统计(用户不可用)
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
				OSprintf("定时关闭完成，系统停止运行\r\n");
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
/*-----------------------------------系统时间统计有关函数（Page Down)------------------------------------------*/


/*----------------------------------------系统核心函数（Page Up)--------=-------------------------------------*/

void OSTimeSliceCounterReset(void) //时间切片计数器清0
{
	OS_System.TimeSliceCnt=0; //时间切片计数器清0，准备重新计数
}
/*--------------------------------------任务切换函数--------------------------------*/
void OSSchedSwitch(void) //任务调度并切换任务
{
	OSScheduler_Process();
	if(OSTCBRun!=OSTCBCur) 
	{
		OSContextExchange();  
	}
}
void OSContextExchangeToTask(OS_TCB* tcb)//切换到指定任务
{
	OSTCBCur=tcb;
  OSContextExchange(); 
}
void OSSchedLock(void)   //任务切换上锁(用户可调用) 上锁后不会切换任务，cpu会一直运行当前任务直到解锁
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
void OSSchedUnlock(void)   //任务解锁（OSSchedLock和OSSchedUnlock必须成对出现）(用户可调用)
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
/*------------------------------系统内核有关函数-------------------------------------*/
void OSRunModeProcess_Order(void) //顺序模式服务程序
{
	INT16U TaskCount;
	for(TaskCount = 1; TaskCount < OS_MAX_TASKS; TaskCount++) //检测所有任务的延时时间是否有的到时
	{ 
		if(OSTCBTbl[TaskCount].TaskState==TASK_RUNNING) //跳过非运行态的任务
		{
			if( OSTCBTbl[TaskCount].TaskDelayMs ==1 )//检测延时时间是否到时
			 {
				 OSTCBCur=&OSTCBTbl[TaskCount]; 	        //跳转到延时完成的任务
				 OSTCBTbl[TaskCount].TaskDelayMs=0;	  //延时清0
				 OS_System.TaskNextRunFlag=OS_FALSE;    //顺序运行标志置FALSE	
				 break;
			 }
		 }	 
	}	
	if(TaskCount==OS_MAX_TASKS) OS_System.TaskNextRunFlag=OS_TRUE;	//顺序运行标志置TRUE			 
	if(OS_System.TaskNextRunFlag==OS_TRUE)	//判断是否可以按正常顺序运行
	{
		OSTCBCur = OSTCBNext;
		if(OS_System.TaskNext<OS_MAX_TASKS-1) OS_System.TaskNext++;
		else OS_System.TaskNext = 0;						
		while((OSTCBTbl[OS_System.TaskNext].StkPtr == (STK32U*)0)||(OSTCBTbl[OS_System.TaskNext].TaskState!=TASK_RUNNING)\
				 ||OSTCBTbl[OS_System.TaskNext].TaskDelayMs > 1) //跳过非法状态任务
		{ 
			OS_System.TaskNext++;
			if(OS_System.TaskNext == OS_MAX_TASKS) 
			{
				OS_System.TaskNext = 0;
				OS_System.RunOneRound=OS_TRUE;
			}
		}
		OSTCBNext = &OSTCBTbl[OS_System.TaskNext];	//更新Next任务
	}	
}
INT16U OSHighestPrioTaskNumGet(void)//获取已创建的所有状态任务中的最高优先级任务
{
  INT16U TaskCount,Count;	
	for(Count=0,TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++) //查找正在运行的常规最高优先级任务
	{ 
		if(OSTCBTbl[Count].TaskPrio<OSTCBTbl[TaskCount].TaskPrio) 
		{
			if(OSTCBTbl[Count].StkPtr!=(STK32U*)0)
				{
					Count=TaskCount; //Count用来记录最大优先级
				}
		}				
	}	
	return Count;
}
INT16U OSRunStateHighestPrioTaskNumGet(void)//获取处于运行态的最高优先级任务
{
  INT16U TaskCount,HighestPrioTask;	
	for(HighestPrioTask=0,TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++) //查找正在运行的常规最高优先级任务
	{ 
		if(OSTCBTbl[HighestPrioTask].TaskPrio<OSTCBTbl[TaskCount].TaskPrio) 
		{
			if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0))
				{
					HighestPrioTask=TaskCount; //Count用来记录最大优先级
				}
		}				
	}	
	return HighestPrioTask;
}
INT16U OSRdyHighestPrioTaskNumGet(void)//获取准备运行就绪最高优先级任务(返回先创建的最高优先级任务(如有同级存在))
{
  INT16U TaskCount,HighestPrioTask;	
	for(HighestPrioTask=0,TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++) //查找正在运行的常规最高优先级任务
	{ 
		if(OSTCBTbl[HighestPrioTask].TaskPrio<OSTCBTbl[TaskCount].TaskPrio) 
		{
			if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)&&\
				 (OSTCBTbl[TaskCount].TaskDelayMs <= 1)&&((OSTCBTbl[TaskCount].TaskFlagBit&_BIT_TimeLess)==0) )
				{
					HighestPrioTask=TaskCount; //HighestPrio用来记录最大优先级
				}
		}				
	}	
	return HighestPrioTask;
}

/***************************************************************************************
*任务名：OSNextRdyHighestPrioTaskNumGet
*作用  ：获取运行就绪最高优先级任务，具有相同优先级的多个任务轮询
*特点  ：如果扫描到本次最高优先级和上次相同且具有该优先级的任务除上次刚运行过的任务以外还有其他任务
        则返回其他任务(其他任务具有2个及以上时，按顺序返回，实现具有相同优先级的多个任务轮询)
***************************************************************************************/
INT16U OSNextRdyHighestPrioTaskNumGet(void)//获取运行就绪最高优先级任务(返回未运行过的最高优先级任务(该优先级一个轮询周期内)))
{
  INT16U TaskCount,HighestPrioTask,LastPrioTaskCount=0,SecdyHigestPrioTask,SecdyCheckFlag=0,MainCheckFlag=0;	
	for(HighestPrioTask=0,TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++) //查找正在运行的常规最高优先级任务
	{ 
		if(OSTCBTbl[HighestPrioTask].TaskPrio<OSTCBTbl[TaskCount].TaskPrio) 
		{
			if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)&&\
				 (OSTCBTbl[TaskCount].TaskDelayMs <= 1)&&((OSTCBTbl[TaskCount].TaskFlagBit&_BIT_TimeLess)==0) )
				{
					HighestPrioTask=TaskCount; //HighestPrioTask用来记录扫描到的最大优先级任务
				}
		}			
	}	
	if(OSTCBTbl[HighestPrioTask].TaskPrio>OSTCBTbl[OS_System.RunLastTask].TaskPrio)
	{
	  
	}
	else if(OSTCBTbl[HighestPrioTask].TaskPrio==OSTCBTbl[OS_System.RunLastTask].TaskPrio)
	{
		for(TaskCount=0; TaskCount < OS_MAX_TASKS; TaskCount++) //统计正在运行的常规最高优先级任务数量(上次运行过的除外)
		{ 
			if(TaskCount==OS_System.RunLastTask) continue ;
			if(OSTCBTbl[HighestPrioTask].TaskPrio==OSTCBTbl[TaskCount].TaskPrio) 
			{
				if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)&&\
					 (OSTCBTbl[TaskCount].TaskDelayMs <= 1)&&((OSTCBTbl[TaskCount].TaskFlagBit&(1<<_BIT_TimeLess))==0) )
					{
						LastPrioTaskCount++; //LastPrioTaskCount用来记录最大优先级任务数量(上次运行过的除外)
					}
			}			
		}	
		if(LastPrioTaskCount>0) //如果扫描到和上次运行的任务优先级相同的任务超过0个(不包含上次任务本身)
		{
		
			for(TaskCount=0; TaskCount < OS_MAX_TASKS; TaskCount++) //查找正在运行的常规最高优先级任务
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
void OSRunModeProcess_Prio(void)//优先级模式服务程序
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
	OSTCBCur=&OSTCBTbl[HighestPrioTask];//OSTCBCur指向最高优先级任务
	OSTCBTbl[HighestPrioTask].TaskDelayMs=0;//延时清0	
	if(HighestPrioTask==0) OS_System.RunOneRound=OS_TRUE;
}

/*---------------------------------任务切换关键函数(Page Down)---------------------------------------*/
void OSContextExchange(void) //上下文切换(任务环境切换,下个任务运行OSTCBCur指向的任务)
{
	OS_INT_ENTER();
	/*Enter PendSV_Handler*/ 
  OSPendSVPulse();
	/*Enter SVC_Handler*/
  //OSSVCPulse();
	OS_INT_EXIT();
}

void OSScheduler_Process(void) //任务调度器（只调度不切换）
{		 
	 if((OS_System.LockNesting == 0) && (OS_System.IntNesting == 0))
	 {			 
#if (OS_FAULT_ENABLE == 1)				 
RESCHEDULE:	
#endif					 
		 {	
#if (OS_SIGN_PP_ENABLE == 1)     					
			 OSFlagGroupHandler();//OSFlagGroup有关
#endif
			 if(OS_System.RuningMode==0)//运行模式:Order	
			 {		 
				 /*******************************************可抢断顺序运行*******************************************/
#if (OS_SIGN_PP_ENABLE == 1) 				 
				 OSMutexHandler_Order();//OSMutex有关函数
#endif			
         if(OSTaskSwitchCheck_Order()==OS_FALSE) //是否有跳转发生完成	
				 {					 
				   OSRunModeProcess_Order();
				 }
			 }
			 else				
				{		
					/*******************************************抢断式优先级******************************************/	
#if (OS_SIGN_PP_ENABLE == 1)     					
					OSMutexHandler_Prio();//OSMutex有关函数
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
/*---------------------------------任务切换关键函数(Page Up)---------------------------------------*/

void OSTaskTimeDelayCount_Process(void) //任务延时计数程序
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
				 OSTCBTbl[TaskCount].TaskFlagBit|=_BIT_TimeOut;//标记延时完成
				 OS_System.TaskDelayTimeOutFlag=OS_TRUE;
			 }
		 }
	 }	
}
void OS_SysTick_Handler(void) //任务切换核心函数
{
	 if(OS_System.Running==OS_TRUE)//系统状态检测
	 {
		 OS_System.ClockCnt++;       //系统时钟节拍计数器累加
		 
     OSTaskTimeDelayCount_Process();//任务延时计数程序
		 
#if (OS_TIMECOUNTSYS_ENABLE == 1)		 
		 OSRunTimeCount_Process();   //系统运行时间统计程序
#endif			 
#if (OS_TIM_SYS_ENABLE == 1)
     OSTimerCount_Process();     //虚拟定时器计数程序
#endif	
		 
		 OS_System.TimeSliceCnt++;   //时间切片计数器(以时钟节拍为基准)
		 if((OS_System.TimeSliceCnt>=OSTCBCur->TaskTimeSlice) //时间切片溢出判断(决定一个时间切片等于多少个时钟节拍)
			  ||(OS_System.TaskDelayTimeOutFlag==OS_TRUE)) //任务延时完成标志(每次有任务延时完成时进来调度一次)
		 { 
			 if(OS_System.RuningMode!=1)//非纯优先级运行模式情况下
			 {
         OSSchedSwitch(); //调度并切换任务 
				 if(OS_System.TaskDelayTimeOutFlag==OS_TRUE)
				 {
					 OS_System.TaskDelayTimeOutFlag=OS_FALSE;
					 if(OSTCBRun!=OSTCBCur)
					 {
						 OS_System.TimeSliceCnt=0; //时间切片计数器清0，准备重新计数
					 }
				 }
				 else
				 {
					 OS_System.TimeSliceCnt=0; //时间切片计数器清0，准备重新计数
				 }
			 }
#if (OS_FAULT_ENABLE == 1) 				 
			 OSTaskEndlessLoopsCheck_Process();//任务卡死检测程序(原理：任务连续不断运行N次则视为卡死)
#endif			 	 
#if (OS_CMD_ALL_ENABLE == 1) 		 
       OSTaskCPUOccRateCount_Process();//任务占用率统计程序
#endif   		 
		 }	 
	 }
	 else if(OS_System.Running==OS_FALSE)
	 {	
     OS_StopRun_Process();//系统关闭程序
	 }
}
/*---------------------------------------系统核心函数（Page Down)-------=-------------------------------------*/



/*----------------------------------------系统延时函数（Page Up)--------=-------------------------------------*/

void delay_us(INT32U nus) //微秒延时函数 (用户可调用,可在中断中使用)
{   	
	OSTIMER_INT_OFF();//关闭系统定时器中断
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
		OSTIMER_INT_ON(); 	//打开系统定时器中断
	}
}
void delay_ms(INT32U nms)  //毫秒延时函数 (用户可调用,中断中可用)
{     
	delay_us(nms*1000);
}
void OS_delayMs(volatile INT32U nms) //OS延时函数(用户可调用,中断中不可用)
{
	while((OS_System.IntNesting != 0) && (OS_System.LockNesting != 0));
	if(nms==0){nms=2;//赋一个比1大的值即可
	OSTCBCur->TaskFlagBit|=(1<<_BIT_TimeLess);} //置位无限等待标志
  else OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeLess);//取消无限等待标志	
	OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//取消计时完成标志
	OSTCBCur->TaskDelayMs=nms+1;
	OSSchedSwitch(); //调度切换任务 
	while(OSTCBCur->TaskDelayMs);
}
void OS_delayDHMS(INT8U day,INT8U hour,INT8U min,INT8U sec)//按日时分秒延时，<=49天17时2分47秒 (用户可调用,中断中不可用)
{
	INT32U time;
	if(day>49) day=49;
	time=(day*24*3600+hour*3600+min*60+sec)*1000;
	OS_delayMs(time);
}
/*---------------------------------------系统延时函数（Page Down)-------=-------------------------------------*/

/**************************************************望穿秋水*****************************************************/
