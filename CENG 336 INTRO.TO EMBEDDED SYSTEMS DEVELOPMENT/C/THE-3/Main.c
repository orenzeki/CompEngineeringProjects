//2264612 Zeki Oren
//2264604 Tunahan Ocal

#include <p18cxxx.h>
#include "Includes.h"
#include "LCD.h"

#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#define _XTAL_FREQ   40000000

unsigned char c1;
unsigned int a =0 , b = 0;
int portbpress = 0;
int rb7count = 0;
int portb6press= 0;
int count=50;
int blinkflag=0;
int timeleft;
int d1=9;
int d2=0;
int count2=20;
int decflag;
int votedigit1=0;
int votedigit2=0;
int iddigit1=0;
int iddigit2=0;
unsigned int advalue;
int count3=20;
char* candname;
char* candid;
int voteflag=0;
int votercount=0;
int votesarray[8]={0,0,0,0,0,0,0,0};
int segd0;
int segd1;
int segd2;
int segd3;
int sevenflag=0;
int array[11]={0x40,0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67};
int maximum;
int location;

void init();
void __interrupt() isr();
void adchandle();
void readadc();
void vote();
void choosecand();
void addarray();
void sevenseg();
void findmaximumvote();
void secondscreen();
void recontroller();
void addvoters();
// Main Function
void main(void){    
    InitLCD();			                    // Initialize LCD in 4bit mode
    init();                                 //Call the init() function
   
    ClearLCDScreen();                       // Clear LCD screen
    
    WriteCommandToLCD(0x80);                // Goto to the beginning of the first line
    WriteStringToLCD(" #Electro Vote# ");	// Write Electro Vote# on LCD
    WriteCommandToLCD(0xC0);                // Goto to the beginning of the second line
    WriteStringToLCD(" ############## ");   // Write ############## on LCD
    recontroller();                         // Goto recontroller() function to control RE1 press and release
     
    for(int i=0; i<200; i++){               // Three seconds delay 
        for(int j=0; j<100; j++){
            for(int k=0; k<100; k++){
            }
        }
    }

    ClearLCDScreen();                        // Clear LCD screen
    addvoters();
                    
}
void addvoters(void){
    
    INTCONbits.RBIE=1;                        // Enable RB Port Change Interrupt Enable bit
    while(1){

        WriteCommandToLCD(0x80);              // Goto to the beginning of the first line
        WriteStringToLCD(" #Enter Voters# "); // Write #Enter Voters# on LCD
        WriteCommandToLCD(0xC1);              // Goto to the second column of the second line
        c1 = (char)(((int)'0') + a);            
        WriteDataToLCD(c1);                   // Write tens digit of the voters
        c1 = (char)(((int)'0') + b);
        WriteDataToLCD(c1);                   // Write units digit of the voters
   
        if(rb7count==2){                      
            
            adchandle();                      // Goto adchandle() function after pressed rb7 button 2 times 
        } 
    }
}

void recontroller(void){
    int repres = 0;
    while(!repres) {
        
        if(PORTEbits.RE1) {             // If RE1 is pressed, wait for the release in the next loop
            repres = 1;
        }
    }
    while(repres) {
        
        if(!PORTEbits.RE1) {            // If RE1 is pressed, exit from the loop
            repres = 0;
            
        }
    }
    
}
void readadc(void){
    
    sevenseg();                          // Goto sevenseg() function to use seven segment 
    
    if(advalue<128){                     // 0-127 = Poe
        
        candname="Poe   ";
        candid="18"; 
    }
    else if(advalue<256){                // 128-255 = Galib
      
        candname="Galib ";
        candid="64"; 
    }
    else if(advalue<384){                // 256-383 = Selimi
       
        candname="Selimi";
        candid="34"; 
    }
    else if(advalue<512){                // 384-511 = Nesimi
        
        candname="Nesimi";
        candid="23";  
    }
    else if(advalue<640){                // 512-639 = Hatayi
       
        candname="Hatayi";
        candid="33";  
    }
    else if(advalue<768){                // 640-767 = Zweig
        
        candname="Zweig ";
        candid="67";   
    }
    else if(advalue<896){                // 768-895 = Nabi
        
        candname="Nabi  ";
        candid="63";      
    }
    else if(advalue<1024){               // 896-1023 = Austen
        
        candname="Austen";
        candid="99";  
    }
}
void adchandle(void){
    
    sevenseg();                           // Goto sevenseg() function to use seven segment
    
    INTCONbits.PEIE_GIEL = 1;             // Enable Peripheral Interrupt Enable bit
    T0CONbits.TMR0ON = 1;                 // Enable TMR0
    T1CONbits.TMR1ON = 1;                 // Enable TMR1
    INTCONbits.RBIE = 1;                  // Enable RB Port Change Interrupt Enable bit    
    
    votercount = a*10+b;                  // Calculate the number of voters
    segd0 = 0;
    segd1 = 0;
    segd2 = 0;
    segd3 = 0;
    
    while(1){
        
        sevenseg();                        // Goto sevenseg() function to use seven segment
        
        readadc();                         // Goto readadc() function to find which candidate is voting.  
        
        if(sevenflag){                     // When seven segment is activated
            
            findmaximumvote();             // Goto findmaximumvote() function to find the candidate who have maximum vote.
            
            if(a==0 && b==0){              // If number of voters is zero, seven segment will show ----
                
                segd0 = 0;
                segd1 = 0;
                segd2 = 0;
                segd3 = 0;
            }
            else if(location == 0){         // Choose Poe (id = 18)
                
                segd3 = 2;                    
                segd2 = 9;
                segd1 = (maximum/10)+1;
                segd0 = (maximum%10)+1;
            }
            else if(location == 1){          // Choose Galib (id = 64)
                
                segd3 = 7;
                segd2 = 5;
                segd1 = (maximum/10)+1;
                segd0 = (maximum%10)+1;
            }
            else if(location == 2){          // Choose Selimi (id = 34)
                
                segd3 = 4;
                segd2 = 5;
                segd1 = (maximum/10)+1;
                segd0 = (maximum%10)+1;
            }
            else if(location == 3){          // Choose Nesimi (id = 23)
                
                segd3 = 3;
                segd2 = 4;
                segd1 = (maximum/10)+1;
                segd0 = (maximum%10)+1;
            }    
            else if(location == 4){          // Choose Hatayi (id = 33)
                
                segd3 = 4;
                segd2 = 4;
                segd1 = (maximum/10)+1;
                segd0 = (maximum%10)+1;
            }   
            else if(location == 5){          // Choose Zweig (id = 67)
            
                segd3 = 7;
                segd2 = 8;
                segd1 = (maximum/10)+1;
                segd0 = (maximum%10)+1;
            }
            else if(location == 6){          // Choose Nabi (id = 63)
                
                segd3 = 7;
                segd2 = 4;
                segd1 = (maximum/10)+1;
                segd0 = (maximum%10)+1;
            }
            else {                           // Choose Austen (id = 99)
                
                segd3 = 10;
                segd2 = 10;
                segd1 = (maximum/10)+1;
                segd0 = (maximum%10)+1;
            }             
        }
      
        if(decflag){                         // TMR1 one second flag to establish 90 seconds vote time
          
            if(d2 == 0){                     // If units digit is 0 then decrement tens digit and set unit digits 9
           
                d2 = 9;
                d1--;
                decflag = 0; 
            }
            else if(d1 == 0){                // If tens digit is 0 then decrement units digit
             
                d2--;
                decflag = 0;
            }
            else{                            // Otherwise decrement units digit                          
             
                d2--;
                decflag = 0;
            }
        }
        if(d1 == 0 && d2 == 0){                        // After 90 seconds vote time finished
            
            while(1){
              
                readadc();                             // Goto readadc()function to find current candidate
                
                WriteCommandToLCD(0x80);               // Goto to the beginning of the first line
                WriteStringToLCD("  Time Left :00");   // Write Time Left :00 on LCD
                WriteCommandToLCD(0xC1);               // Goto to the second column of the second line
                WriteDataToLCD('>');                   // Write > on LCD
                WriteCommandToLCD(0xC2);               // Goto to the third column of the second line
                
                sevenseg();                            // Goto sevenseg() function to use seven segment
      
                WriteStringToLCD(candid);              // Write candidate id on LCD
                WriteCommandToLCD(0xC5);               // Goto to the sixth column of the second line
                WriteStringToLCD(candname);            // Write candidate name on LCD
                WriteCommandToLCD(0xCC);               // Goto to the thirteenth column of the second line
                WriteDataToLCD(':');                   // Write : on LCD
                WriteCommandToLCD(0xCD);               // Goto to the fourteenth column of the second line
            
                choosecand();
   
                c1 = (char)(((int)'0') + votedigit1);  
                WriteDataToLCD(c1);                    // Write tens digit of the votes of current candidate
                WriteCommandToLCD(0xCE);               // Goto to the fifteenth column of the second line
                c1 = (char)(((int)'0') + votedigit2);
                WriteDataToLCD(c1);                    // Write units digit of the votes of current candidate
            }
        }
        WriteCommandToLCD(0x80);                       // Goto to the beginning of the first line
        
        sevenseg();                                    // Goto sevenseg() function to use seven segment
    
        WriteStringToLCD("  Time Left :");	           // Write Time Left : on LCD
        WriteCommandToLCD(0x8D);                       // Goto to the fourteenth column of the first line
        c1 = (char)(((int)'0') + d1);
        WriteDataToLCD(c1);                            // Write tens digit of the rest vote time
        WriteCommandToLCD(0x8E);                       // Goto to the fifteenth column of the first line
        c1 = (char)(((int)'0') + d2);
        WriteDataToLCD(c1);                            // Write units digit of the rest vote time
        WriteCommandToLCD(0xC0);                       // Goto to the beginning of the second line
        c1 = ' ';
        WriteDataToLCD(c1);                            // Write a blank column on LCD
    
        sevenseg();                                    // Goto sevenseg() function to use seven segment
    
        if(blinkflag){                                 // After vote time activated, > will blink.
            
            WriteCommandToLCD(0xC1);                   // Goto to the second column of the second line 
            WriteDataToLCD('>');                       // Write > on LCD
        }
        else{
            
            WriteCommandToLCD(0xC1);                   // Goto to the second column of the second line
            WriteDataToLCD(' ');                       // Write a blank column on LCD to provide blink
        }  
        
        WriteCommandToLCD(0xC2);                       // Goto to the third column of the second line
        WriteStringToLCD(candid);                      // Write candidate id of the current candidate on LCD
        WriteCommandToLCD(0xC5);                       // Goto to the sixth column of the second line
        WriteStringToLCD(candname);                    // Write candidate name of the current candidate LCD
        WriteCommandToLCD(0xCC);                       // Goto to the thirteenth column of the second line
        WriteDataToLCD(':');                           // Write : on LCD
        WriteCommandToLCD(0xCD);                       // Goto to the fourteenth column of the second line
         
        choosecand();
       
        addarray();
        
        sevenseg();                                    // Goto sevenseg() function to use seven segment
        
        c1 = (char)(((int)'0') + votedigit1);
        WriteDataToLCD(c1);                            // Write tens digit of the votes of current candidate on LCD
        WriteCommandToLCD(0xCE);                       // Goto to the fifteenth column of the second line
        c1 = (char)(((int)'0') + votedigit2);
        WriteDataToLCD(c1);                            // Write units digit of the votes of current candidate on LCD
        
        sevenseg();                                    // Goto sevenseg() function to use seven segment
        
        choosecand();                                  // Goto choosecand() function to find current candidate
        
        vote();                                        // Goto vote() function to be able to vote current candidate
        
        addarray();                                    // Goto addarray() function to add votes of the current candidate to the array.
    }
}
void vote(void){
        
    sevenseg();                                        // Goto sevenseg() function to use seven segment                    
    
    if(voteflag){                                      // When pressed to the RB7 voterflag will be set.
         
        if(votercount==0){                             // If all votes are given some candidates, vote will be stopped
            
            voteflag=0;
            return;
        }        
        else  if(votedigit2==0){                       // Add the first vote
            
            votedigit2++;             
            voteflag=0;
            votercount--;  
        }
        else if(votedigit2==9){                        // To handle tens digits of votes
              
            votedigit2=0;
            votedigit1++;
            voteflag=0;
            votercount--;
        }
        else{                                          // Increment the vote.
            
            votedigit2++;
            voteflag=0;
            votercount--;
        }
    }
}
void choosecand(void){
    
    sevenseg();                                        // Goto sevenseg() function to use seven segment
    
    if(candid=="18"){                                  // If candidate id is 18, we get the votes digits of Poe to show in LCD 
        
        votedigit1=votesarray[0]/10;
        votedigit2=votesarray[0]%10;
    }
    else if(candid=="64"){                             // If candidate id is 64, we get the votes digits of Galib to show in LCD
        
        votedigit1=votesarray[1]/10;
        votedigit2=votesarray[1]%10;
    }
    else if(candid=="34"){                             // If candidate id is 34, we get the votes digits of Selimi to show in LCD
       
        votedigit1=votesarray[2]/10;
        votedigit2=votesarray[2]%10;
    }
    else if(candid=="23"){                             // If candidate id is 23, we get the votes digits of Nesimi to show in LCD
        
        votedigit1=votesarray[3]/10;
        votedigit2=votesarray[3]%10;
    }
    else if(candid=="33"){                             // If candidate id is 33, we get the votes digits of Hatayi to show in LCD
       
        votedigit1=votesarray[4]/10;
        votedigit2=votesarray[4]%10;
    }
    else if(candid=="67"){                             // If candidate id is 67, we get the votes digits of Zweig to show in LCD
      
        votedigit1=votesarray[5]/10;
        votedigit2=votesarray[5]%10;
    }
    else if(candid=="63"){                             // If candidate id is 63, we get the votes digits of Nabi to show in LCD
       
        votedigit1=votesarray[6]/10;
        votedigit2=votesarray[6]%10;
    }
    else if(candid=="99"){                             // If candidate id is 99, we get the votes digits of Austen to show in LCD
       
        votedigit1=votesarray[7]/10;
        votedigit2=votesarray[7]%10;
    }
}
void addarray(void){                                   // we keep the votes of all candidates in an array (votesarray)
    
    sevenseg();                                        // Goto sevenseg() function to use seven segment
   
    if(candid=="18"){                                  // Calculate the votes and keep it in the 0 index of array
        
        votesarray[0]=votedigit1*10+votedigit2;
    }
    else if(candid=="64"){                             // Calculate the votes and keep it in the 1st index of array
       
        votesarray[1]=votedigit1*10+votedigit2;
    }
    else if(candid=="34"){                             // Calculate the votes and keep it in the 2nd index of array
       
        votesarray[2]=votedigit1*10+votedigit2;
    }
    else if(candid=="23"){                             // Calculate the votes and keep it in the 3rd index of array
        
        votesarray[3]=votedigit1*10+votedigit2;
    }
    else if(candid=="33"){                             // Calculate the votes and keep it in the 4th index of array
        
        votesarray[4]=votedigit1*10+votedigit2;
    }
    else if(candid=="67"){                             // Calculate the votes and keep it in the 5th index of array
       
        votesarray[5]=votedigit1*10+votedigit2;
    }
    else if(candid=="63"){                             // Calculate the votes and keep it in the 6th index of array
        
        votesarray[6]=votedigit1*10+votedigit2;
    }
    else if(candid=="99"){                             // Calculate the votes and keep it in the 7th index of array
        
        votesarray[7]=votedigit1*10+votedigit2;
    }
}
void sevenseg(void){
    
    LATH=0;             // Clear LATH
    
    LATJ=array[segd0];  // Set seven segment left-most first digit
    LATH3=1;            // Light the digit
    __delay_ms(0.5);    // Wait 500 microsecond
    LATH3=0;            // Clear the digit
    
    LATJ=array[segd1];  // Set seven segment left-most first digit
    LATH2=1;            // Light the digit
    __delay_ms(0.5);    // Wait 500 microsecond
    LATH2=0;            // Clear the digit
    
    LATJ=array[segd2];  // Set seven segment left-most first digit
    LATH1=1;            // Light the digit
    __delay_ms(0.5);    // Wait 500 microsecond
    LATH1=0;            // Clear the digit
    
    LATJ=array[segd3];  // Set seven segment left-most first digit
    LATH0=1;            // Light the digit
    __delay_ms(0.5);    // Wait 500 microsecond
    LATH0=0;            // Clear the digit
    
    return;    
}
void findmaximumvote(void){           // We use this function to find the candidate which take votes more than the others.
    
    maximum = votesarray[0];
    int c=1;
    location = 0;
    for (c = 1; c < 8; c++){
        
        if (votesarray[c] > maximum){
            
            maximum  = votesarray[c];
            location = c;
        }
    }
    
    sevenseg();                         // Goto sevenseg() function to use seven segment
}
void init(void) {
    INTCON = 0;            // INTCON is cleared.
    TRISB = 0xC0;          // RB6,RB7 are set inputs.
    
    RCONbits.IPEN = 0;     // Priorities are disabled.
    PORTE = 0;             // Clear PORTE
    LATE = 0;              // Clear LATE
    TRISE = 0xFF;          // Configure RE1 as input
    
    INTCON2bits.RBPU = 0;  // Pull-ups are enabled.
    
    TMR0L = 61;            // TMR0 is loaded with 61. (256-61=195)
    T0CON = 0b01000111;    // TMR0 is configured --16 bit -- 1:8 prescaler.
    INTCON = 0b10100000;   // GIE and TIMER0 are enabled
   
    PIE1bits.TMR1IE = 1;   // TMR1 is enabled. 
    PIR1bits.TMR1IF = 0;   // TMR1 Interrupt Flag is enabled.
    T1CON = 0b10110000;    // TMR1 is Configured --16 bit -- 1:8 prescaler.
    TMR1 = 3036;           // TMR1 is loaded with 3036. (65536-3036=62500)
    
    TRISH4 = 1;            // Make input to 4th bit of TRISH
    ADCON0 = 0x30;         
    ADCON1 = 0x00;         
    ADCON2 = 0x8A;         
    ADCON0bits.ADON = 1;   // AD Converter enabled.
    
    PIR1bits.ADIF = 0;     // AD converter flag bit is set to 0
    PIE1bits.ADIE = 1;     // AD Converter Interrupt enabled.
    
    TRISB4 = 0;            // Clear TRISB4 to ignore its interrupts
    TRISB5 = 0;            // Clear TRISB5 to ignore its interrupts
    TRISB6 = 1;            // Set TRISB6 to be able to use RB6 interrupt
    TRISB7 = 1;            // Set TRISB7 to be able to use RB7 interrupt
    
    PORTB = 0;             // PORTB is cleared.
    LATB = 0;              // LATB is cleared.
    
    TRISJ = 0;             // TRISJ is cleared
    LATJ = 0;              // LATJ is cleared
    LATH = 0;              // LATH is cleared
}
void __interrupt() isr(void){
    
    if(INTCONbits.T0IF==1){       // TMR0 Interrupt 
         
        sevenseg();               // Goto sevenseg() function to use seven segment
        
        TMR0L=61;                 // 256-61=195 times 
        INTCONbits.T0IF=0;        // TMR0 interrupt flag bit is cleared
        count--;
        count3--;
       
        if(count==0){             // Blink 
            
            count=40;
            blinkflag=!blinkflag;
        }
        if(count3==0){            // A/D interrupt 
            
            count3=20;
            PIR1bits.ADIF=0;      // A/D interrupt flag is cleared
            ADCON0bits.GODONE=1;  // A/D Conversion Status bit is enabled
            PIE1bits.ADIE=1;      // A/D Interrupt Enabled
        }
    }
    if(PIR1bits.TMR1IF){          // TMR1 Interrupt
         
        TMR1=3036;                // 65536-3036=62500 times
        PIR1bits.TMR1IF=0;        // TMR1 interrupt flag bit is cleared
        count2--;
        
        if(!count2){              // One second timer to decrement vote time 
            
            count2=20;
            decflag=1;
        }
    }
    if(ADIF){                         // A/D Converter Interrupt
        
        advalue=((ADRESH<<8)+ADRESL);
        ADIF=0;                       // AD interrupt flag bit is cleared
    }
    
   
    if (INTCONbits.RBIF==1){          // PORTB Interrupt

        if (PORTBbits.RB7==1){        // RB7 is pressed
            
            portb6press=2;
        } 
        else if(PORTBbits.RB6==1){    // RB6 is pressed
           
            portb6press=1;
        }     

        if(portb6press==1){           // If RB6 is pressed
           
            if(PORTBbits.RB6==0){     // Flag of RB6 is set
                
                portbpress=1;
                portb6press=0;
            }
         
        }
        else if(portb6press==2){       // If RB7 is pressed 
           
            if(PORTBbits.RB7==0){      // Flag of RB7 is set
                
                portbpress=2;
                portb6press=0;
            }
        
        }
        if(portbpress==2){             // If RB7 is pressed
        
            if(rb7count<2){            // If RB7 is pressed less then 2 times
                
                INTCONbits.RBIF=0;     // RB interrupt flag is cleared
                portb6press=0;
                portbpress=0;
                rb7count++;
                return;       
            }
            else {                     // If RB7 is pressed 2 or more times
                
                INTCONbits.RBIF=0;     // RB interrupt flag is cleared  
                portb6press=0;
                portbpress=0;
                voteflag=1;            // Votes can be used
                sevenflag=1;           // Seven segment will be started
                return;
            }
        }
        else if(portbpress==1){        // If RB6 is pressed

            if(rb7count==1){           // If RB7 is pressed 1 times before, set the units digit of the voters number
            
                if(b==9){
                    
                    portb6press=0;
                    INTCONbits.RBIF=0; // RB interrupt flag is cleared
                    portbpress=0;
                    b=0;
                    return;
                }
                else{
                    
                    portb6press=0;
                    INTCONbits.RBIF=0; // RB interrupt flag is cleared 
                    portbpress=0;
                    b++;
                    return;
                }    
            }
            if(rb7count==0){           // If RB7 is not pressed before, set the tens digit of the voters number
                
                if(a==9){
                    
                    portbpress=0;
                    INTCONbits.RBIF=0; // RB interrupt flag is cleared 
                    a=0;
                    portb6press=0;
                    return;
                }
                else{
                    portb6press=0;
                    portbpress=0;
                    INTCONbits.RBIF=0; // RB interrupt flag is cleared 
                    a++;
                    return;
                }    
            }
        }
 
    INTCONbits.RBIF=0;                 // RB interrupt flag is cleared
    }
}
