/* ---------------------------------------------------------
                          stxlcd_uart.h

     Header Softwaremodul zum Ansteuern eines PFS154-
     Textdisplayadapter mittels UART

     MCU   :  STM8S103F3
     Takt  :  interner Takt 16 MHz

     TxD-Pin an STM8x03 ist PD5 (Pin2 TSSOP20 Gehauese)

     29.10.2024   R. Seelig
   --------------------------------------------------------- */

#ifndef in_stxlcd_uart
  #define in_stxlcd_uart

  #include <stdint.h>

  #include <libopencm3.h>

  #include "sysf0xx_init.h"
  #include "uart.h"


  #define enable_stxt_printf       1

  #if (enable_stxt_printf == 1)
    extern char printfkomma;
  #endif

  extern uint8_t wherex, wherey;

  #define cursor_on   0x0e
  #define cursor_off  0x0f


  /* --------------------------------------------------------
                            Prototypen
     -------------------------------------------------------- */

  void stxt_init(void);
  void stxt_putchar(char ch);
  void gotoxy(uint8_t x, uint8_t y);
  void stxt_addch(uint8_t ch, uint8_t pos);
  void stxt_senduserchar(uint8_t nr, const uint8_t *userch);
  void stxt_shiftleft(uint8_t anz);
  void stxt_shiftright(uint8_t anz);
  void stxt_printf(const char *s,...);
  void stxt_init(void);

  void stxt_contrast(uint8_t val);


  #define stxt_clrscr()               { stxt_putchar(0x82); delay(80); }

#endif
