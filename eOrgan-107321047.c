#include "keypad.h"
#include "LED_Display.h"
#include "8051.h"
#include "eOrgan-107321047.h"
__code unsigned int scaleTable[5][8] = { { 0,57844, 58780, 59439, 59789, 60434, 60991, 61504   }  ,     //低8度 * 2
                                         { 0,61720, 62135, 62506, 62679, 62985, 63264, 63512   }  ,     //低8度 
                                         { 0,63628, 63835, 64021, 64103, 64260, 64400, 64524   }  ,     //中央C 
                                         { 0,64580, 64685, 64778, 64821, 64899, 64968, 65030   }  ,     //高8度 
                                         { 0,65059, 65111, 65157, 65179, 65218, 65252, 65283   } };     //高8度 * 2
int main()
{
    System_init();
    Timer_init ();
    while(1)
    {
        BUTTOM = Keypad4x4();
        switch (BUTTOM)
       {
        /* No Keypad press */
            case 0:           
                PITCH_PTR  = CHANGE_PTR ; 
                PLAY_SPEED = CHANGE_SPEED;
                PAUSE  = CHANGE_PAUSE    ;
                if ( MODE != 3)
                   TR0 = 0; 
                break;
        /*  Adjust the pitch (+ 8 度)    */
            case 11:
                if ( CHANGE_PTR < 4 && CHANGE_PTR == PITCH_PTR)
                CHANGE_PTR ++ ;
                break;
        /*  Adjust the pitch (- 8 度)    */
            case 10:
               if ( CHANGE_PTR > 0 && CHANGE_PTR == PITCH_PTR)
                   CHANGE_PTR -- ;
                break;
        /* Back to init status also trun off recorder (Keypad 0) */
            case 16: 
               MODE = 1 ;                   
               CHANGE_PTR = 2 ;
               PLAY_PTR   = 0 ;
               TR1        = 0 ;
               TR0        = 0 ;
               TIMER_COUNTER = 0;
               CHANGE_SPEED  = RECORD_SPEED;
               break;
        /* Start Record */
            case 15:
                MODE = 2 ;
                TR1 = 1 ;
                break;
        /* Turn off Record and Play record */
            case 14:
               TR0 = 0 ;
               TR1 = 0 ;
               MODE = 3 ;
               PLAY_PTR    =  0 ;
               RECORD_SIZE = RECORD_PTR ;
               TIMER_COUNTER = 0 ;
               CHANGE_SPEED = RECORD_SPEED ;
               TR0 = 1 ;
               TR1 = 1;
               break;
        /* Pause for playing or recording */
            case 13:
                if (CHANGE_PAUSE == PAUSE)
                   CHANGE_PAUSE = !CHANGE_PAUSE ;
                break ;
        /* Adjust the speed on playing record ( 2x or 4x) */
            case 9 :
                if (CHANGE_SPEED > 1 && CHANGE_SPEED == PLAY_SPEED)
                    CHANGE_SPEED   = (int)CHANGE_SPEED / 2;
                break ; 
         /* Adjust the speed on playing record ( 0.5x or 0.25x) */
            case 8 :
                if (CHANGE_SPEED < 24 && CHANGE_SPEED == PLAY_SPEED)
                    CHANGE_SPEED  *= 2;
                break ; 
        /* Delete the record */ 
            case 12:
                if ( MODE == 1)
                {
                   while(1)
                  {
                       TR0 = 0;
                       TR1 = 0;
                       show_check();
                       BUTTOM = Keypad4x4(); 
                       if ( BUTTOM == 1)
                       {  
                           Delete_record();
                           break;
                       }
                       else if (BUTTOM == 16)
                           break;
                   }
                }
        /* Scale */
            default:
                TR0 = 1 ;
        }  //Switch end 
        LED_Display(MODE,BUTTOM,PITCH_PTR,PLAY_SPEED,CHANGE_PAUSE);
    } 
}

void System_init()
{
   P3_0          = 0  ;
   MODE          = 1  ;      
   BUTTOM        = 0  ;      
   TIMER_COUNTER = 0  ;
   RECORD_PTR    = 0  ;
   RECORD_SIZE   = 0  ;
   PLAY_PTR      = 0  ;
   CHANGE_PAUSE  = 0  ;
   CHANGE_PTR    = 2  ;    //中央 C

   PAUSE         = CHANGE_PAUSE ; 
   CHANGE_SPEED  = RECORD_SPEED ;
   PLAY_SPEED    = CHANGE_SPEED;
   PITCH_PTR     = CHANGE_PTR ;
}
void Timer_init()
{
    TMOD = 0x11;
    IE = 0x8A;
    /* Timer 0 */
    TL0 = 0x00 & 0xFF;
    TH0 = 0x00 >> 8; 

    /* Timer 1 */
    TH1 = TIMER_VAL >> 8;
    TL1 = TIMER_VAL & 0xFF;
}
void Timer0_ISR(void) __interrupt(1) __using (1) 
{
    TF0 = 0;
    if ( MODE != 3 )
    {
   	   TH0 =  scaleTable[PITCH_PTR][BUTTOM] >> 8;
       TL0 =  scaleTable[PITCH_PTR][BUTTOM] & 0xff;
       P3_0 = !P3_0;
    }
    else
    {  
       TH0 =  scaleTable[RECORD_DATA[PLAY_PTR].pitch][RECORD_DATA[PLAY_PTR].scale] >> 8;
       TL0 =  scaleTable[RECORD_DATA[PLAY_PTR].pitch][RECORD_DATA[PLAY_PTR].scale] & 0xff;
       if (RECORD_DATA[PLAY_PTR].scale != 0 && !PAUSE )
          P3_0 = !P3_0;
    }
}
void Timer1_ISR(void) __interrupt (3) __using (2) 
{   
    TH1 = TIMER_VAL >> 8;
    TL1 = TIMER_VAL & 0xFF;
    TF1 = 0;

    TIMER_COUNTER ++ ;
    /* Recording */
    if ( MODE == 2 && TIMER_COUNTER == RECORD_SPEED )
    {
        if ( BUTTOM < 8  && !PAUSE)
        {
            RECORD_DATA[RECORD_PTR].scale = BUTTOM ;
            RECORD_DATA[RECORD_PTR].pitch = PITCH_PTR ;
      
        
            if (RECORD_PTR < RECORD_MAX_SIZE)
                RECORD_PTR++;
            else
                MODE = 1 ; 
        }
       
       TIMER_COUNTER = 0;
    }
    /* Playing Record */
    if ( MODE == 3 && TIMER_COUNTER == PLAY_SPEED)
    {
        
        if (!PAUSE)
            PLAY_PTR ++ ;
        if ( PLAY_PTR > RECORD_SIZE)
             PLAY_PTR = 0;
        TIMER_COUNTER = 0;
        show_scale(RECORD_DATA[PLAY_PTR].scale);
    }
    if ( TIMER_COUNTER > PLAY_SPEED)
        TIMER_COUNTER = 0;

}

void Delete_record()
{
    int i ;
    for ( i = 0; i < RECORD_SIZE ; i ++)
    {    
        RECORD_DATA[i].scale = 0 ;
        RECORD_DATA[i].pitch = 0 ;
    }
    RECORD_SIZE = 0 ;
    PLAY_PTR    = 0 ;
    RECORD_PTR  = 0 ;
}
