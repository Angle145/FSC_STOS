Cortex_M0     EQU   0              ;can be used to M0°¢M3°¢M4  (without FPU in M4)
Cortex_M0_FPU EQU   1	 
Cortex_M3     EQU   2              ;can be used to M3°¢M4  (without FPU in M4)
Cortex_M3_FPU EQU   3
Cortex_M4     EQU   4              ;can be used to M4  (with FPU in M4)
Cortex_M4_FPU EQU   5	
	
;*---------------------ƒ⁄∫À…Ë÷√(Kernel Set)-------------------*	
Kernel    EQU Cortex_M0	            ;for stm32f0xx
;Kernel    EQU Cortex_M0_FPU	        ;for stm32f0xx	
;Kernel    EQU Cortex_M3	            ;for stm32f1xx
;Kernel    EQU Cortex_M3_FPU	        ;for stm32f1xx
;Kernel    EQU Cortex_M4	            ;for stm32f4xx 	
;Kernel    EQU Cortex_M4_FPU	        ;for stm32f4xx 
;*------------------------------------------------------------*


    IMPORT    OSTCBRun
    IMPORT    OSTCBCur            
	IMPORT    OSPendSVPulse 
    EXPORT    OS_INT_ENTER        
    EXPORT    OS_INT_EXIT
    EXPORT    PendSV_Handler
	EXPORT	  OSStartUp	
    PRESERVE8 
    AREA |.text|, CODE, READONLY
    THUMB         
OS_INT_ENTER     
    CPSID    I   
    BX    LR    
OS_INT_EXIT      
    CPSIE    I   
    BX    LR                                 
OSStartUp                            
    CPSID    I                                    
    LDR R4,=0x0                  
    MSR    PSP, R4                 
    LDR     R0,=OSPendSVPulse      
	BLX     R0
    CPSIE    I                     
OSStartPending                        
    B    OSStartPending  
PendSV_Handler
    CPSID    I                     
    MRS     R0, PSP                  
	IF ((Kernel == Cortex_M0)||(Kernel == Cortex_M0_FPU))	
;------------------------Cortex_M0 Code	Start---------------------;
	CMP     R0 , #0x00
	BEQ     OSFirstEnter 	
    
	IF (Kernel == Cortex_M0_FPU)
   ;FPU support code	  
    SUBS    R0, R0, #0X40 
    VSTM    R0, {S16-S31}	
   ;FPU support code
    ENDIF
  
    SUBS    R0 , R0 , #0x20  	
    STR     R4 , [R0]               
    ADDS    R0 , R0 , #0x4           
    STR     R5 , [R0]               
    ADDS    R0 , R0, #0x4           
    STR     R6 , [R0]               
    ADDS    R0 , R0 , #0x4           
    STR     R7 , [R0]               
    ADDS    R0 , R0 , #0x4  	
	;M0 support code
	;M0 is disable to operate R8-R15 
	MOV     R4 , R8
    STR     R4 , [R0]               
    ADDS    R0 , R0 , #0x4           
	MOV     R5 , R9
    STR     R5 , [R0]                
    ADDS    R0 , R0 , #0x4           
	MOV     R6 , R10
    STR     R6 , [R0]                
    ADDS    R0 , R0 , #0x4           
    MOV     R7 , R11	
    STR     R7 , [R0]                
    ADDS    R0 , R0 , #0x4  
    ;M0 support code 	
    SUBS    R0 , R0 , #0x20  
;------------------------Cortex_M0 Code End-----------------------;	
	ELSE
    IF ((Kernel == Cortex_M3)||(Kernel == Cortex_M3_FPU))	
;------------------------Cortex_M3 Code Start---------------------;		
	CBZ     R0, OSFirstEnter 
	
	IF (Kernel == Cortex_M3_FPU)
   ;FPU support code	  
    SUBS    R0, R0, #0X40 
    VSTM    R0, {S16-S31}	
   ;FPU support code
    ENDIF
	
    SUB     R0, R0, #0x20  	
    STM     R0, {R4-R11}  
;------------------------Cortex_M3 Code End-----------------------;		
	ELSE
    IF ((Kernel == Cortex_M4)||(Kernel == Cortex_M4_FPU))	
;------------------------Cortex_M4 Code Start---------------------;
	CBZ     R0, OSFirstEnter 					   

	IF (Kernel == Cortex_M4_FPU)
    ;FPU support code	  
    SUBS    R0, R0, #0X40 
    VSTM    R0, {S16-S31}	
    ;FPU support code
    ENDIF

    SUB     R0, R0, #0x20          
    STM     R0, {R4-R11} 
;------------------------Cortex_M4 Code End-----------------------;	
	ENDIF
	ENDIF
	ENDIF
	
    LDR     R1, =OSTCBRun          
    LDR     R1, [R1]               
    STR     R0, [R1]               
OSFirstEnter    
    LDR     R0, =OSTCBRun          
    LDR     R1, =OSTCBCur    

    IF ((Kernel == Cortex_M0)||(Kernel == Cortex_M0_FPU))	
;------------------------Cortex_M0 Code	Start---------------------;
    LDR     R2 , [R1]               
    STR     R2 , [R0]               	                               
    LDR     R0 , [R2]   
    LDR     R4 , [R0]              
    ADDS    R0 , R0 , #0x4         
    LDR     R5 , [R0]              
    ADDS    R0 , R0 , #0x4         
    LDR     R6 , [R0]              
    ADDS    R0 , R0 , #0x4         
    LDR     R7 , [R0]             
    ADDS    R0 , R0 , #0x4 
    ;M0 support code 
	PUSH    {R4 - R7}             
    LDR     R4 , [R0]             
    ADDS    R0 , R0 , #0x4         
	MOV     R8 , R4
    LDR     R5 , [R0]             
    ADDS    R0 , R0 , #0x4         
	MOV     R9 , R5
    LDR     R6 , [R0]             
    ADDS    R0 , R0 , #0x4         
	MOV     R10, R6 
    LDR     R7 , [R0]             
    ADDS    R0 , R0 , #0x4         
	MOV     R11, R7
	POP    {R4 - R7}  
    ;M0 support code 
	
	IF (Kernel == Cortex_M0_FPU)
    ;FPU support code
    VLDM    R0, {S16-S31} 
    ADDS    R0, R0, #0X40
    ;FPU support code
	ENDIF
		
    MSR     PSP, R0    
    MOVS    R1 , #0x04
	MOV     R2 , LR
    ORRS    R2 , R2 , R1  
	MOV     LR , R2
;------------------------Cortex_M0 Code End-----------------------;	
    ELSE
    IF ((Kernel == Cortex_M3)||(Kernel == Cortex_M3_FPU))	
;------------------------Cortex_M3 Code Start---------------------;
    LDR     R2, [R1]               
    STR     R2, [R0]               	                               
    LDR     R0, [R2]               
	LDM     R0, {R4-R11}           
	ADD     R0, R0, #0x20  

	IF (Kernel == Cortex_M3_FPU)
    ;FPU support code
    VLDM    R0, {S16-S31} 
    ADDS    R0, R0, #0X40
    ;FPU support code
	ENDIF

    MSR     PSP, R0                
    ORR     LR, LR, #0x04   
;------------------------Cortex_M3 Code End-----------------------;	
    ELSE
    IF ((Kernel == Cortex_M4)||(Kernel == Cortex_M4_FPU))	
;------------------------Cortex_M4 Code Start---------------------;
	LDR     R2, [R1]                
    STR     R2, [R0]                                               
    LDR     R0, [R2]               
	LDM     R0, {R4-R11}           
	ADD     R0, R0, #0x20 
	
	IF (Kernel == Cortex_M4_FPU)
    ;FPU support code
    VLDM    R0, {S16-S31} 
    ADDS    R0, R0, #0X40
    ;FPU support code
	ENDIF
   
    MSR     PSP, R0                 		
    ORR     LR, LR, #0x04   					   
;------------------------Cortex_M4 Code End-----------------------;	
    ENDIF	
    ENDIF
    ENDIF		
						   
    CPSIE     I                    
    BX    LR                       
    align 4   
    end     ;by Angle_145    
		