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
int direction;          // Determines mouse direction
static int forward=0;
static int backward=1;
static int right=2;
static int left=3;
/* i.e. uint8_t <variable_name>; j*/

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
//#pragma config FOSC=INTRCIO, WDTE=OFF, PWRTE=OFF, MCLRE=ON, CP=OFF, CPD=OFF, BOREN=OFF, IESO=OFF, FCMEN=OFF
uint16_t adc_convert(uint8_t channel)
{
    ADCON0bits.CHS      = channel;  // reselect the given channel
    ADCON0bits.GO       = 0b1;      // start the conversion
    while(ADCON0bits.DONE);         // wait for the conversion to finish
    return (ADRESH<<8)|ADRESL;      // return the result
}

void main(void)
{   //int delay_time = 1000;
    //enable 8MHz internal clock
    OSCCON = 0b01111100;
    //enable internal PLL clock = 32MHz
    OSCTUNE = 0b01001111;
    /* Configure the oscillator for the device */
    //ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    //InitApp();

    /* TODO <INSERT USER APPLICATION CODE HERE> */
    TRISA = 0b00000001;
    TRISC = 0x00;
    TRISB = 0x08;
    TRISD = 0b00000000;

    ADCON0bits.CHS = 0b0000;    // select output channel
    ADCON0bits.GO = 0;          // A/D conversion initially idle
    ADCON1bits.VCFG = 0b00;     // configure voltage references
    ADCON1bits.PCFG = 0b1110;   // configure channel 0 for analog input
    ADCON2bits.ADFM = 1;        // right justify output
    ADCON2bits.ACQT = 0b010;    // configure acquisition time
    ADCON2bits.ADCS = 0b010;    // removes need for wait time
    ADCON0bits.ADON = 1;        // enable A/D converter module
    unsigned int adc;           // ADC reading

    // configuration of interrupt bits
    INTCONbits.GIE = 1;         // Set the Global Interrupt Enable
    INTCONbits.PEIE = 1;        // enable peripheral interrupts
    INTCONbits.INT0IE = 0;      // disable external interrupt

    // configuration of motor 1 bits
    INTCONbits.TMR0IE = 1;        // enable timer 0 overflow interrupts
    INTCONbits.INT0IF = 0;        // Clear the Timer 0 interrupt flag

    // configuration of motor 2 bits
    PIE1bits.TMR1IE = 1;        // enable interrupt for timer 1 overflow
    PIR1bits.TMR1IF = 0;        // clear timer 1 interrupt flag

    // configuration of motor timers
    TMR0H = 0b00000000;  // set timer 0 preloader
    TMR0L = 0b00000000;
    TMR1H = 0b00000000; // set timer 1 preloader
    TMR1H = 0b00000000;
    T0CON = 0b10000111; // set timer 0 prescaler and turn on timer 0
    T1CON = 0b01000001; // set timer 1 prescaler and turn on timer 1

    while(1);



 

    /*while (1)
    {
        while(ADCON0bits.DONE); // wait for conversion to finish
        adc = adc_convert(0);   // get analog reading from channel 0
    }*/




    //while(1)
    //{
        /*
        LATC = 0b10001000;
        delay(1000);
        LATC = 0b01000100;
        delay(1000);
        LATC = 0b00100010;
        delay(1000);
        LATC = 0b00010001;
        delay(1000);
        */
        //int i = 0;
        //while(i<delay_time)
        //{
        /*int step = 0b00000001;
        LATC = step;
        LATC = step * 2;
        LATC = step * 2;
        LATC = step * 2;

            LATC=0b10001000;
            delay(1000);
            LATC=0b10000100;
            delay(1000);
            LATC=0b01000010;
            delay(1000);
            LATC=0b01000001;
            delay(1000);
            LATC=0b00101000;
            delay(1000);
            LATC=0b00100100;
            delay(1000);
            LATC=0b00010010;
            delay(1000);
            LATC=0b00010001;
            delay(1000);*/
          //  i++;
        //}
   // }
}


