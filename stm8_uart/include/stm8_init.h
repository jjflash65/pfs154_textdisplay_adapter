/* -------------------------------------------------------
                      stm8_init.h

     Initialisierung des STM8S103, interner Takt 16 MHz,
     alle Clocks fuer Peripherieeinheiten eingeschaltet

     MCU   :  STM8S103F3
     Takt  :  interner Takt 16 MHz

     18.05.2016  R. Seelig
   ------------------------------------------------------ */

#ifndef in_stm8init
  #define in_stm8init

  #include <stdint.h>
  #include "stm8s.h"

  void sysclock_init(char xtalen);
  void delay(uint16_t cnt);
  void delay_us(uint16_t cnt);
  void int_enable(void);
  void int_disable(void);


#endif
