/* -------------------------------------------------------
                      uart.h

     Library-Header fuer die serielle Schnittstelle

     MCU   :  STM8S103F3
     Takt  :  interner Takt 16 MHz

     18.05.2016  R. Seelig
   ------------------------------------------------------ */

#ifndef in_uartinit
  #define in_uartinit

  #include "stm8s.h"
  #include <stdarg.h>
  #include <stdint.h>

  #define readint_enable          0

  // Bei Verwendung eines STM8S105K4 USART-Schnittstelle 2 verwenden
  #ifdef __stm8s105k4__
    #define use_usart             2
  #else
    #define use_usart             1
  #endif

  void uart_putchar(char ch);
  void uart_putstring(char *p);
  char uart_getchar(void);
  char uart_ischar(void);
  void uart_init(unsigned int baudrate);
  void uart_putstring(char *p);

  #if (readint_enable == 1)
    int16_t readint(void);
  #endif

  #define uart_prints(s)   uart_putstring(s)

#endif
