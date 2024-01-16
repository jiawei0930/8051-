#include "8051.h"
#include "LED_Display.h"

unsigned char SegTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char ScanTable[]={0xFE,0xFD,0xFB,0xF7};

void LED_Display(int mode , int buttom , int pitch_ptr , int speed , int pause)
{
	int i = 0;
	/* Playing Mode and Recording Mode */
	if ( mode <　3)
    {      
    	/* First LED , Show MODE */
    	P2 = ScanTable[0];       
        P0 = SegTable[mode];
        delay(200); 
        
        /* Second LED , Show '-' If this pitch is under central C */
        if ( pitch_ptr < 2)
        {
            P2 = ScanTable[1];         
            P0 = 0xBF;
            delay(200);
        }
        /* Thrid LED , Show Pitch */
        P2 = ScanTable[2];
        P0 = SegTable[absolute(pitch_ptr-2)];
        delay(200);
        
        /* Fourth LED , Show the note from keypad */
        if (buttom <=7 && buttom >= 1)
        {  
           P2 = ScanTable[3];         
           P0 = SegTable[buttom];
           delay(200);
        }

	}
	/* Play-Record Mode */
	else
	{
		/* First LED , Show MODE */
		P2 = ScanTable[0];       
        P0 = SegTable[mode]; 
        delay(200);

        if ( pause)
        	return ;
        speed=speedTable(speed); 
        if ( speed < 0)     /* PLAY_SPEED is slower than RECORD_SPEED */
        {
   	        /* Second LED , Show '-' */
   	        P2 = ScanTable[1];         
            P0 = 0xBF;
            delay(200);
            /* Thrid LED , Show Speed */
            P2 = ScanTable[2];
            P0 = SegTable[absolute(speed)];
            delay(200);
       }
       else 
       {
   	        /* Thrid LED , Show Speed */
   	        P2 = ScanTable[2];
            P0 = SegTable[absolute(speed)];
            delay(200);
       }
   }

}
void show_scale(int scale)
{
   if (scale == 0)
   	 return ;
   P2 = ScanTable[3];         
   P0 = SegTable[scale];
}

void show_check()
{
   int i;
   for (i = 0 ;i < 4 ; ++i)
   {
   	  P2 = ScanTable[i];
   	  P0 = 0x7f;
   	  delay(200);   
   }
}

void delay(int count) 
{
    int i;
    for (i = 0; i < count; i++);
}

int absolute(int x)
{
    if (x > 0)
        return x ;
    else 
        return -x ;
}

int speedTable(int speed)
{
    switch (speed)
   {
        case 3:
  	        return  2;
  	    case 1:
  	        return  4;
  	    case 12:   
  	        return -2;
  	    case 24:
  	        return -4;   
   }
   return 0;
}