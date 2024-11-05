/* ---------------------------------------------------------
                         stxlcd.h

     Header Softwaremodul zum Ansprechen eines ueber einen
     Padauk PFS154 Controller gesteuerten Textdisplays
     mittels einer single-wire Verbindung.

     Datenleitung: PD2 (aenderbar)

     MCU      : AVR
     Compiler : avr-gcc

     zusaetzliche Hardware:
        PFS154 - Textdisplay

     19.10.2024   R. Seelig
   --------------------------------------------------------- */

#ifndef in_pfslcd
  #define in_pfslcd

  #include <avr/io.h>
  #include <util/delay.h>
  #include <avr/pgmspace.h>
  #include <stdarg.h>


  #include "avr_gpio.h"

  #define delay            _delay_ms

  #define enable_stxt_printf       1

  #if (enable_stxt_printf == 1)
    extern char printfkomma;
  #endif

  extern uint8_t wherex, wherey;

  /* --------------------------------------------------
       Pinanschluss, auf dem der PFS-Textdisplay-
       adapter angesteuert wird
     -------------------------------------------------- */

  #define stxt_init()   { PC4_output_init(); stx_set(); }
  #define stx_set()     PC4_set()
  #define stx_clr()     PC4_clr()

  /* --------------------------------------------------
     Pulslaenge und -dauer in Mikrosekunden deklarieren
     -------------------------------------------------- */

  #define pulselength    70
  #define pulsepause     20

  /* --------------------------------------------------
       Prototypen
     -------------------------------------------------- */

  void stxt_putchar(uint8_t ch);
  void stxt_sendval(uint16_t val);
  void stxt_senduserchar(uint8_t nr, const uint8_t *userch);
  void gotoxy(uint8_t x, uint8_t y);
  void stxt_printf(const uint8_t *s,...);

  /* --------------------------------------------------
       Kommandomakros
     -------------------------------------------------- */

  #define stxt_clrscr()             { stxt_sendval(0x200); delay(100); gotoxy(1,1); }
  #define stxt_shiftleft(anz)       stxt_sendval(0x500 | anz)
  #define stxt_shiftright(anz)      stxt_sendval(0x600 | anz)
  #define stxt_addch(ch, pos)       { stxt_sendval(0x300 | (ch << 3) | pos); delay(4); }
  #define stxt_contrast(val)         stxt_sendval(0x700 | val)

  #define stxttiny_printf(str,...)  (stxt_printf(PSTR(str), ## __VA_ARGS__))


#endif
