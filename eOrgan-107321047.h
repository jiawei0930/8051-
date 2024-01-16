#define TIMER_VAL    55536          //It means timer interrupt of 0.01 s 
#define RECORD_SPEED 6
#define RECORD_MAX_SIZE 1000

struct Note
{
   unsigned int scale;
   unsigned int pitch; 
};
struct Note __xdata RECORD_DATA [RECORD_MAX_SIZE];     

int BUTTOM        ;
int MODE          ;
int TIMER_COUNTER ;
int PLAY_PTR      ;   
int PITCH_PTR     ;
int CHANGE_PTR    ;
int RECORD_PTR    ;
int RECORD_SIZE   ;
int PAUSE         ;
int CHANGE_PAUSE  ;
int PLAY_SPEED    ;
int CHANGE_SPEED  ;

void System_init();
void Timer_init();
void Delete_record();


