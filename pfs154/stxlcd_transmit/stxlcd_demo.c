/* ---------------------------------------------------------
                         stxlcd_demo.c

     Demoprogramm zur Ansteuerung eines Textdisplays,
     welches an einen PFS154 Controller angeschlossen ist.
     ( stxlcd_recv )

     MCU      : PFS1xx
     Compiler : SDCC 4.01 oder neuer

     zusaetzliche Hardware:
       PFS154 - Textdisplay

             +----------------+
         PB4 | 1   |___|   16 | PB3
         PB5 | 2           15 | PB2
         PB6 | 3           14 | PB1
         PB7 | 4  PFS154   13 | PB0
              ----------------
         VDD | 5 (1)   (8) 12 | GND
         PA7 | 6 (2)   (7) 11 | PA0
 ICPDA / PA6 | 7 (3)   (6) 10 | PA4
   Vpp / PA5 | 8 (4)   (5)  9 | PA3 / ICPCK
             +----------------+


     19.10.2024   R. Seelig
   --------------------------------------------------------- */

#include <stdint.h>
#include "pdk_init.h"
#include "pfs1xx_gpio.h"
#include "delay.h"

#include "stxlcd.h"

const uint8_t charbmp1[8]  =                             // Bitmap fuer gefuelltes Rechteck unten
  { 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x00 };

const uint8_t charbmp2[8]  =                             // Bitmap fuer gefuelltes Rechteck oben
  { 0x00, 0x1f, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00 };


/* --------------------------------------------------
                            main
   -------------------------------------------------- */
int main(void)
{
  int16_t i;
  int16_t cx;

  // Puffer fuer integer to string Konvertierung. Puffergroesse
  // ergibt sich aus max. 5 Ziffern, einem "-" Vorzeichen, einem
  // eventuellen Komma und dem abschliessenden 0-Byte als Ende-
  // kennung des Strings

  char strpuffer[8];

  stx_init();
  stxt_contrast(40);

  while(1)
  {
    stxt_clrscr();
    stxt_puts("PFS\n\rDisplayadapter");
    delay(2000);

    // Linefeed - Demo
    stxt_clrscr();
    stxt_puts("Text\nlf 1");
    delay(2000);
    stxt_puts("\nlf 2");
    delay(2000);
    stxt_puts("\rvorne");
    delay(2000);

    // Userchar - Demo
    stxt_clrscr();
    stxt_puts("  UserChars");
    stxt_senduserchar(1, &charbmp1[0]);
    stxt_senduserchar(2, &charbmp2[0]);
    for (i= 0; i< 5; i++)
    {
      gotoxy(1,2);
      stxt_putchar(1);
      stxt_putchar(2);
      delay(200);
      gotoxy(1,2);
      stxt_putchar(2);
      stxt_putchar(1);
      delay(200);
    }

    // Cursorblinken an/aus
    stxt_clrscr();
    stxt_puts("Cursor an");
    stxt_putchar(0x0e);
    delay(2000);
    stxt_puts("%lCursor aus");
    stxt_putchar(0x0f);
    delay(2000);

    // Tabulator-Demo
    stxt_clrscr();
    stxt_puts("Tabulator");
    gotoxy(1,2);
    stxt_puts("4.5\t3.2\t8.4\t12.1");
    delay(2000);



    // vordefinierte User-Zeichen ausgeben
    // additional chars
    delay(2000);
    stxt_clrscr();
    gotoxy(1,1);
    stxt_puts("Additional Chars");
    gotoxy(1,2);
    for (i= 0; i< 8; i++)
    {
      stxt_addch(i,i);
      stxt_putchar(i);
      stxt_putchar(' ');
    }
    delay(2000);

    stxt_puts("%l");
    for (i= 0; i< 7; i++)
    {
      stxt_addch(i+8,i);
      stxt_putchar(i);
      stxt_putchar(' ');
    }
    delay(2000);

    // Balkenzeichen
    stxt_clrscr();
    gotoxy(1,1);
    stxt_puts("Balkenzeichen");
    gotoxy(1,2);
    for (i= 0; i< 8; i++)
    {
      stxt_addch(i+16,i);
      stxt_putchar(i);
    }
    delay(2000);

    // Quadratzeichen ( hier: i^2 )
    stxt_clrscr();
    gotoxy(1,1);
    stxt_puts("Quadratzeichen");
    gotoxy(1,2);
    stxt_addch(15,1);
    i= 12;
    stxt_itoa(i, 0, &strpuffer[0]);
    stxt_puts(&strpuffer[0]);
    stxt_puts("%1 = ");
    i= i*i;
    stxt_itoa(i, 0, &strpuffer[0]);
    stxt_puts(&strpuffer[0]);
    delay(2000);

    // einfacher Counter
    stxt_clrscr();
    stxt_addch(9,1);                     // dt. Umlaut a an Speicherstelle 1
    stxt_addch(4,2);                     // Pfeil nach links an Speicherstelle 2
    stxt_addch(5,3);                     // Pfeil nach rechts an Speicherstelle 3

    gotoxy(3,1); stxt_puts("%2%2 Z%1hler %3%3");

    cx= -15;
    while (cx < 16)
    {
      stxt_itoa(cx, 0, &strpuffer[0]);
      gotoxy(2,2);
      stxt_puts(&strpuffer[0]);
      stxt_puts("      ");
      cx++;
      delay(200);
    }
    delay(2000);

  }
}

