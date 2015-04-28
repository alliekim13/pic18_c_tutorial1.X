/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include <stdio.h>
#include <stdlib.h>
#include <pic18f4620.h>
#include <time.h>
#include <xc.h>
#define _XTAL_FREQ 32000000

#endif
uint8_t lmotorstephigh; // timer preloader values
uint8_t lmotorsteplow;
uint8_t rmotorstephigh;
uint8_t rmotorsteplow;
unsigned int leftadc;       // ADC reading
unsigned int rightadc;       // ADC reading
unsigned int middleadc;       // ADC reading
int lstep;              // motor step count of left motor
int rstep;              // motor step count of right motor
int rstepcount;
int lstepcount;
int lturn;
int lturncount;
unsigned int ustep;
unsigned int rightturnstep;     //
unsigned int lturnstep;
unsigned int rightstep;
unsigned int forward;
unsigned int right;
unsigned int left;
unsigned int uturn;
unsigned int has_left_wall; // Boolean variable for wall presence
unsigned int has_right_wall;
unsigned int has_front_wall;
unsigned int prev_adc;  

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* High-priority service */

#if defined(__XC) || defined(HI_TECH_C)
#elif defined (__18CXX)
#pragma code high_isr=0x08
#pragma interrupt high_isr
void high_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
void high_priority interrupt high_isr(void)
{

    if (PIR2bits.TMR3IF && forward && lstepcount < 1000)
    {   // set speed at which motor steps

    // step left motor once 1230 forward
    if (lstep == 1)
        {   LATCbits.LATC3 = 0;
            LATCbits.LATC1 = 1;
            LATCbits.LATC0 = 0;
            LATCbits.LATC2 = 0;
            lstep++;
        }
    else if (lstep == 2)
        {   LATCbits.LATC3 = 0;
            LATCbits.LATC2 = 1;
            LATCbits.LATC0 = 0;
            LATCbits.LATC1 = 0;
            lstep++;
        }
    else if (lstep == 3)
        {   LATCbits.LATC3 = 1;
            LATCbits.LATC2 = 0;
            LATCbits.LATC1 = 0;
            LATCbits.LATC0 = 0;
            lstep++;
        }  
       else if (lstep == 4)
        {   LATCbits.LATC2 = 0;
            LATCbits.LATC1 = 0;
            LATCbits.LATC0 = 1;
            LATCbits.LATC3 = 0;
            lstep = 1;
            lstepcount++;
        }


    PIR2bits.TMR3IF = 0;        // turn off timer 3 interrupt flag
    T3CONbits.TMR3ON = 1;       // turn on timer 3

    TMR3H = lmotorstephigh;     // set timer 3 preloader
    TMR3L = lmotorsteplow;
    }  

    // step right motor once
    if (PIR1bits.TMR1IF && forward && rstepcount < 1000)
    {   // set speed at which motor steps (temporary)
        // forward 4765

        // step right motor once
        if (rstep == 1)
        {   LATCbits.LATC6 = 0;
            LATCbits.LATC7 = 0;
            LATCbits.LATC4 = 1;
            LATCbits.LATC5 = 0;
            rstep++;
        }
        else if (rstep == 2)
        {   LATCbits.LATC4 = 0;
            LATCbits.LATC5 = 0;
            LATCbits.LATC7 = 1;
            LATCbits.LATC6 = 0;
            rstep++;
        }
        else if (rstep == 3)
        {   LATCbits.LATC4 = 0;
            LATCbits.LATC5 = 0;
            LATCbits.LATC6 = 1;
            LATCbits.LATC7 = 0;
            rstep++;
        }
        else if (rstep == 4)
        {   LATCbits.LATC5 = 1;
            LATCbits.LATC6 = 0;
            LATCbits.LATC7 = 0;
            LATCbits.LATC4 = 0;
            rstep = 1;
            rstepcount++;
        }
        
        PIR1bits.TMR1IF = 0;        // turn timer 1 interrupt flag off
        T1CONbits.TMR1ON = 1;       // turn timer 1 on

        TMR1H = rmotorstephigh;     // set timer 1 preloader
        TMR1L = rmotorsteplow;
    }

    // if we are turning around
    if (PIR2bits.TMR3IF && lturn && lturncount < uturn)
    {
        if (lturnstep == 1)
        {   LATCbits.LATC0 = 1;
            LATCbits.LATC1 = 0;
            LATCbits.LATC2 = 0;
            LATCbits.LATC3 = 0;
            LATCbits.LATC4 = 1;
            LATCbits.LATC5 = 0;
            LATCbits.LATC6 = 0;
            LATCbits.LATC7 = 0;
            lturnstep++;
        }
        else if (lturnstep == 2)
        {   LATCbits.LATC0 = 0;
            LATCbits.LATC1 = 0;
            LATCbits.LATC2 = 0;
            LATCbits.LATC3 = 1;
            LATCbits.LATC4 = 0;
            LATCbits.LATC5 = 0;
            LATCbits.LATC6 = 0;
            LATCbits.LATC7 = 1;
            lturnstep++;
        }
        else if (lturnstep == 3)
        {   LATCbits.LATC0 = 0;
            LATCbits.LATC1 = 0;
            LATCbits.LATC2 = 1;
            LATCbits.LATC3 = 0;
            LATCbits.LATC4 = 0;
            LATCbits.LATC5 = 0;
            LATCbits.LATC6 = 1;
            LATCbits.LATC7 = 0;
            lturnstep++;
        }
        else if (lturnstep == 4)
        {   LATCbits.LATC0 = 0;
            LATCbits.LATC1 = 1;
            LATCbits.LATC2 = 0;
            LATCbits.LATC3 = 0;
            LATCbits.LATC4 = 0;
            LATCbits.LATC5 = 1;
            LATCbits.LATC6 = 0;
            LATCbits.LATC7 = 0;  
            lturnstep = 1;
            lturncount++;
        }
    PIR2bits.TMR3IF = 0;       // turn timer 1 interrupt flag off
    T3CONbits.TMR3ON = 1;
    TMR3H = rmotorstephigh;    // set timer 1 preloader
    TMR3L = rmotorsteplow;     // set timer 1 preloader
    }
}


/* Low-priority interrupt routine */
#if defined(__XC) || defined(HI_TECH_C)
void low_priority interrupt low_isr(void)
#elif defined (__18CXX)
#pragma code low_isr=0x18
#pragma interruptlow low_isr
void low_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
{

      /* This code stub shows general interrupt handling.  Note that these
      conditional statements are not handled within 3 seperate if blocks.
      Do not use a seperate if block for each interrupt flag to avoid run
      time errors. */

#if 0

      /* TODO Add Low Priority interrupt routine code here. */

      /* Determine which flag generated the interrupt */
      if(<Interrupt Flag 1>)
      {
          <Interrupt Flag 1=0>; /* Clear Interrupt Flag 1 */
      }
      else if (<Interrupt Flag 2>)
      {
          <Interrupt Flag 2=0>; /* Clear Interrupt Flag 2 */
      }
      else
      {
          /* Unhandled interrupts */
      }

#endif

}
