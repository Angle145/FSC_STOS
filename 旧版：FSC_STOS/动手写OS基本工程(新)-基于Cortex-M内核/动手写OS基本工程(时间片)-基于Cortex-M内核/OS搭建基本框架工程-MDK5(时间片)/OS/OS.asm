;˵����M0�ں˽�֧��ֱ�ӷ���CPU�Ĵ���R0-R7��R8-R15����ͨ��R0-R8��Ϊ�м�ý��������,�÷�����������M�ںˡ�
;M3��M3����֧��ֱ�ӷ���R0-R15���÷���������M3�������ںˡ�

;�Ķ����´���ʱ����Ҫ���Cortex-M0(M3)Ȩ��ָ���飬�˽�Ĵ�����ṹ�����ã�����Cortex-M������Ի�����
Cortex_M0     EQU   0              ;can be used to Cortex-M ALL      //�ȼ�C���ԣ�#define Cortex_M0 0
Cortex_M3     EQU   1              ;can be used to without Cortex-M0 //           #define Cortex_M3 1	
	
;*---------------------�ں�ѡ��(Kernel Set)-------------------*	
;Kernel    EQU Cortex_M0	            ;for stm32f0xx                   //#define Kernel Cortex_M0
Kernel    EQU Cortex_M3	            ;for stm32f1xx                   //#define Kernel Cortex_M3
;*------------------------------------------------------------*

    IMPORT	  OSSysTickInit    ;//�ⲿ��������, �ȼ�C���ԣ�void OSSysTickInit(void);
    IMPORT    OSPendSVInit     ;//                         void OSPendSVInit(void);
	IMPORT    OSPendSVPulse	   ;//                         void OSPendSVPulse(void);
	IMPORT    OSTCBCur         ;//�ⲿ��������:            extern OS_TCB *OSTCBCur;
    IMPORT    OSTCBNext        ;//                         extern OS_TCB *OSTCBNext;
	   
    EXPORT    OS_CORE_ENTER    ;//�ڲ��������ⲿ�ļ�����, �ȼ�C���ԣ�void OS_CORE_ENTER(void);    
    EXPORT    OS_CORE_EXIT     ;//                                   void OS_CORE_EXIT(void); 
    EXPORT    PendSV_Handler   ;//                                   void PendSV_Handler(void); 
	EXPORT	  OSStartUp	       ;//                                   void OSStartUp(void); 
    PRESERVE8                  ;//�ֽڶ���ؼ���,ָ����ǰ�ļ����ֽڶ��롣
    AREA |.text|, CODE, READONLY ;//����һ������λ����ݶΡ�
    THUMB                      ;//ָ������ָ���THUMBָ�(ARM����ж���ָ�)
OS_CORE_ENTER                  ;//�����ٽ��������ţ��ȼ�C���Ժ���void OS_CORE_ENTER(void){}
    CPSID    I                 ;//�ر�ȫ���ж�(Cortex-M�ں˵�ȫ���жϿ���ָ��)
    BX    LR                   ;//�ȼ�C���ԣ�return
OS_CORE_EXIT                   ;//void OS_CORE_EXIT(void){}
    CPSIE    I                 ;//��ȫ���ж�
    BX    LR                   ;//return                                 
OSStartUp                      ;//void OSStartUp(void){}      
    CPSID    I                 ;//�ر�ȫ���ж�
    LDR     R0,=OSPendSVInit   ;//�ȼ�C���ԣ�����OSPendSVInit(); ����,PendSV��ʹ��  
	BLX     R0                 ;//return
    LDR     R0,=OSSysTickInit  ;//ͬ��, SysTick��ʱ����ʹ��    
	BLX     R0	               ;//ͬ��  
    LDR R4,=0x0                ;//R4װ��������0(��ֱ�Ӹ�PSP��ֵ0���Ǿ���R�Ĵ�����Ϊý������ΪPSPֻ�ܺ�R�Ĵ����򽻵�)            
    MSR     PSP, R4            ;//PSP(process stack pointer)�����ջָ�븳ֵ0��PSP���û���(�ؼ�Ȩ��ΪMSP)��˫��ջ�ṹ��   
    LDR     R0,=OSPendSVPulse  ;//OSPendSVPulse();����һ��PendSV�ж�(ȫ���ж�û�򿪣���������)    
	BLX     R0                 ;//return
    CPSIE    I                 ;//��ȫ���ж�(��ʱ��û�������ж�����Ӧ������������PendSV�жϺ���)    
OSStartPending                 ;//ԭ�صȴ���š�      
    B    OSStartPending        ;//����OSStartPending����ԭ�صȴ�,�ȼ�C���ԣ�while(1); //�ȴ�����PendSV�жϺ���
PendSV_Handler                 ;//PendSV�жϺ���(��ֲʱ��Ҫע��ԭ���̵�PendSV_Handler�жϺ���)
    CPSID    I                 ;//�ر�ȫ���ж�    
    MRS     R0, PSP            ;//��ȡPSP��ջָ���ֵ��R0
;+++++++++++++++++++++Save Context Code(PageUp)+++++++++++++++++++;//���񻷾����濪ʼ

    IF (Kernel == Cortex_M0)   ;//��������,�ȼ�C���ԣ�#if	
;------------------------Cortex_M0 Code	Start---------------------; //M0�ں��йش��뿪ʼ
	CMP     R0 , #0x00         ;//�ж�R0�Ƿ��0���ȼ�C���ԣ�if(R0==0)
	BEQ     OSFirstEnter 	   ;//�ж�Ϊ������תOSFirstEnter��ţ��ȼ�C���ԣ�goto OSFirstEnter;//��ת���
    
	;//˵����PendSV�жϻ��ƣ������ж�ǰ�����Զ���xPSR,PC,LR,R12,R3-R0ѹջ����������ֻ��Ҫ����R4-R11��ֵ��
	;//���ڱ��Զ�ѹջ�����е�����PSP��ֵʵ�����Ѿ��������ջ�����ַ���ֵTaskStk[N-1](ջ��)����ƫ����8���Ĵ���
    ;//����ֵ(PSP��ջ����������)���ٱ���R4-R11��8���Ĵ���ʱ��PSP�ܹ�ƫ��ջ��16���Ĵ�������ֵ��
    ;//�����������ʱ�������������û�������Ҫ�����м���̱��������ʱ��PSP���ڴ˻����Ͻ�����ջ�ͳ�ջ������
    ;//����Ӱ�쵽ջ����ջ��-16�����ջ��ֵ���ر�˵���£��������д�������ʱ���õ�R4-R12,�����õ���ֻ��R0-R3����
    ;//��ʱR4-R12���ò����ģ���Ȼ�������л�ʱҲ����ͻָ������ǣ��������ǵ�ֵ����һֱΪ0�������ڼ�������ջʹ����
    ;//ʱ�ͻ���ʾ����ڴ��ǿ��еģ�������ʱ����ջ����ֻ�õ�������Ҫ�ļĴ���xPSR,PC,LR,R12,R3-R0��	
	;//�ȱ���R4-R7����ΪR8-R11Ҫͨ��R4-R7Ϊý����ܲ�����Ϊ�˲��ƻ�R4-R7��ֵ�������ȱ���R4-R7��ֵ,��Ȼ��Ҳ����
	;//�ȱ���R4-R7��ֵ���ȱ���R8-R11��Ȼ���ٱ���R4-R7��ֵ���������ַ�����ֻҪ��֤����˳��ͻָ�˳�����Ӧ���ɡ�
    SUBS    R0 , R0 , #0x20    ;//R0=R0-0x20  (��ַ��ƫ��8���Ĵ����ĳ���(32byte)���Ĵ���Ϊ4byte=32bit)	
    STR     R4 , [R0]          ;//����R4��ֵ���ڴ�(R0��ֵΪ���ڴ�ĵ�ַ)     
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04  (��ַָ����һ��4byte�ռ䣬׼�������µļĴ���ֵ)     
    STR     R5 , [R0]          ;//����R5��ֵ���ڴ�     
    ADDS    R0 , R0, #0x4      ;//R0=R0+0x04     
    STR     R6 , [R0]          ;//����R6��ֵ���ڴ�      
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04       
    STR     R7 , [R0]          ;//����R7��ֵ���ڴ�       
    ADDS    R0 , R0 , #0x4  	
	;M0 support code
	;M0 is disable to operate R8-R15 
	MOV     R4 , R8            ;//����R8��ֵ��R4��(M0�ں˲�����ֱ�Ӳ���R8-R15)
    STR     R4 , [R0]          ;//����R4��ֵ���ڴ�(ʵ���Ǳ���R8��ֵ���ڴ�)     
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04       
	MOV     R5 , R9            ;//����R9��ֵ��R5
    STR     R5 , [R0]          ;//����R5��ֵ���ڴ�(ʵ���Ǳ���R9��ֵ���ڴ�)       
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04       
	MOV     R6 , R10           ;//����R10��ֵ��R6
    STR     R6 , [R0]          ;//����R6��ֵ���ڴ�(ʵ���Ǳ���R10��ֵ���ڴ�)       
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04       
    MOV     R7 , R11	       ;//����R11��ֵ��R7
    STR     R7 , [R0]          ;//����R7��ֵ���ڴ�(ʵ���Ǳ���R11��ֵ���ڴ�)       
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04 
    ;M0 support code 	
    SUBS    R0 , R0 , #0x20    ;//R0=R0-0x20 (���ھ�������R4-R11��ֵ,R0=R0+0x04ִ��8�Σ�
	                           ;//��ַ�ֻص���ԭ��ַ��������-0x20��R0ָ���ջ�������ݺ��λ��)
;------------------------Cortex_M0 Code End-----------------------;	//M0�ں��йش������

	ELSE                       ;//#else
    IF (Kernel == Cortex_M3)   ;//#if	   
		
;------------------------Cortex_M3 Code Start---------------------;	//M3�������ں��йش��뿪ʼ	
    CBZ     R0, OSFirstEnter   ;//�ж�R0�Ƿ��0,���ھ�ֱ����תOSFirstEnter,��ָ��M3�����ϲ���,��CMP��ݡ�
	                           ;//�ȼ�C���ԣ�if(R0==0){goto OSFirstEnter;}
	
    SUB     R0, R0, #0x20  	   ;//R0=R0-0x20
    STM     R0, {R4-R11}       ;//��������R0-R11��ֵ���ڴ�(��R0Ϊ�׵�ַ)��������,M0��֧�֡�
;------------------------Cortex_M3 Code End-----------------------;	//M3�������ں��йش������	
	ENDIF                      ;//#endif
	ENDIF                      ;//#endif
;+++++++++++++++++++Save Context Code(PageDown)+++++++++++++++++++;	//���񻷾��������	
	
    LDR     R1, =OSTCBCur      ;//��ȡOSTCBCurָ��ֵ��R1������R0�����ʱR0���б���R�Ĵ�������ڴ��ַ��
                               ;//ʵ���Ͻṹ����==�׵�ַ==��һ����Ա�����ĵ�ַ����TCB�ṹ���У���һ����
	                           ;//����ԱΪ�����ջָ�룬���Զ�ȡOSTCBCur�ȼ��ڶ�ȡ�����ջָ��ֵ��R1��
							   ;//����Ҫ��ṹ���һ����Ա�����������ջָ��������������)   
    LDR     R1, [R1]           ;//������R1�ϵ�ֵΪ��ַ���ڴ����ݣ����浽R1��    
    STR     R0, [R1]           ;//����R0��ֵ���ڴ�(��R1��ֵΪ��ַ���ڴ�), ����˵���Ǹ���һ��TCB�ϵĶ�ջָ�������ֵ    
OSFirstEnter                   ;//��һ�ν���ϵͳ���(��һ�ν���ϵͳ������Ҫ����R�Ĵ���ֵ����Ϊ��ʱ��ֵ�����õ�)
    LDR     R0, =OSTCBCur      ;//��ȡOSTCBCurָ��ֵ��R0    
    LDR     R1, =OSTCBNext     ;//��ȡOSTCBNextָ��ֵ��R1  
    LDR     R2 , [R1]          ;//��ȡ�ڴ����ݵ�R2(��R1Ϊ��ַ���ڴ�)//R2=OSTCBNext->StkPtr;     
    STR     R2 , [R0]          ;//����R2�����ڴ浽(��R0Ϊ��ַ���ڴ�)//OSTCBCur->StkPtr=OSTCBNext->StkPtr;//�л���ջָ��  
	
    LDR     R0 , [R2]          ;//��ȡ�ڴ����ݵ�R0(��R2Ϊ��ַ���ڴ�)//R0=OSTCBNext->StkPtr;
;+++++++++++++++++++++Read Context Code(PageUp)+++++++++++++++++++;//���񻷾��ָ���ʼ

    IF (Kernel == Cortex_M0)   ;//#if
;------------------------Cortex_M0 Code	Start---------------------;//M0�ں��йش��뿪ʼ

	
    LDR     R4 , [R0]          ;//��ȡ�ڴ����ݵ�R4(��R0Ϊ��ַ���ڴ�)//�ָ�R4��ֵ 
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04    
    LDR     R5 , [R0]          ;//�ָ�R5��ֵ    
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04     
    LDR     R6 , [R0]          ;//�ָ�R6��ֵ    
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04    
    LDR     R7 , [R0]             
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04
    ;M0 support code 
	PUSH    {R4 - R7}          ;//��R4-R7��ֵѹջ    
    LDR     R4 , [R0]          ;//��ȡ����ջ��R8��ֵ��R4   
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04    
	MOV     R8 , R4            ;//����R4��ֵ��R8
    LDR     R5 , [R0]          ;//��ȡ����ջ��R9��ֵ��R5   
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04    
	MOV     R9 , R5            ;//����R5��ֵ��R9
    LDR     R6 , [R0]          ;//��ȡ����ջ��R10��ֵ��R6   
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04    
	MOV     R10, R6            ;//����R6��ֵ��R10 
    LDR     R7 , [R0]          ;//��ȡ����ջ��R11��ֵ��R7   
    ADDS    R0 , R0 , #0x4     ;//R0=R0+0x04     
	MOV     R11, R7            ;//����R7��ֵ��R11 
	POP    {R4 - R7}           ;//R4-R7��ֵ��ջ(��ջ���Զ�װ�ص�R4-R7)
    ;M0 support code 
		
    MSR     PSP, R0            ;//��ȡR0��ֵ��PSP
    MOVS    R1 , #0x04         ;//R1��ֵ0x04
	MOV     R2 , LR            ;//��ȡLR�Ĵ�����ֵ��R2(LR��R14�Ĵ������üĴ��������ź�����תǰ����һ��ַ�������ص�ַ)
    ORRS    R2 , R2 , R1       ;//R2=R2|R1;����LR�Ĵ�����3λ��1���ӽ��̶�ջ������ջ���������غ�ʹ��PSP��
	MOV     LR , R2            ;//��R2��ֵ����LR����ΪM0������ָ�֧����LR�Ĵ����ϲ���������������R1,R2��Ϊ�м������
;------------------------Cortex_M0 Code End-----------------------;//M0�ں��йش������
    ELSE                       ;//#else
    IF (Kernel == Cortex_M3)   ;//#if	
;------------------------Cortex_M3 Code Start---------------------;//M3�������ں��йش��뿪ʼ	
	LDM     R0, {R4-R11}       ;//�������ڴ��ȡR4-R11��ֵ��R4-R11�Ĵ���    
	ADD     R0, R0, #0x20      ;//R0=R0+0x20 (��8��0x04)

    MSR     PSP, R0            ;//��ȡR0��ֵ��PSP    
    ORR     LR, LR, #0x04      ;//LR(R14)�Ĵ�������0x04(LR�Ĵ�����3λ��1���ӽ��̶�ջ������ջ���������غ�ʹ��PSP)
;------------------------Cortex_M3 Code End-----------------------;//M3�������ں��йش������	
    ENDIF	                   ;//#endif
    ENDIF	                   ;//#endif
;+++++++++++++++++++Read Context Code(PageDown)+++++++++++++++++++;//���񻷾��ָ�����

    CPSIE     I                ;//��ȫ���ж�    
    BX    LR                   ;//����(��ʱPC��ֵΪĳ���������׵�ַ(Ҳ�������������м��ĳ����ַ))    
    align 4                    ;//�ڴ����ָ���4���ֽ�(32λ)����
    end                        ;//αָ��,���ڳ����е����,���߱�����������򵽴˽���
    ;by Angle_145    
		