/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
0.  作者：望穿秋水
1.  
    Keil工程默认参数：Device：STM32F103C8，C/C++ define:：STM32F10X_MD。		                  
		
    Target1文件夹右键->Options for Target -> (也可以点击上方快捷图标"魔法棒")
                       1.Device页面下： 修改成用户实际的芯片型号。
                       2.C/C++ 页面下： Define 处： USE_STDPERIPH_DRIVER,STM32F10X_MD 中的 STM32F10X_MD按实际芯片容量修改
									 
		根据芯片型号修改此处的STM32F10X_MD:
				flash为16~32k:  STM32F10X_LD  (低容量型号)
				flash为64~128k: STM32F10X_MD  (中容量型号) (参考型号：STM32F103C8T6、STM32F103CBT6、STM32F103RBT6)
				flash为>=256k:  STM32F10X_HD  (高容量型号) (参考型号：STM32F103RCT6、STM32F103VCT6、STM32F103VET6、STM32F103ZET6)
	
		芯片容量参考：
				<1>芯片命名规则STM32F103xxxx后面的字符含义:
				    （1）T = 36脚、C = 48脚、R = 64脚、V = 100脚、Z = 144脚
					（2）闪存存储器容量：4=16K 6=32 8=64K B=128K C=256K D=384K E=512K
					
				 <2>（1）使用stlink/v2下载时，Target1右键->Options->
				    （2）Debug页面下选择Use ST-Link Debugger，点击Settings进入二级设置页面。
					（3）Debug页面下设置为Unit:ST-LINK/V2 ,Port:SW ,Download to flash打勾(可选)
				    （4）Flash Download页面下 Reset and run打勾，Programming Algorithm下的容量改成实际芯片相应的容量
					（5）点击编译，编译通过后，点击编译图标右边的LOAD图标进行stlink/v2下载程序到芯片中。
2.  系统提供精确小延时函数:
    delay_us(); 微秒级延时,单位:us微秒 
	  delay_ms(); 毫秒级延时,单位:ms毫秒 		
    任务延时函数：
    OS_delayMs(); 用于任务的延时,单位:ms毫秒  （调用后立即切换至其他任务直到延时间到立即被系统切换回来）
		
		PS:delay_ms()和OS_delayMs()区别：前者是原地等待，后者是延时期间让当前任务暂停运行，把运行时间留给其他任务，延时时间
		                                 到达时会立即获得运行，从而提高运行效率。
		
		如要在其他c文件中使用延时函数，复制以下声明到c或h文件的开头处,或在c文件里包含FSC_STOS.h头文件
		extern void delay_us(unsigned int nus);
		extern void delay_ms(unsigned int nms);		
		
3.  系统提供任务切换锁定函数：(重要)
    OSSchedLock();   任务切换上锁 (某个任务调用上锁后，会获得无限时间的运行权直到解锁为止)
		OSSchedUnlock(); 任务切换解锁	

    运用：某些代码执行过程不能被打断，需要一次性完成时。
		例如：读取DS18B20温度值时，读取DS3231时间数据时，刷新LCD页面时等等情况，在操作之前调用OSSchedLock()上锁当前任务，在操作
		      完成后调用OSSchedUnlock()解锁，这样就能保证操作的完整性。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/	
/*********************************************************
STM32F10x_C51Mode.h功能：

IO口操作方式示例(仅限STM32F10x系列)：

1. PA_OUT; //配置PA口16个IO口全为推挽输出模式
   PB_OUT; //配置PB口16个IO口全为推挽输出模式
	 ...其他同理
	 
	 PA=0x80FF;  //PA口16个IO输出 1000 0000 1111 1111  (PA15---PA0)
	 
	 PA_IN; //配置PA口16个IO口全为上拉输入模式(默认上拉，如需下拉则在PA_IN之后加上PA=0x0000即可)
   PB_IN; //配置PB口16个IO口全为上拉输入模式
	 ...其他同理
	 
	 if(PAin==0xFF00) {  } //读取PA口16个IO的电平,判断PA是否等于0xFF00  
	 
(以下PA4仅为示例，其他引脚同理)

2. PA4_OUT; //初使化PA4 IO口为推挽输出模式
   PA4=1;   // PA4输出高电平
   PA4=0;   // PA4输出低电平

3. PA4_IN;  //初使化PA4 IO口为输入模式 (默认上拉，如需下拉则在PA4_IN之后加上PA4=0即可)	 
	 if(PA4in==0) {  } //读取PA4引脚的电平,判断PA4是否等于0(低电平)
	 if(PA4in==1) {  } //读取PA4引脚的电平,判断PA4是否等于1(高电平)

 其他说明：
     PB3，PB4(JNTRST)，PA13(SWD)，PA14(SWC)，PA15  为JTAG/SWD仿真器的调试接口(上电默认为SW/JTAG功能,不能用作普通IO口)
	 
     SWJ_ON();         ---SW+JTAG功能都打开(上电默认状态) ，               5个都不能用作IO功能
	 SWJ_NOJNTRST();   ---SW+JTAG功能都打开,但是不用JTAG复位引脚(PB4)，    仅PB4用作IO口功能
	 SWON_JTAGOFF();   ---SW功能打开，JTAG功能关闭，                       PB3,PB4,PA15用作普通IO口功能(默认为此设置)
	 SWJ_OFF();        ---SW+JTAG功能都关闭,                               PB3,PB4,PA13,PA14,PA15 全部用作普通IO口功能
*********************************************************/	
	


/*********************************************************
有关系统功能：
1.使用不到Driver文件夹内的驱动时请删除掉：左键分别点击对应驱动.c和.h，分别右键选择Remove File "xxx"

2.添加任务：（添加任务之前请查看fsc_stos.h中最大任务数参数是否满足当前任务数量需要，不满足则需修改）
  
	*添加步骤：(以添加Task13为例,任务名称任意，只要对应的所有名称一致即可。这里为方便，取名为Task13)
	*1.void Task13(void);//任务13声明 
    *2.#define Task13_StkSize 128  //任务13堆栈大小，>=64,(先设一个大的，然后串口查看使用率来修改设定，一般比实际使用大20%左右最佳)
    *3.__align(8) OS_STK Task13_Stk[Task13_StkSize];//任务13堆栈创建
	*4.OSTaskCreate("Task13",Task13,Task13_Stk,Task13_StkSize,TASK_RUNNING,1);//在main()函数里创建任务Task13(加入OS)
	*5.void Task13(void) //任务13实体函数
		{
			 while(1) 
			 {		
				 //printf("Task13 is running\r\n");//示例代码，使用时删除	
				 //OS_delayMs(1000); 				       //示例代码，使用时删除	
			 }
		}
		
	参数：   任务名   任务函数名     任务堆栈       任务堆栈大小       任务状态     任务优先级(数字越大优先级越高，最大65535,最低1)
		    "Task13",  Task13,     Task13_Stk,   Task13_StkSize,   TASK_RUNNING,       1
		
3.系统串口指令: （默认串口1，不使用发送换行符）
    已集成指令：（格式：以//为结束符）
		<1> cmd/Task1/close//             关闭Task1任务，Task1可为其他任务名
		<2> cmd/Task1/open//              打开Task1任务，Task1可为其他任务名
		<3> cmd/Task1/prio=%d//           设置任务优先级,如cmd/Task1/prio=8//
		<4> cmd/osmanage//                任务状态信息
		<5> cmd/runmode/order//           可抢断的顺序运行模式  
        <6> cmd/runmode/prio//            抢断式优先级运行模式  		
		
************************************************/