list p=18f8722
#include <p18f8722.inc>
CONFIG OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

C1 udata 0x20
C1
 
C2 udata 0x21
C2
 
C3 udata 0x22
C3
 
tester udata 0x23
tester

ORG 0x00
goto state1
   
state1

clrf PORTA
clrf PORTB
clrf PORTC
clrf PORTD    
clrf PORTE
 
movlw 0FH
movwf ADCON1

movlw b'11110000'
movwf TRISA
bsf PORTA,0
bsf PORTA,1
bsf PORTA,2
bsf PORTA,3
    
movlw b'11110000'
movwf TRISB
bsf PORTB,0
bsf PORTB,1
bsf PORTB,2
bsf PORTB,3   
    
movlw b'11110000'
movwf TRISC
bsf PORTC,0
bsf PORTC,1
bsf PORTC,2
bsf PORTC,3
    
movlw b'11110000'
movwf TRISD
bsf PORTD,0
bsf PORTD,1
bsf PORTD,2
bsf PORTD,3  

movlw b'11111111'
movwf TRISE
 
call delay1

clrf PORTA
clrf PORTB
clrf PORTC
clrf PORTD    
clrf PORTE 

movlw b'11110000'
movwf TRISA
bcf PORTA,0
bcf PORTA,1
bcf PORTA,2
bcf PORTA,3
    
movlw b'11110000'
movwf TRISB
bcf PORTB,0
bcf PORTB,1
bcf PORTB,2
bcf PORTB,3   
    
movlw b'11110000'
movwf TRISC
bcf PORTC,0
bcf PORTC,1
bcf PORTC,2
bcf PORTC,3
    
movlw b'11110000'
movwf TRISD
bcf PORTD,0
bcf PORTD,1
bcf PORTD,2
bcf PORTD,3

call delay2

goto state2
 
state2

clrf PORTA
clrf PORTB
clrf PORTC
clrf PORTD    
clrf PORTE
 
bsf PORTA,0
call delay3
bsf PORTA,1
call delay3
bsf PORTA,2
call delay3
bsf PORTA,3
call delay3

bsf PORTB,0
call delay3
bsf PORTB,1
call delay3
bsf PORTB,2
call delay3
bsf PORTB,3
call delay3
 
bsf PORTC,0
call delay3
bsf PORTC,1
call delay3
bsf PORTC,2
call delay3
bsf PORTC,3
call delay3

bsf PORTD,0
call delay3
bsf PORTD,1
call delay3
bsf PORTD,2
call delay3
bsf PORTD,3
call delay3

goto lastcontroller
 
;RE4 Controller Conditions-------------------
re4cond0
bcf PORTD,3
call delay3
goto re4cond1
 
re4cond1
bcf PORTD,2
call delay3
goto re4cond2 

re4cond2 
bcf PORTD,1
call delay3
goto re4cond3 

re4cond3
bcf PORTD,0
call delay3
goto re4cond4 

re4cond4  
bcf PORTC,3
call delay3
goto re4cond5 

re4cond5  
bcf PORTC,2
call delay3
goto re4cond6 

re4cond6 
bcf PORTC,1
call delay3
goto re4cond7 

re4cond7 
bcf PORTC,0
call delay3
goto re4cond8 

re4cond8 
bcf PORTB,3
call delay3
goto re4cond9 

re4cond9 
bcf PORTB,2
call delay3
goto re4cond10 

re4cond10  
bcf PORTB,1
call delay3
goto re4cond11 

re4cond11 
bcf PORTB,0
call delay3 
goto re4cond12 

re4cond12  
bcf PORTA,3
call delay3
goto re4cond13 

re4cond13  
bcf PORTA,2
call delay3
goto re4cond14 

re4cond14 
bcf PORTA,1
call delay3
goto re4cond15 

re4cond15 
bcf PORTA,0
call delay3
goto lastcontroller 

;RE3 Controller Conditions--------------------------------------
re3cond0
bsf PORTA,0
call delay3
goto re3cond1
 
re3cond1
bsf PORTA,1
call delay3
goto re3cond2 

re3cond2 
bsf PORTA,2
call delay3
goto re3cond3 

re3cond3
bsf PORTA,3
call delay3
goto re3cond4 

re3cond4  
bsf PORTB,0
call delay3
goto re3cond5 

re3cond5  
bsf PORTB,1
call delay3
goto re3cond6 

re3cond6 
bsf PORTB,2
call delay3
goto re3cond7 

re3cond7 
bsf PORTB,3
call delay3
goto re3cond8 

re3cond8 
bsf PORTC,0
call delay3
goto re3cond9 

re3cond9 
bsf PORTC,1
call delay3
goto re3cond10 

re3cond10  
bsf PORTC,2
call delay3
goto re3cond11 

re3cond11 
bsf PORTC,3
call delay3 
goto re3cond12 

re3cond12  
bsf PORTD,0
call delay3
goto re3cond13 

re3cond13  
bsf PORTD,1
call delay3
goto re3cond14 

re3cond14 
bsf PORTD,2
call delay3
goto re3cond15 

re3cond15 
bsf PORTD,3
call delay3 
goto lastcontroller
 
; DELAY1----------------------- 
delay1

movlw 0xCA
movwf C3
    
l3	
movlw 0xBD
movwf C2

l2
movlw 0xAD
movwf C1
    
l1
decfsz C1,F
goto l1
decfsz C2,F
goto l2
decfsz C3,F
goto l3
return

; DELAY2------------------------ 
delay2
 
movlw 0x92
movwf C3   
ll3	
movlw 85
movwf C2
ll2
movlw 0xAA
movwf C1    
ll1
decfsz C1,F
goto ll1
decfsz C2,F
goto ll2
decfsz C3,F
goto ll3
return

;DELAY3-------------------------------------------- 
delay3 

movlw 0x37
movwf C3
lll3	
movlw 0x40
movwf C2
lll2
movlw 0x80
btfsc PORTA,4
movlw 0x65
movwf C1
lll1

call controller
decfsz C1,F
goto lll1
decfsz C2,F
goto lll2

decfsz C3,F
goto lll3
return    

;Stopping Button Control Started------------------------  
 
controller
btfss PORTA,4
goto ra4rcontroller
goto ra4pcontroller

;RA4 Button Control---------------------------- 
ra4rcontroller
btfss tester,0
return
goto ra4prcontroller
 
ra4pcontroller
bsf tester,0
btfsc PORTA,4
return
goto ra4prcontroller
 
ra4prcontroller
bcf tester,0
btfsc PORTA,4
return
goto recontroller

;Up-Down Button Control Started-----------------------  
recontroller
btfss PORTE,3
goto re4controller
goto re3controller
;RE3-Up Button Started--------------------------- 
re3controller
btfsc PORTE,3
goto re3controller
goto re3conditionA0

;RE4-Down Button Started----------------------- 
re4controller
btfss PORTE,4
goto recontroller
goto re4rcontroller 

re4rcontroller
btfsc PORTE,4
goto re4rcontroller
goto re4conditionA0

;RE4 Started-------------------- 
re4conditionA0
btfss PORTA,0
goto lastcontroller
goto re4conditionA1

re4conditionA1
btfss PORTA,1
goto re4cond15
goto re4conditionA2
 
re4conditionA2
btfss PORTA,2
goto re4cond14
goto re4conditionA3

re4conditionA3
btfss PORTA,3
goto re4cond13
goto re4conditionB0

re4conditionB0
btfss PORTB,0
goto re4cond12
goto re4conditionB1

re4conditionB1
btfss PORTB,1
goto re4cond11
goto re4conditionB2
 
re4conditionB2
btfss PORTB,2
goto re4cond10
goto re4conditionB3

re4conditionB3
btfss PORTB,3
goto re4cond9
goto re4conditionC0

re4conditionC0
btfss PORTC,0
goto re4cond8
goto re4conditionC1

re4conditionC1
btfss PORTC,1
goto re4cond7
goto re4conditionC2
 
re4conditionC2
btfss PORTC,2
goto re4cond6
goto re4conditionC3

re4conditionC3
btfss PORTC,3
goto re4cond5
goto re4conditionD0 

re4conditionD0
btfss PORTD,0
goto re4cond4
goto re4conditionD1

re4conditionD1
btfss PORTD,1
goto re4cond3
goto re4conditionD2
 
re4conditionD2
btfss PORTD,2
goto re4cond2
goto re4conditionD3

re4conditionD3
btfss PORTD,3
goto re4cond1
goto re4cond0  

;RE3 Started---------------------------
re3conditionA0
btfss PORTA,0
goto re3cond0
goto re3conditionA1

re3conditionA1
btfss PORTA,1
goto re3cond1
goto re3conditionA2
 
re3conditionA2
btfss PORTA,2
goto re3cond2
goto re3conditionA3

re3conditionA3
btfss PORTA,3
goto re3cond3
goto re3conditionB0

re3conditionB0
btfss PORTB,0
goto re3cond4
goto re3conditionB1

re3conditionB1
btfss PORTB,1
goto re3cond5
goto re3conditionB2
 
re3conditionB2
btfss PORTB,2
goto re3cond6
goto re3conditionB3

re3conditionB3
btfss PORTB,3
goto re3cond7
goto re3conditionC0

re3conditionC0
btfss PORTC,0
goto re3cond8
goto re3conditionC1

re3conditionC1
btfss PORTC,1
goto re3cond9
goto re3conditionC2
 
re3conditionC2
btfss PORTC,2
goto re3cond10
goto re3conditionC3

re3conditionC3
btfss PORTC,3
goto re3cond11
goto re3conditionD0 

re3conditionD0
btfss PORTD,0
goto re3cond12
goto re3conditionD1

re3conditionD1
btfss PORTD,1
goto re3cond13
goto re3conditionD2
 
re3conditionD2
btfss PORTD,2
goto re3cond14
goto re3conditionD3

re3conditionD3
btfss PORTD,3
goto re3cond15
goto lastcontroller  

;Last-First Element Wait Controller 
lastcontroller
btfss PORTA,4
goto lastcontroller 
goto lastcontroller2

lastcontroller2
btfsc PORTA,4
goto lastcontroller2
goto lastcontroller3 

lastcontroller3
btfss PORTD,3
goto re3lastcontroller
goto re4lastcontroller
 
re3lastcontroller
btfss PORTE,3
goto re3lastcontroller
goto re3rlastcontroller

re3rlastcontroller
btfsc PORTE,3
goto re3rlastcontroller 
goto re3conditionA0
 
re4lastcontroller
btfss PORTE,4
goto re4lastcontroller
goto re4rlastcontroller

re4rlastcontroller
btfsc PORTE,4
goto re4rlastcontroller 
goto re4conditionA0
 
END
