#include "8051.h"

int keypadnum [4][4] = { {15,11,10,16},{14,3,2,1},{13,6,5,4},{12,9,8,7}};    //Change keypad 0 -> 16
int Keypad4x4(){
    unsigned char pad[] = {0xfe, 0xfd, 0xfb, 0xf7}, 
                column[] = {0x10, 0x20, 0x40, 0x80};
    int i, j, key = 0;
    for (i = 0; i < 4; i++){
        P1 = pad[i];
        for (j = 0; j < 4; j++){
            if ((P1 & column[j]) == 0){
                key =  keypadnum[i][j];
                i = 4;
                j = 4;
            } 
        }
    }
    return key;
}