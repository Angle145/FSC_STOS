;asm for M0 ( stm32f0 ) 

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
    MRS     R0 , PSP                
    CMP     R0 , #0x00
	BEQ     OSFirstEnter  
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
    LDR     R1 , =OSTCBRun          
    LDR     R1 , [R1]               
    STR     R0 , [R1]               
OSFirstEnter    
    LDR     R0 , =OSTCBRun          
    LDR     R1 , =OSTCBCur          
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
	
    MSR     PSP, R0    
    MOVS    R1 , #0x04
	MOV     R2 , LR
    ORRS    R2 , R2 , R1  
	MOV     LR , R2
    CPSIE   I                    
    BX      LR                       
    align   4   
    end     ;by Angle_145         
		