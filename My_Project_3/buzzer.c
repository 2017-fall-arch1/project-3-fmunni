#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"


#define MIN_PERIOD 1000
#define MAX_PERIOD 4000


static unsigned int period =4000;
static signed int rate = 12000;



void buzzer_init()
{
    /* 
       Direct timer A output "TA0.1" to P2.6.  
        According to table 21 from data sheet:
          P2SEL2.6, P2SEL2.7, anmd P2SEL.7 must be zero
          P2SEL.6 must be 1
        Also: P2.6 direction must be output
    */
    timerAUpmode();		/* used to drive speaker */
    P2SEL2 &= ~(BIT6 | BIT7);
    P2SEL &= ~BIT7; 
    P2SEL |= BIT6;
    P2DIR = BIT6;		/* enable output to speaker (P2.6) */

   // buzzer_set_period(1000);	/* start buzzing!!! */
}



void buzzer_calculation(){
    period +=rate;
    if((rate >0 && (period > MAX_PERIOD)) || (rate <0 && (period < MIN_PERIOD))){
        rate = -rate;
        period += (rate <<1);
    }
    buzzer_period(period);
} 
    
    void buzzer_period(short cycles){
        CCR0 = cycles;
        CCR1 = cycles >> 1;
    }
    
    void GameWinner (int win){
        switch(win){
            case 0:
                buzzer_calculation(4000);
                break;
            case1 :
                buzzer_calculation(6000);
                break;
            case 2:
                break;
        }
            
    }

    
    
  

