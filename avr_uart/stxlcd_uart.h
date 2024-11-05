/* ---------------------------------------------------------
                          stxlcd_uart.h

     Header Softwaremodul zum Ansteuern eines PFS154-
     Textdisplayadapter mittels UART

     MCU   : ATmegaxx8

     Baudrate und Pin siehe softuart.h

     27.10.2024   R. Seelig
   --------------------------------------------------------- */

#ifndef in_stxlcd_uart
  #define in_stxlcd_uart

  #include <avr/io.h>
  #include <util/delay.h>
  #include <stdint.h>

  #include "avr_gpio.h"
  #include "softuart.h"

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
  void stxt_printf(const uint8_t *s,...);
  void stxt_init(void);

  void stxt_contrast(uint8_t val);


  #define stxt_clrscr()               { stxt_putchar(0x82); _delay_ms(80); }
  #define stxttiny_printf(str,...)    (stxt_printf(PSTR(str), ## __VA_ARGS__))

#endif
