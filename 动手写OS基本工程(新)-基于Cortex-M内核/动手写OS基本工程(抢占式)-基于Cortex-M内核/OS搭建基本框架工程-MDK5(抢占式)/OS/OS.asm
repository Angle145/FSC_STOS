;说明：M0内核仅支持直接访问CPU寄存器R0-R7，R8-R15必须通过R0-R8作为中间媒介来操作,该方法适用所有M内核。
;M3及M3以上支持直接访问R0-R15，该方法仅适用M3及以上内核。

;阅读以下代码时，需要配合Cortex-M0(M3)权威指南书，了解寄存器组结构和作用，掌握Cortex-M汇编语言基础。
Cortex_M0     EQU   0              ;can be used to Cortex-M ALL      //等价C语言：#define Cortex_M0 0
Cortex_M3     EQU   1              ;can be used to without Cortex-M0 //           #define Cortex_M3 1	
	
;*---------------------内核选择(Kernel Set)-------------------*	
;Kernel    EQU Cortex_M0	            ;for stm32f0xx                   //#define Kernel Cortex_M0
Kernel    EQU Cortex_M3	            ;for stm32f1xx                   //#define Kernel Cortex_M3
;*------------------------------------------------------------*

    IMPORT	  OSSysTickInit    ;//外部函数声明, 等价C语言：void OSSysTickInit(void);
    IMPORT    OSPendSVInit     ;//                         void OSPendSVInit(void);
	IMPORT    OSPendSVPulse	   ;//                         void OSPendSVPulse(void);
	IMPORT    OSTCBCur         ;//外部变量声明:            extern OS_TCB *OSTCBCur;
    IMPORT    OSTCBNext        ;//                         extern OS_TCB *OSTCBNext;
	   
    EXPORT    OS_CORE_ENTER    ;//内部函数向外部文件声明, 等价C语言：void OS_CORE_ENTER(void);    
    EXPORT    OS_CORE_EXIT     ;//                                   void OS_CORE_EXIT(void); 
    EXPORT    PendSV_Handler   ;//                                   void PendSV_Handler(void); 
	EXPORT	  OSStartUp	       ;//                                   void OSStartUp(void); 
    PRESERVE8                  ;//字节对齐关键词,指定当前文件八字节对齐。
    AREA |.text|, CODE, READONLY ;//定义一个代码段或数据段。
    THUMB                      ;//指定以下指令都是THUMB指令集(ARM汇编有多种指令集)
OS_CORE_ENTER                  ;//进入临界代码区标号，等价C语言函数void OS_CORE_ENTER(void){}
    CPSID    I                 ;//关闭全局中断(Cortex-M内核的全局中断开关指令)
    BX    LR                   ;//等价C语言：return
OS_CORE_EXIT                   ;//void OS_CORE_EXIT(void){}
    CPSIE    I                 ;//打开全局中断
    BX    LR                   ;//return                                 
OSStartUp                      ;//void OSStartUp(void){}      
    CPSID    I                 ;//关闭全局中断
    LDR     R0,=OSPendSVInit   ;//等价C语言：运行OSPendSVInit(); 函数,PendSV初使化  
	BLX     R0                 ;//return
    LDR     R0,=OSSysTickInit  ;//同理, SysTick定时器初使化    
	BLX     R0	               ;//同理  
    LDR R4,=0x0                ;//R4装载立即数0(不直接给PSP赋值0而是经进R寄存器作为媒介是因为PSP只能和R寄存器打交道)            
    MSR     PSP, R4            ;//PSP(process stack pointer)程序堆栈指针赋值0。PSP属用户级(特级权下为MSP)，双堆栈结构。   
    LDR     R0,=OSPendSVPulse  ;//OSPendSVPulse();触发一次PendSV中断(全局中断没打开，会悬挂起)    
	BLX     R0                 ;//return
    CPSIE    I                 ;//打开全局中断(此时若没有其他中断在响应，则立即进入PendSV中断函数)    
OSStartPending                 ;//原地等待标号。      
    B    OSStartPending        ;//返回OSStartPending，即原地等待,等价C语言：while(1); //等待进入PendSV中断函数
PendSV_Handler                 ;//PendSV中断函数(移植时需要注释原工程的PendSV_Handler中断函数)
    CPSID    I                 ;//关闭全局中断    
    MRS     R0, PSP            ;//读取PSP堆栈指针的值到R0
;+++++++++++++++++++++Save Context Code(PageUp)+++++++++++++++++++;//任务环境保存开始

    IF (Kernel == Cortex_M0)   ;//条件编译,等价C语言：#if	
;------------------------Cortex_M0 Code	Start---------------------; //M0内核有关代码开始
	CMP     R0 , #0x00         ;//判断R0是否等0，等价C语言：if(R0==0)
	BEQ     OSFirstEnter 	   ;//判断为真则跳转OSFirstEnter标号，等价C语言：goto OSFirstEnter;//跳转标号
    
	;//说明：PendSV中断机制：进入中断前，会自动将xPSR,PC,LR,R12,R3-R0压栈，所以这里只需要保存R4-R11的值。
	;//由于被自动压栈，运行到这里PSP的值实际上已经相对任务栈数组地址最大值TaskStk[N-1](栈顶)向下偏移了8个寄存器
    ;//长度值(PSP入栈是向下生长)，再保存R4-R11共8个寄存器时，PSP总共偏移栈顶16个寄存器长度值。
    ;//运行任务程序时，遇到函数调用或运算需要保存中间过程变量等情况时，PSP就在此基础上进行入栈和出栈操作，
    ;//不会影响到栈顶到栈顶-16这段内栈的值。特别说明下，并非所有代码运行时都用到R4-R12,经常用到的只有R0-R3，所
    ;//有时R4-R12是用不到的，虽然上下文切换时也保存和恢复了它们，但是它们的值可能一直为0，这样在计算任务栈使用率
    ;//时就会显示这段内存是空闲的，所以有时任务栈可能只用到几个必要的寄存器xPSR,PC,LR,R12,R3-R0。	
	;//先保存R4-R7是因为R8-R11要通过R4-R7为媒介才能操作，为了不破环R4-R7的值，所以先保存R4-R7的值,当然，也可以
	;//先备份R4-R7的值再先保存R8-R11，然后再保存R4-R7的值，无论哪种方法，只要保证保存顺序和恢复顺序相对应即可。
    SUBS    R0 , R0 , #0x20    ;//R0=R0-0x20  (地址先偏移8个寄存器的长度(32byte)，寄存器为4byte=32bit)	
    STR     R4 , [R0]          ;//保存R4的值到内存(R0的值为该内存的地址)     
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04  (地址指向下一个4byte空间，准备保存新的寄存器值)     
    STR     R5 , [R0]          ;//保存R5的值到内存     
    ADDS    R0 , R0, #0x4      ;//R0=R0+0x04     
    STR     R6 , [R0]          ;//保存R6的值到内存      
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04       
    STR     R7 , [R0]          ;//保存R7的值到内存       
    ADDS    R0 , R0 , #0x4  	
	;M0 support code
	;M0 is disable to operate R8-R15 
	MOV     R4 , R8            ;//复制R8的值到R4，(M0内核不可以直接操作R8-R15)
    STR     R4 , [R0]          ;//保存R4的值到内存(实质是保存R8的值到内存)     
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04       
	MOV     R5 , R9            ;//复制R9的值到R5
    STR     R5 , [R0]          ;//保存R5的值到内存(实质是保存R9的值到内存)       
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04       
	MOV     R6 , R10           ;//复制R10的值到R6
    STR     R6 , [R0]          ;//保存R6的值到内存(实质是保存R10的值到内存)       
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04       
    MOV     R7 , R11	       ;//复制R11的值到R7
    STR     R7 , [R0]          ;//保存R7的值到内存(实质是保存R11的值到内存)       
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04 
    ;M0 support code 	
    SUBS    R0 , R0 , #0x20    ;//R0=R0-0x20 (由于经过保存R4-R11的值,R0=R0+0x04执行8次，
	                           ;//地址又回到了原地址，所以又-0x20让R0指向堆栈保存数据后的位置)
;------------------------Cortex_M0 Code End-----------------------;	//M0内核有关代码结束

	ELSE                       ;//#else
    IF (Kernel == Cortex_M3)   ;//#if	   
		
;------------------------Cortex_M3 Code Start---------------------;	//M3及以上内核有关代码开始	
    CBZ     R0, OSFirstEnter   ;//判断R0是否等0,等于就直接跳转OSFirstEnter,此指令M3及以上才有,比CMP快捷。
	                           ;//等价C语言：if(R0==0){goto OSFirstEnter;}
	
    SUB     R0, R0, #0x20  	   ;//R0=R0-0x20
    STM     R0, {R4-R11}       ;//批量保存R0-R11的值到内存(以R0为首地址)，方便快捷,M0不支持。
;------------------------Cortex_M3 Code End-----------------------;	//M3及以上内核有关代码结束	
	ENDIF                      ;//#endif
	ENDIF                      ;//#endif
;+++++++++++++++++++Save Context Code(PageDown)+++++++++++++++++++;	//任务环境保存结束	
	
    LDR     R1, =OSTCBCur      ;//读取OSTCBCur指针值到R1，不用R0是因此时R0上有保存R寄存器后的内存地址。
                               ;//实际上结构体名==首地址==第一个成员变量的地址，而TCB结构体中，第一个变
	                           ;//量成员为任务堆栈指针，所以读取OSTCBCur等价于读取任务堆栈指针值到R1，
							   ;//所以要求结构体第一个成员必须是任务堆栈指针变量，否则出错)   
    LDR     R1, [R1]           ;//读出以R1上的值为地址的内存数据，保存到R1中    
    STR     R0, [R1]           ;//保存R0的值到内存(以R1的值为地址的内存), 总体说就是更新一下TCB上的堆栈指针变量的值    
OSFirstEnter                   ;//第一次进入系统标号(第一次进入系统，不需要保存R寄存器值，因为此时的值是无用的)
    LDR     R0, =OSTCBCur      ;//读取OSTCBCur指针值到R0    
    LDR     R1, =OSTCBNext     ;//读取OSTCBNext指针值到R1  
    LDR     R2 , [R1]          ;//读取内存数据到R2(以R1为地址的内存)//R2=OSTCBNext->StkPtr;     
    STR     R2 , [R0]          ;//保存R2数据内存到(以R0为地址的内存)//OSTCBCur->StkPtr=OSTCBNext->StkPtr;//切换堆栈指针  
	
    LDR     R0 , [R2]          ;//读取内存数据到R0(以R2为地址的内存)//R0=OSTCBNext->StkPtr;
;+++++++++++++++++++++Read Context Code(PageUp)+++++++++++++++++++;//任务环境恢复开始

    IF (Kernel == Cortex_M0)   ;//#if
;------------------------Cortex_M0 Code	Start---------------------;//M0内核有关代码开始

	
    LDR     R4 , [R0]          ;//读取内存数据到R4(以R0为地址的内存)//恢复R4的值 
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04    
    LDR     R5 , [R0]          ;//恢复R5的值    
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04     
    LDR     R6 , [R0]          ;//恢复R6的值    
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04    
    LDR     R7 , [R0]             
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04
    ;M0 support code 
	PUSH    {R4 - R7}          ;//把R4-R7的值压栈    
    LDR     R4 , [R0]          ;//读取任务栈中R8的值到R4   
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04    
	MOV     R8 , R4            ;//复制R4的值到R8
    LDR     R5 , [R0]          ;//读取任务栈中R9的值到R5   
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04    
	MOV     R9 , R5            ;//复制R5的值到R9
    LDR     R6 , [R0]          ;//读取任务栈中R10的值到R6   
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04    
	MOV     R10, R6            ;//复制R6的值到R10 
    LDR     R7 , [R0]          ;//读取任务栈中R11的值到R7   
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04     
	MOV     R11, R7            ;//复制R7的值到R11 
	POP    {R4 - R7}           ;//R4-R7的值出栈(出栈后自动装载到R4-R7)
    ;M0 support code 
		
    MSR     PSP, R0            ;//读取R0的值到PSP
    MOVS    R1 , #0x04         ;//R1赋值0x04
	MOV     R2 , LR            ;//读取LR寄存器的值到R2(LR即R14寄存器，该寄存器保存着函数跳转前的下一地址，即返回地址)
    ORRS    R2 , R2 , R1       ;//R2=R2|R1;即把LR寄存器第3位置1，从进程堆栈中做出栈操作，返回后使用PSP。
	MOV     LR , R2            ;//把R2的值赋给LR。因为M0或运算指令不支持在LR寄存器上操作，所以引入了R1,R2作为中间变量。
;------------------------Cortex_M0 Code End-----------------------;//M0内核有关代码结束
    ELSE                       ;//#else
    IF (Kernel == Cortex_M3)   ;//#if	
;------------------------Cortex_M3 Code Start---------------------;//M3及以上内核有关代码开始	
	LDM     R0, {R4-R11}       ;//批量从内存读取R4-R11的值到R4-R11寄存器    
	ADD     R0, R0, #0x20      ;//R0=R0+0x20 (即8个0x04)

    MSR     PSP, R0            ;//读取R0的值到PSP    
    ORR     LR, LR, #0x04      ;//LR(R14)寄存器或上0x04(LR寄存器第3位置1，从进程堆栈中做出栈操作，返回后使用PSP)
;------------------------Cortex_M3 Code End-----------------------;//M3及以上内核有关代码结束	
    ENDIF	                   ;//#endif
    ENDIF	                   ;//#endif
;+++++++++++++++++++Read Context Code(PageDown)+++++++++++++++++++;//任务环境恢复结束

    CPSIE     I                ;//打开全局中断    
    BX    LR                   ;//返回(此时PC的值为某个任务函数首地址(也可能是任务函数中间的某个地址))    
    align 4                    ;//内存对齐指令，以4个字节(32位)对齐
    end                        ;//伪指令,放在程序行的最后,告诉编译器编译程序到此结束
    ;by Angle_145    
		