;2264064 Tunahan ÖCAL
;2264612 Zeki ÖREN
    
list P=18F8722

#include <p18f8722.inc>
config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

state   udata 0x21
state

counter   udata 0x22
counter

w_temp  udata 0x23
w_temp

status_temp udata 0x24
status_temp

pclath_temp udata 0x25
pclath_temp

asteroid_counter udata 0x26
asteroid_counter
 
laserbitsb udata 0x27
laserbitsb

laserbitsc udata 0x28
laserbitsc

laserbitsd udata 0x29
laserbitsd

laserbitse udata 0x30
laserbitse

laserbitsf udata 0x31
laserbitsf

templaserbits udata 0x32
templaserbits

templaserbits2 udata 0x33
templaserbits2

movelaserflag udata 0x34
movelaserflag


spaceship udata 0x36
spaceship
 
count udata 0x38
count
 
asteroidthreebits udata 0x40
asteroidthreebits

asteroidbits2 udata 0x41
asteroidbits2
 
asteroidbits3 udata 0x42
asteroidbits3
 
asteroidbitsa udata 0x43
asteroidbitsa 
 
asteroidbitsb udata 0x44
asteroidbitsb

asteroidbitsc udata 0x45
asteroidbitsc

asteroidbitsd udata 0x46
asteroidbitsd

asteroidbitse udata 0x47
asteroidbitse

asteroidbitsf udata 0x48
asteroidbitsf
 
crashcount udata 0x49
crashcount
 
crashnumber udata 0x50
crashnumber
 
firstlaserbits udata 0x51
firstlaserbits

digit1 udata 0x52
digit1

digit2 udata 0x53
digit2
 
temp1 udata 0x54
temp1
 
temp2 udata 0x55
temp2
 
lbit udata 0x56
lbit

hbit udata 0x57
hbit
 
t1 udata 0x58
t1
 
t2 udata 0x59
t2
 
count2 udata 0x60
count2 
 
org 0x00
goto init
org 0x08
goto control

init:
    ;Disable interrupts
    clrf INTCON
    clrf INTCON2   

    clrf PORTA
    clrf PORTB
    clrf PORTC
    clrf PORTD
    clrf PORTE
    clrf PORTF
    clrf PORTG
    clrf PORTH
    clrf PORTJ
    
    movlw 0FH
    movwf ADCON1
    
    movlw b'11000000'
    movwf TRISA
    movwf TRISB
    movwf TRISC
    movwf TRISD
    movwf TRISE
    movwf TRISF
    clrf TRISH
    clrf TRISJ
    clrf LATH
    clrf LATJ
    
    movlw b'11111111'
    movwf TRISG
    
    clrf laserbitsb
    clrf laserbitsc
    clrf laserbitsd
    clrf laserbitse
    clrf laserbitsf
    clrf templaserbits
    clrf templaserbits2
    clrf movelaserflag
    clrf count
    clrf count2

    movlw b'00000000'
    movwf laserbitsb
    movwf laserbitsc
    movwf laserbitsd
    movwf laserbitse
    movwf laserbitsf
    movwf asteroidbitsa
    movwf asteroidbitsb
    movwf asteroidbitsc
    movwf asteroidbitsd
    movwf asteroidbitse
    movwf asteroidbitsf
    movwf templaserbits
    movwf templaserbits2
    movwf movelaserflag
    movwf spaceship
    movwf count
    movwf count2
    movwf asteroidthreebits
    movwf asteroidbits2
    movwf asteroidbits3
    movwf crashnumber
    movwf crashcount
    movwf digit1
    movwf digit2
    movwf t1
    movwf t2
    
    call sevenseg
    
    bcf     INTCON2, 7  ;Pull-ups are enabled - clear INTCON2<7>
    
    movlw   b'01000111' ;Disable Timer0 by setting TMR0ON to 0 (for now)
                        ;Configure Timer0 as an 8-bit timer/counter by setting T08BIT to 1
                        ;Timer0 increment from internal clock with a prescaler of 1:256.
    movwf   T0CON ; T0CON = b'01000111'
    
    clrf    PIE1
    
    bsf PIE1,0
    
    movlw   b'11000000'
    movwf   T1CON
    
initialstate: 
    call sevensegment
    bsf PORTA,3
    bsf spaceship,3
    btfss PORTG,0
    goto initialstate
    goto rg0control
rg0control:
    btfsc PORTG,0
    goto rg0control
    goto main
    
    
main:
    call sevensegment
    bsf     T1CON,0
    movff   TMR1L,lbit
    movff   TMR1H,hbit
    movlw   b'11101000' ;Enable Global, peripheral, Timer0 and RB interrupts by setting GIE, PEIE, TMR0IE and RBIE bits to 1
    movwf   INTCON
    bsf     T0CON, 7    ;Enable Timer0 by setting TMR0ON to 1
   
    
game: 
    btfss movelaserflag,0
    goto game2
    goto checkcrash
    
game2:    
    call sevensegment
    btfss movelaserflag,0
    goto control
    goto createasteroid

createasteroid
    clrf movelaserflag
    btfss lbit,0
    goto cleara
    goto seta
    
cleara    
    bcf asteroidthreebits,0
    goto controlas1
seta    
    bsf asteroidthreebits,0
    goto controlas1
    
 controlas1   
    btfss lbit,1
    goto clearb
    goto setb
clearb
    bcf asteroidthreebits,1
    goto controlas2
setb
    bsf asteroidthreebits,1
    goto controlas2
controlas2    
    btfss lbit,2
    goto clearc
    goto setcc
clearc    
    bcf asteroidthreebits,2
    goto cont
setcc
    bsf asteroidthreebits,2
    goto cont 
cont    
    movlw b'00000111'
    cpfseq asteroidthreebits
    goto a6 
    bsf asteroidbitsf,1
    bsf PORTF,1
    incf count
    incf count2
    goto shifting

a6
    movlw b'00000110'
    cpfseq asteroidthreebits
    goto a5
    bsf asteroidbitsf,0
    bsf PORTF,0
    incf count
    incf count2
    goto shifting

a5
    movlw b'00000101'
    cpfseq asteroidthreebits
    goto a4
    bsf asteroidbitsf,5
    bsf PORTF,5
    incf count
    incf count2
    goto shifting
    
a4
    movlw b'00000100'
    cpfseq asteroidthreebits
    goto a3
    bsf asteroidbitsf,4
    bsf PORTF,4
    incf count
    incf count2
    goto shifting
    
a3
    movlw b'00000011'
    cpfseq asteroidthreebits
    goto a2
    bsf asteroidbitsf,3
    bsf PORTF,3
    incf count
    incf count2
    goto shifting
    
a2
    movlw b'00000010'
    cpfseq asteroidthreebits
    goto a1
    bsf asteroidbitsf,2
    bsf PORTF,2
    incf count
    incf count2
    goto shifting
 
a1
    movlw b'00000001'
    cpfseq asteroidthreebits
    goto a0
    bsf asteroidbitsf,1
    bsf PORTF,1
    incf count
    incf count2
    goto shifting
   
a0
    movlw b'00000000'
    cpfseq asteroidthreebits
    goto game
    bsf asteroidbitsf,0
    bsf PORTF,0
    incf count
    incf count2
    goto shifting
shifting:
    movlw d'10'
    cpfseq count2
    goto last
    clrf count2
    comf hbit,1
    comf lbit,1
    
last    
    btfss lbit,0
    goto clearmybit
    goto setmybit
setmybit
    bsf t1,0
    goto hbitcont
clearmybit
    bcf t1,0
    goto hbitcont 
    
hbitcont
    btfss hbit,0
    goto clearmybith
    goto setmybith
setmybith
    bsf t2,0
    goto contin
clearmybith
    bcf t2,0
    goto contin
    
contin    
    rrncf lbit,1
    rrncf hbit,1
    btfss t1,0
    goto clearbith
    goto setbith
    
setbith
    bsf hbit,7
    goto lowbit
clearbith
    bcf hbit,7
    goto lowbit
lowbit
    btfss t2,0
    goto clearbitl
    goto setbitl
setbitl
    bsf lbit,7
    goto game
clearbitl
    bcf lbit,7
    goto game
    
    
countcrash:
    btfss crashnumber,0
    goto ch1
    incf crashcount
    call sevensegment
    goto ch1
ch1
    btfss crashnumber,1
    goto ch2
    incf crashcount
    call sevensegment
    goto ch2
ch2
    btfss crashnumber,2
    goto ch3
    incf crashcount
    call sevensegment
    goto ch3
ch3
    btfss crashnumber,3
    goto ch4
    incf crashcount
    call sevensegment
    goto ch4
ch4
    btfss crashnumber,4
    goto ch5
    incf crashcount
    call sevensegment
    goto ch5
ch5
    btfss crashnumber,5
    return
    incf crashcount
    call sevensegment
    return

checkcrash
    
    movf laserbitsb,0
    movff laserbitsb,firstlaserbits
    xorwf asteroidbitsb,0
    andwf asteroidbitsb,1
    movff asteroidbitsb,asteroidbitsa
    andwf laserbitsb,1
    movf laserbitsb,0
    xorwf firstlaserbits,0
    movwf crashnumber
    call countcrash
    movf spaceship,0
    xorwf asteroidbitsa,0
    andwf spaceship,1
    movlw b'00000000'
    cpfseq spaceship
    goto continue
    goto init
 
continue
    movf spaceship,0
    iorwf asteroidbitsa,0
    movwf PORTA
    
    movf laserbitsb,0
    movff laserbitsb,firstlaserbits
    movff asteroidbitsc,asteroidbitsb
    xorwf asteroidbitsb,0
    andwf asteroidbitsb,1
    andwf laserbitsb,1
    movf laserbitsb,0
    xorwf firstlaserbits,0
    movwf crashnumber
    call countcrash
    movff laserbitsb, templaserbits
    clrf laserbitsb
    movf asteroidbitsb,0
    movwf PORTB
    
    movf laserbitsc,0
    movff laserbitsc,firstlaserbits
    xorwf asteroidbitsd,0
    andwf asteroidbitsd,1
    andwf laserbitsc,1
    movf laserbitsc,0
    xorwf firstlaserbits,0
    movwf crashnumber
    call countcrash
    
    movff laserbitsc, templaserbits2
    movff asteroidbitsd,asteroidbitsc
    movff templaserbits,laserbitsc
    movf laserbitsc,0
    movff laserbitsc,firstlaserbits
    xorwf asteroidbitsc,0
    andwf asteroidbitsc,1
    movwf PORTC
    andwf laserbitsc,1
    movf laserbitsc,0
    xorwf firstlaserbits,0
    movwf crashnumber
    call countcrash
    
    movf laserbitsd,0
    movff laserbitsd,firstlaserbits
    xorwf asteroidbitse,0
    andwf asteroidbitse,1
    andwf laserbitsd,1
    movf laserbitsd,0
    xorwf firstlaserbits,0
    movwf crashnumber
    call countcrash
    
    movff laserbitsd, templaserbits
    movff asteroidbitse,asteroidbitsd
    movff templaserbits2,laserbitsd
    movf laserbitsd,0
    movff laserbitsd,firstlaserbits
    xorwf asteroidbitsd,0
    andwf asteroidbitsd,1
    movwf PORTD
    andwf laserbitsd,1
    movff laserbitsd,templaserbits2
    movf laserbitsd,0
    xorwf firstlaserbits,0
    movwf crashnumber
    call countcrash

    movf laserbitse,0
    movff laserbitse,firstlaserbits
    xorwf asteroidbitsf,0
    andwf asteroidbitsf,1
    andwf laserbitse,1
    movf laserbitse,0
    xorwf firstlaserbits,0
    movwf crashnumber
    call countcrash
    
    movff laserbitse, templaserbits2
    movff asteroidbitsf,asteroidbitse
    movff templaserbits,laserbitse
    movf laserbitse,0
    movff laserbitse,firstlaserbits
    xorwf asteroidbitse,0
    andwf asteroidbitse,1
    movwf PORTE
    andwf laserbitse,1
    movff laserbitse,templaserbits
    movf laserbitse,0
    xorwf firstlaserbits,0
    movwf crashnumber
    call countcrash
    
    movf laserbitsf,0
    movff laserbitsf,firstlaserbits
    xorwf asteroidbitsf,0
    andwf asteroidbitsf,1
    andwf laserbitsf,1
    movf laserbitsf,0
    xorwf firstlaserbits,0
    movwf crashnumber
    call countcrash
    
    movff templaserbits2,laserbitsf
    clrf asteroidbitsf
    movf laserbitsf,0
    movwf PORTF   
    goto game2
    
moveupdown
    btfss PORTG,3
    goto movedown
    goto g3control

movedown
    btfss PORTG,2
    goto laser
    goto g2control

g2control
    btfsc PORTG,2
    goto g2control
    goto movedowncontrol
    
g3control
    btfsc PORTG,3
    goto g3control
    goto moveup

laser
    btfss PORTG,1
    goto game 
    goto g1control
g1control
    btfsc PORTG,1
    goto g1control
    goto shootlaser
shootlaser
    btfss spaceship,0
    goto checka1
    bsf PORTB,0
    bsf laserbitsb,0
    goto game
checka1
    btfss spaceship,1
    goto checka2
    bsf PORTB,1
    bsf laserbitsb,1
    goto game
checka2
    btfss spaceship,2
    goto checka3
    bsf PORTB,2
    bsf laserbitsb,2
    goto game
checka3
    btfss spaceship,3
    goto checka4
    bsf PORTB,3
    bsf laserbitsb,3
    goto game
checka4
    btfss spaceship,4
    goto checka5
    bsf PORTB,4
    bsf laserbitsb,4
    goto game
checka5
    btfss spaceship,5
    return
    bsf PORTB,5
    bsf laserbitsb,5
    goto game
    
moveup    
    btfss spaceship,5
    goto checku1
    bcf PORTA,5
    bcf spaceship,5
    bsf PORTA,4
    bsf spaceship,4
    goto game
checku1
    btfss spaceship,4
    goto checku2
    bcf PORTA,4
    bcf spaceship,4
    bsf PORTA,3
    bsf spaceship,3
    goto game
checku2
    btfss spaceship,3
    goto checku3
    bcf PORTA,3
    bcf spaceship,3
    bsf PORTA,2
    bsf spaceship,2
    goto game
checku3
    btfss spaceship,2
    goto checku4
    bcf PORTA,2
    bcf spaceship,2
    bsf PORTA,1
    bsf spaceship,1
    
    goto game
checku4
    btfss spaceship,1
    goto checku5
    bcf PORTA,1
    bcf spaceship,1
    bsf PORTA,0
    bsf spaceship,0
    goto game
checku5
    goto game

movedowncontrol           
    btfss spaceship,0
    goto checkd1
    bcf PORTA,0
    bcf spaceship,0
    bsf PORTA,1
    bsf spaceship,1
    goto game
checkd1
    btfss spaceship,1
    goto checkd2
    bcf PORTA,1
    bcf spaceship,1
    bsf PORTA,2
    bsf spaceship,2
    goto game
checkd2
    btfss spaceship,2
    goto checkd3
    bcf PORTA,2
    bcf spaceship,2
    bsf PORTA,3
    bsf spaceship,3
    goto game
checkd3
    btfss spaceship,3
    goto checkd4
    bcf PORTA,3
    bcf spaceship,3
    bsf PORTA,4
    bsf spaceship,4
    goto game
checkd4
    btfss spaceship,4
    goto checkd5
    bcf PORTA,4
    bcf spaceship,4
    bsf PORTA,5
    bsf spaceship,5
    goto game
checkd5
    goto game 

control:
    call save_registers
    btfss INTCON,2
    goto moveupdown
    goto timer0_interrupt
    
timer0_interrupt:
    
    movlw       b'00001010'
    cpfslt      count
    goto        timer400
    incf	counter, f              ;Timer interrupt handler part begins here by incrementing count variable
    movf	counter, w              ;Move count to Working register
    sublw	d'100'                    ;Decrement 100 from Working register
    btfss	STATUS, Z               ;Is the result Zero?
    goto	timer_interrupt_exit    ;No, then exit from interrupt service routine
    clrf	counter                 ;Yes, then clear count variable
    incf        movelaserflag 
    goto	timer_interrupt_exit

timer400:
    movlw       b'00011110'
    cpfslt      count
    goto        timer300
    incf	counter, f              ;Timer interrupt handler part begins here by incrementing count variable
    movf	counter, w              ;Move count to Working register
    sublw	d'80'                    ;Decrement 80 from Working register
    btfss	STATUS, Z               ;Is the result Zero?
    goto	timer_interrupt_exit    ;No, then exit from interrupt service routine
    clrf	counter                 ;Yes, then clear count variable
    incf        movelaserflag   
    goto	timer_interrupt_exit
    
timer300:
    movlw       b'00110010'
    cpfslt      count
    goto        timer200
    incf	counter, f              ;Timer interrupt handler part begins here by incrementing count variable
    movf	counter, w              ;Move count to Working register
    sublw	d'60'                    ;Decrement 60 from Working register
    btfss	STATUS, Z               ;Is the result Zero?
    goto	timer_interrupt_exit    ;No, then exit from interrupt service routine
    clrf	counter                 ;Yes, then clear count variable
    incf        movelaserflag   
    goto	timer_interrupt_exit
    
timer200:
    incf	counter, f              ;Timer interrupt handler part begins here by incrementing count variable
    movf	counter, w              ;Move count to Working register
    sublw	d'40'                    ;Decrement 40 from Working register
    btfss	STATUS, Z               ;Is the result Zero?
    goto	timer_interrupt_exit    ;No, then exit from interrupt service routine
    clrf	counter                 ;Yes, then clear count variable
    incf        movelaserflag   
    
timer_interrupt_exit:
        bcf     INTCON, 2           ;Clear TMROIF
	movlw	d'61'               ;256-61=195; 195*256*5 = 249600 instruction cycle;
	movwf	TMR0
	call	restore_registers   ;Restore STATUS and PCLATH registers to their state before interrupt occurs
	retfie

;;;;;;;;;;;; Register handling for proper operation of main program ;;;;;;;;;;;;
save_registers:
    movwf 	w_temp          ;Copy W to TEMP register
    swapf 	STATUS, w       ;Swap status to be saved into W
    clrf 	STATUS          ;bank 0, regardless of current bank, Clears IRP,RP1,RP0
    movwf 	status_temp     ;Save status to bank zero STATUS_TEMP register
    movf 	PCLATH, w       ;Only required if using pages 1, 2 and/or 3
    movwf 	pclath_temp     ;Save PCLATH into W
    clrf 	PCLATH          ;Page zero, regardless of current page
    return

restore_registers:
    movf 	pclath_temp, w  ;Restore PCLATH
    movwf 	PCLATH          ;Move W into PCLATH
    swapf 	status_temp, w  ;Swap STATUS_TEMP register into W
    movwf 	STATUS          ;Move W into STATUS register
    swapf 	w_temp, f       ;Swap W_TEMP
    swapf 	w_temp, w       ;Swap W_TEMP into W
    return

sevensegment:
    movlw d'10'
    cpfseq crashcount
    goto callfunc
    goto eqcond1
eqcond1    
    movlw d'0'
    movwf crashcount
    incf digit1
    movlw d'10'
    cpfseq digit1
    goto callfunc
    goto eqcond2

eqcond2
    movlw d'0'
    movwf digit1
    incf digit2
    goto callfunc
    
callfunc    
    call digits2
    call digits1
    call digits0
    return

;;;;;;;;;;;;;;;;;;;;;;; SEVEN_SEGMENT_DISPLAY ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;     
sevensegmentarray:
    lfsr FSR0,100h
    addwf FSR0L
    movf INDF0,0
    return    

digits0:
    movf crashcount,0  
    call sevensegmentarray
    movwf   LATJ
    bsf     LATH,3
    call    calldelay
    bcf     LATH,3
    return   
  
digits1:
    movf digit1,0  
    call sevensegmentarray
    movwf   LATJ
    bsf     LATH,2
    call    calldelay
    bcf     LATH,2
    return    
    
digits2:
    movf digit2, 0  
    call sevensegmentarray
    movwf   LATJ
    bsf     LATH,1
    call    calldelay
    bcf     LATH, 1
    return
    
calldelay:
    movlw d'1'
    movwf temp2

l2
    movlw d'10'
    movwf temp1

l1
    decfsz temp1,f
    goto l1
    decfsz temp2,f
        goto l2
    return    

sevenseg:
    lfsr FSR0, 100h
    movlw  b'00111111'
    movwf POSTINC0
    movlw  b'00000110'
    movwf POSTINC0
    movlw  b'01011011'
    movwf POSTINC0
    movlw  b'01001111'
    movwf POSTINC0
    movlw  b'01100110'
    movwf POSTINC0
    movlw  b'01101101'
    movwf POSTINC0
    movlw  b'01111101'
    movwf POSTINC0
    movlw  b'00000111'
    movwf POSTINC0
    movlw  b'01111111'
    movwf POSTINC0
    movlw  b'01100111'
    movwf INDF0		
    return    
    
 end 