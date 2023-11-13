#include <xc.h>
#include <stdint.h>
#include "config.h"
#define _XTAL_FREQ 4000000
#define Rev RB0
#define Lv0 RB1
#define Lv1 RB2
#define Lv2 RB3
#define Lv3 RB4

void PWM1SetDuty(uint16_t DC){
    //checking if DC value isn't bigger than 10-bit
    if(DC < 1024){
        CCP1Y = DC & 1;
        CCP1X = DC & 2;
        CCPR1L = DC >> 2;
    }
}
void main(void) {
    //make first 5 pins of the port B as input from RB0~RB4
    TRISB = 0x1F;
    //configure the CCP1 module as PWM
    CCP1M2 = 1;
    CCP1M3 = 1;
    //configure RC2/CCP1 as output
    TRISC2 = 0;
    //TMR2 prescaler
    T2CKPS0 = 1;
    T2CKPS1 = 0;
    //preload the PR2 with the value you got from the equation to set the frequency
    PR2 = 124;
    //timer2 on
    TMR2ON = 1;
    //setting the direction of the motor
    TRISD0 = 0;
    TRISD1 = 0;
    // Initially (0, 1) Say it's ClockWise rotation!
    // For Reversing The Direction Write (1, 0)
    RD0 = 0;
    RD1 = 1;
    while(1){
      if(Rev){
          //----------------SAFETY ADJUSTMENT-----------------------
          
          if(RD0 == 0){
              RD1 = ~RD1; 
              RD0 = ~RD0;
          }
          else{
          RD0 = ~RD0;
          RD1 = ~RD1;
          }
          //the reason why we are doing this instead of directly toggling their state is to make sure
          //that there won't be a specific moment where both of them will be equal to 1
          //for example: if RD0 = 0 AND RD1 = 1, when you toggle RD0 first then at a certain moment >> RD0 = 1 RD1 = 1 
          //ofcourse this moment is very small however this moment will be repeated a lot and so fast that it will cause
          //damage to the transistors
          //that's why we check first if RD0 = 0, then this means that RD1 = 1 so in that case change RD1 first to 0
          //so that both of them will be 0, and then change RD0 to 1
          //and for the other else statement then just toggle them
          
          __delay_ms(500); // wait for the button to be released
      }
      if(Lv0){
          PWM1SetDuty(0);
          __delay_ms(100);//wait for the button to be released 
      }
      if(Lv1){
          PWM1SetDuty(250);
          __delay_ms(100);//wait for the button to be released 
      }
      if(Lv2){
          PWM1SetDuty(375);
          __delay_ms(100);//wait for the button to be released 
      }
      if(Lv3){
          PWM1SetDuty(500);
          __delay_ms(100);//wait for the button to be released 
      }
      __delay_ms(10);  // Wait To Reduce The Simulation Overloading
    }
    return;
}
