/* -----------------------------------------------
                   stxlcd_demo_uart.c

     Demoprogramm zum Ansteuern eines PFS154-
     Textdisplayadapter mittels UART

     MCU   :  STM32f0xx
     Takt  :

     30.10.2024  R. Seelig
   ----------------------------------------------- */

#include <libopencm3.h>

#include "sysf0xx_init.h"
#include "uart.h"
#include "stxlcd_uart.h"

#define printf         stxt_printf

const uint8_t charbmp1[8]  =                             // Bitmap fuer gefuelltes Rechteck unten
  { 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x00 };

const uint8_t charbmp2[8]  =                             // Bitmap fuer gefuelltes Rechteck oben
  { 0x00, 0x1f, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00 };

const uint8_t balkenbmp[][8] =
{
  { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 },
  { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 },
  { 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c },
  { 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e },
  { 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f },
};


#define cnt_speed    500

/* --------------------------------------------------
                            main
   -------------------------------------------------- */
int main(void)
{
  int8_t      cx;
  uint8_t     i;

  sys_init();
  delay(1000);

  stxt_init();
  stxt_contrast(25);
  delay(50);

  while(1)
  {
    stxt_clrscr();
    gotoxy(3,1);

    stxt_putchar(0x0f);             // Cursor aus
    printf("Progressbar");
    for (i= 0; i<5; i++)
    {
      stxt_senduserchar(i, &balkenbmp[i][0]);
    }
    for (i= 0; i< 80; i++)
    {
      gotoxy((i / 5)+1, 2);
      printf("%c",i % 5);
      delay(20);
    }
    delay(2000);

    stxt_clrscr();
    printf("Text\nlf 1");
    delay(2000);
    printf("\nlf 2");
    delay(2000);
    printf("\rvorne");
    delay(2000);

    stxt_clrscr();
    gotoxy(1,1);
    printf("%l  UserChars");

    stxt_senduserchar(1, &charbmp1[0]);
    stxt_senduserchar(2, &charbmp2[0]);
    for (i= 0; i< 5; i++)
    {
      printf("%p%P%c%c",1,2,1,2);
      delay(200);
      printf("%p%P%c%c",1,2,2,1);
      delay(200);
    }

    stxt_clrscr();
    printf("Cursor an%c", 0x0e);
    delay(2000);
    gotoxy(1,1);
    printf("%cCursor aus",0x0f);
    delay(2000);

    // vordefinierte User-Zeichen ausgeben
    stxt_clrscr();
    gotoxy(1,1);
    printf("Additional Chars%c");
    printf("%P%l",2);
    gotoxy(1,2);
    for (i= 0; i< 8; i++)
    {
      stxt_addch(i,i);
      printf("%c ",i);
    }
    delay(2000);

    printf("%P%l",2);
    gotoxy(1,2);
    for (i= 0; i< 7; i++)
    {
      stxt_addch(i+8,i);
      printf("%c ",i);
    }
    delay(2000);

    stxt_clrscr();
    gotoxy(1,1); printf("Balkenzeichen");
    gotoxy(1,2);
    for (i= 0; i< 8; i++)
    {
      stxt_addch(i+16,i);
      printf("%c",i);
    }
    delay(2000);

    stxt_clrscr();
    gotoxy(1,1); printf("Quadratzeichen");
    gotoxy(1,2);
    stxt_addch(15,1);
    i= 3;
    printf("%d%c= %d ", i,1,i*i);
    delay(2000);
    stxt_clrscr();
    printfkomma= 1;
    gotoxy(1,1); printf("Tabulator");

    gotoxy(1,2); printf("%k\t%k\t%k\t%k",45, 18, 39, 22);
    delay(2000);

    stxt_clrscr();
    stxt_addch(9,2);
    gotoxy(1,1); printf("Z%chler:",2);

    for (cx= -11; cx< 11; cx++)
    {
      printf("%p%P%d    ",3,2,cx);
      delay(500);
    }
  }
}
