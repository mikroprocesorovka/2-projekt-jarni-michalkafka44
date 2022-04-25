#include "stm8s.h"
#include "milis.h"
#include <stdio.h>
#include "swi2c.h"
#include "stm8_hd44780.h"

#define _ISOC99_SOURCE
#define _GNU_SOURCE

#define BUZZER_PO GPIOB
#define BUZZER_PI  GPIO_PIN_7
#define BUZZER_HIGH   GPIO_WriteHigh(BUZZER_PO, BUZZER_PI)
#define BUZZER_LOW  GPIO_WriteLow(BUZZER_PO, BUZZER_PI)



void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz
    GPIO_Init(BUZZER_PO, BUZZER_PI, GPIO_MODE_OUT_PP_LOW_SLOW);

    BUZZER_HIGH;

    init_milis();
    swi2c_init();
    lcd_init();

}


int main(void)
{
    uint32_t time = 0;
    uint8_t precteno[10] = {0,0,0,0,0,0,0,0,0,0};
    uint8_t zapsano[10] = {0,0,0,0,0,0,0,0,0,0};
    uint8_t err;

    char text[32];
    char ddmmmrr[32];



    setup();

    /*error=swi2c_read_buf(0b11010000,0x00,precteno,4) */
    zapsano[0] = 0x30;  // sekundy
    zapsano[1] = 0x04;  // minuty
    zapsano[2] = 0x23;  // hodiny
    zapsano[3] = 0x06;  // den v týdnu
    zapsano[4] = 0x23;  // den
    zapsano[5] = 0x04;  // měsíc
    zapsano[6] = 0x22;  // rok

    // když tento řádek odkomentuješ, tak se čas zapíše do obvodu
    //printf("ErrorCode: %X\n",  swi2c_write_buf(0x68, 0x00, zapsano, 7));

   

    while (1) {
        if (milis() - time > 1000 /*&& BTN_PUSH*/) {
            
            

            // čtu RTC
            err = swi2c_read_buf(0x68, 0x00, precteno, 7);

            if(precteno[0] == 0){
                BUZZER_LOW;
            }
            else{
                BUZZER_HIGH;
            }



            lcd_gotoxy(0,0);
            
            lcd_puts("cas:");

            lcd_gotoxy(8,0);
            sprintf(text,"%d%d:%d%d:%d%d ",       // v RTC obvodu je čas uložen v BCD ************************************************************
                    precteno[2] >> 4, precteno[2] & 0x0F,
                    precteno[1] >> 4, precteno[1] & 0x0F,
                    precteno[0] >> 4, precteno[0] & 0x0F);
            lcd_puts(text);

            time = milis();
        }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
