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
extern int direction;
extern int forward;
extern int backward;
extern int right;
extern int left;
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
void interrupt high_isr(void)
{
    if (INTCONbits.INT0IF)      // if timer 0 interrupt flag went off
    {   // clear timer 0 interrupt flag
        INTCONbits.INT0IF = 0;
        // step motor 1
        if ((PORTC & 0b00000001) == 1)
            LATC = 0b00000010;
        else if ((PORTC & 0b00000010) == 1)
            LATC = 0b00000100;
        else if ((PORTC & 0b00000100) == 1)
            LATC = 0b00001000;
        else if ((PORTC & 0b00001000) == 1)
            LATC = 0b00000001;
        else LATC = 0b00000001;
        // reset timer 0
        TMR0L = 0b00000000;
        TMR0H = 0b00000000;
    }

    if (PIR1bits.TMR1IF)
    {   // clear timer 1 interrupt flag
        PIR1bits.TMR1IF = 0;
        // step motor 2
        if ((PORTC & 0b00010000) == 1)
            LATC = 0b00100000;
        else if ((PORTC & 0b00100000) == 1)
            LATC = 0b01000000;
        else if ((PORTC & 0b01000000) == 1)
            LATC = 0b10000000;
        else LATC = 0b00010000;
        // reset timer 1
        TMR1L = 0b00000000;
        TMR1H = 0b00000000;
    }

      /* This code stub shows general interrupt handling.  Note that these
      conditional statements are not handled within 3 seperate if blocks.
      Do not use a seperate if block for each interrupt flag to avoid run
      time errors. */

    
      /* TODO Add High Priority interrupt routine code here. */

      /* Determine which flag generated the interrupt */
  //tris and lat
    //int x = 0b00001111 & PORTCbits;
    //if (direction == forward)
    //{ direction = 0;
     // if(right != 0b00001000)
//          x << 1;
//      else x >> 3;
//    }
//    else if (direction == backward)
//    {   direction = 0;
//        if (PORTCbits != 0b00001000)
//            PORTCBITS >> 1;
//        else PORTCbits << 3;
//    }
//      else
          /* Unhandled interrupts */



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
