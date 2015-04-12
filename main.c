
/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <stdio.h>
#include <stdlib.h>
#include <pic18f4620.h>
#include <time.h>
#include <xc.h>
#define _XTAL_FREQ 32000000
#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
uint8_t lmotorstephigh; // timer preloader values
uint8_t lmotorsteplow;
uint8_t rmotorstephigh;
uint8_t rmotorsteplow;
unsigned int adc;       // ADC reading
int lstep;              // motor step count of left motor
int rstep;              // motor step count of right motor
int rstepcount;
int lstepcount;
int lturn;
int lturncount;
unsigned int ustep;
unsigned int rightturnstep;     //
unsigned int leftturnstep;
unsigned int rightstep;
unsigned int forward;
unsigned int right;
unsigned int left;
unsigned int uturn;
unsigned int has_left_wall; // Boolean variable for wall presence
unsigned int has_right_wall;
unsigned int has_front_wall;
unsigned int step_count;
unsigned int prev_adc;  

/* i.e. uint8_t <variable_name>; j*/
// make mouse take a certain number of steps
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void high_priority interrupt high_isr(void);


uint16_t adc_convert(uint8_t channel)
{   prev_adc = adc;                 // save current ADC reading
    ADCON0bits.CHS      = channel;  // reselect the given channel
    ADCON0bits.GO       = 0b1;      // start the conversion
    while(ADCON0bits.DONE);         // wait for the conversion to finish
    return (ADRESH<<8)|ADRESL;      // return the result
}
 

void delay(unsigned int ms)
{   for(int i = 0; i < ms; i++)
        __delay_ms(1);
}  

          

void main(void)
{   //int delay_time = 1000;
    //enable 8MHz internal clock
    OSCCON = 0b01111100;
    //enable internal PLL clock = 32MHz
    OSCTUNE = 0b01001111;
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    //InitApp();

    /* TODO <INSERT USER APPLICATION CODE HERE> */
    TRISA = 0b00000001;
    TRISC = 0b00000000;
    TRISB = 0b00000000;
    TRISD = 0b00000000;



    // set initial conditions for stepping motors
    LATC = 0b00000000;  // set initial values of all output ports
    
    lstep = 1;          // set step increment for motors
    rstep = 1;
    leftturnstep = 1;
    ustep = 1;
    forward = 0;        // set initial state to forward
    lturn = 1;
    rstepcount = 0;
    lstepcount = 0;
    lturncount = 0;


    ADCON0bits.CHS = 0b0000;    // select output channel
    ADCON0bits.GO = 0;          // A/D conversion initially idle
    ADCON1bits.VCFG = 0b00;     // configure voltage references
    ADCON1bits.PCFG = 0b1100;   // configure channel 0 for analog input
    ADCON2bits.ADFM = 1;        // right justify output
    ADCON2bits.ACQT = 0b010;    // configure acquisition time
    ADCON2bits.ADCS = 0b010;    // removes need for wait time
    ADCON0bits.ADON = 1;        // enable A/D converter module

    // configuration of interrupts
    RCONbits.IPEN = 1;          // enable interrupt priority levels
    INTCONbits.GIE = 1;         // enable high priority interrupts
    INTCONbits.PEIE = 1;        // enable peripheral interrupts
    INTCONbits.INT0IE = 0;      // disable external interrupt
    INTCONbits.RBIE = 0;


    // configuration of motor 1 timer bits
    T3CONbits.TMR3ON = 0;   // turn off timer 3 while configuring
    T3CONbits.RD16 = 1;     // configure timer 1 to 16 bit
    T3CONbits.T3CKPS1 = 0;  // configure prescaler to 1:8
    T3CONbits.T3CKPS0 = 0;
    T3CONbits.TMR3CS = 0;   // select internal clock
    PIR2bits.TMR3IF = 0;    // turn off timer 3 interrupt flag
    PIE2bits.TMR3IE = 1;    // enable timer 3 interrupt

    // configuration of motor 2 timer bits
    T1CONbits.TMR1ON = 0;   // turn timer 1 off while configuring
    T1CONbits.RD16 = 1;     // configure timer 1 to 16 bit
    T1CONbits.T1CKPS1 = 0;  // configure timer 1 prescaler as 1:8
    T1CONbits.T1CKPS0 = 0;
    T1CONbits.TMR1CS = 0;   // select internal clock
    PIR1bits.TMR1IF = 0;    // turn off timer 1 interrupt flag
    PIE1bits.TMR1IE = 1;    // enable timer 1 interrupt

    T1CONbits.TMR1ON = 1;   // turn timers for stepping motors on
    T3CONbits.TMR3ON = 1;

    while(1)
    {    
        //while(ADCON0bits.DONE); // wait for conversion to finish
        //adc = adc_convert(0);   // get analog reading from channel 0
    } 
    
}

