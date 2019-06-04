/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
0.  作者：望穿秋水

1.  系统提供精确小延时函数:
        delay_us(); 微秒级延时,单位:us微秒 
	    delay_ms(); 毫秒级延时,单位:ms毫秒 		
    任务延时函数：
        OS_delayMs(); 用于任务的延时,单位:ms毫秒  （调用后立即切换至其他任务直到延时间到立即被系统切换回来）
		
		PS:delay_ms()和OS_delayMs()区别：前者是原地等待，后者是延时期间让当前任务暂停运行，把运行时间留给其他任务，延时时间
		                                 到达时会立即获得运行，从而提高运行效率。
		
		如要在其他c文件中使用延时函数，复制以下声明到c或h文件的开头处,或在c文件里包含FSC_STOS.h头文件
		extern void delay_us(unsigned int nus);
		extern void delay_ms(unsigned int nms);		
		
2.  系统提供任务切换锁定函数：(重要)
        OSSchedLock();   任务切换上锁 (某个任务调用上锁后，会获得无限时间的运行权直到解锁为止)
		OSSchedUnlock(); 任务切换解锁	

    运用：某些代码执行过程不能被打断，需要一次性完成时。
		例如：读取DS18B20温度值时，读取DS3231时间数据时，刷新LCD页面时等等情况，在操作之前调用OSSchedLock()上锁当前任务，在操作
		      完成后调用OSSchedUnlock()解锁，这样就能保证操作的完整性。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/	

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
有关系统功能：
									 
1.使用不到Driver文件夹内的驱动时请删除掉：左键分别点击对应驱动.c和.h，分别右键选择Remove File "xxx"

2.添加任务：（添加任务之前请查看fsc_stos.h中最大任务数参数是否满足当前任务数量需要，不满足则需修改）
  
	*添加步骤：(以添加Task13为例,任务名称任意，只要对应的所有名称一致即可。这里为方便，取名为Task13)
	*1.void Task13(void);//任务13声明 
    *2.#define Task13_StkSize 128  //任务13堆栈大小，>=64,(先设一个大的，然后串口查看使用率来修改设定，一般比实际使用大20%左右最佳)
    *3.OSSTK Task13_Stk[Task13_StkSize];//任务13堆栈创建
	*4.OSTaskCreate("Task13",Task13,Task13_Stk,Task13_StkSize,TASK_RUNNING,1);//在main()函数里创建任务Task13(加入OS)
	*5.void Task13(void) //任务13实体函数
		{
			 while(1) 
			 {		
				 //printf("Task13 is running\r\n");//示例代码，使用时删除	
				 //OS_delayMs(1000); 				       //示例代码，使用时删除	
			 }
		}
		
	参数：   任务名   任务函数名    任务堆栈      任务堆栈大小      任务状态     任务优先级(数字越大优先级越高，最大65535,最低1)
		    "Task13",  Task13,     Task13_Stk,   Task13_StkSize,   TASK_RUNNING,       1
		
3.系统串口指令: （默认串口1）
    已集成指令：（格式：以//为结束符）
		<1> cmd/Task1/close//             关闭Task1任务，Task1可为其他任务名
		<2> cmd/Task1/open//              打开Task1任务，Task1可为其他任务名
		<3> cmd/Task1/prio=%d//           设置任务优先级,如cmd/Task1/prio=8//
		<4> cmd/osmanage//                任务状态信息
		<5> cmd/runmode/order//           可抢断的顺序运行模式  
        <6> cmd/runmode/prio//            抢断式优先级运行模式 

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/



通用版FSC_STOS支持STM32F4 FPU设置：

STM32F4 FPU浮点运器，为支持FPU参与运算，将通用版的FSC_STOS作以下修改

一、MDK工程设置(点击魔法棒)

1.Target处 Floating Point Hardware: 设为  Use Single Precision

2.C/C++处 Define:(复制以下填到Define处)
    USE_STDPERIPH_DRIVER,STM32F429_439xx,ARM_MATH_CM4,__CC_ARM,ARM_MATH_MATRIX_CHECK,ARM_MATH_ROUNDING,__FPU_PRESENT=1


二、修改启动文件

1.startup_stm32f429_439x.s启动文件(可以为其他型号F4的启动文件)

原文件处：
; Reset handler
Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
                 IMPORT  SystemInit
                 IMPORT  __main

                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP

修改：用以下内容替换上面内容
; Reset handler
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main

                IF {FPU} != "SoftVFP"
                                                ; Enable Floating Point Support at reset for FPU
                LDR.W   R0, =0xE000ED88         ; Load address of CPACR register
                LDR     R1, [R0]                ; Read value at CPACR
                ORR     R1,  R1, #(0xF <<20)    ; Set bits 20-23 to enable CP10 and CP11 coprocessors
                                                ; Write back the modified CPACR value
                STR     R1, [R0]                ; Wait for store to complete
                DSB
                
                                                ; Disable automatic FP register content
                                                ; Disable lazy context switch
                LDR.W   R0, =0xE000EF34         ; Load address to FPCCR register
                LDR     R1, [R0]
                AND     R1,  R1, #(0x3FFFFFFF)  ; Clear the LSPEN and ASPEN bits
                STR     R1, [R0]
                ISB                             ; Reset pipeline now the FPU is enabled
                ENDIF
				 
				 
				 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP
				 				 