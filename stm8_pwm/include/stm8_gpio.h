/* -------------------------------------------------------
                         stm8_gpio.h

     Macros zum Handling der I/O Anschluesse des STM8
     Microcontrollers.

     Jeder einzelne Anschlusspin des MCU ist als Eingang
     oder Ausgang konfigurierbar. Wird ein Anschluss als
     Eingang konfiguriert, wird automatisch der interne
     Pull-Up Widerstand eingeschaltet.

     Verwendung als Ausgang:
     ---------------------------------------------------

     -   Pxy_output_init()

         x bezeichnet den Port (bspw. PA, PB, PC, PD)
         y bezeichnet das Portbit.

     Bit 5 des Ports C muss dann wie folgt
     initialisiert werden:

         PC5_output_init();

     Setzen eines Bits auf 1:

         PC5_set();

     Setzen eines Bits auf 0 (loeschen):

         PC5_clr();

     Verwendung als Eingang:
     ---------------------------------------------------

     - Pxy_input_init()

     Bsp.: Port C, Bit 6 als Eingang:

       PC6_input_init()

     Lesen des Eingangs:

     if (isbit_PC6) PC5_set(); else PC5_clr();

     23.05.2016  R. Seelig
   ------------------------------------------------------ */


// ##############################################################
//   PORT A
// ##############################################################

//   PA0 I/O
#define PA0_output_init()  {  PA_DDR |=   0x01;         \
                              PA_CR1 |=   0x01;         \
                              PA_CR2 &= ~(0x01); }
#define PA0_set()          (  PA_ODR |=   0x01   )
#define PA0_clr()          (  PA_ODR &= ~(0x01)  )

#define PA0_input_init()   {  PA_DDR &= ~(0x01);        \
                              PA_CR1 |=   0x01;  }
#define is_PA0()           (( PA_IDR &    0x01) )


//   PA1 I/O
#define PA1_output_init()  {  PA_DDR |=   0x02;         \
                              PA_CR1 |=   0x02;         \
                              PA_CR2 &= ~(0x02); }
#define PA1_set()          (  PA_ODR |=   0x02   )
#define PA1_clr()          (  PA_ODR &= ~(0x02)  )

#define PA1_input_init()   {  PA_DDR &= ~(0x02);        \
                              PA_CR1 |=   0x02;  }
#define is_PA1()           (( PA_IDR &    0x02) >> 1)

//   PA2 I/O
#define PA2_output_init()  {  PA_DDR |=   0x04;         \
                              PA_CR1 |=   0x04;         \
                              PA_CR2 &= ~(0x04); }
#define PA2_set()          (  PA_ODR |=   0x04   )
#define PA2_clr()          (  PA_ODR &= ~(0x04)  )

#define PA2_input_init()   {  PA_DDR &= ~(0x04);        \
                              PA_CR1 |=   0x04;  }
#define is_PA2()           (( PA_IDR &    0x04) >> 2)


//   PA3 I/O
#define PA3_output_init()  {  PA_DDR |=   0x08;         \
                              PA_CR1 |=   0x08;         \
                              PA_CR2 &= ~(0x08); }
#define PA3_set()          (  PA_ODR |=   0x08   )
#define PA3_clr()          (  PA_ODR &= ~(0x08)  )

#define PA3_input_init()   {  PA_DDR &= ~(0x08);        \
                              PA_CR1 |=   0x08;  }
#define is_PA3()           (( PA_IDR &    0x08) >> 3)


//   PA4 I/O
#define PA4_output_init()  {  PA_DDR |=   0x10;         \
                              PA_CR1 |=   0x10;         \
                              PA_CR2 &= ~(0x10); }
#define PA4_set()          (  PA_ODR |=   0x10   )
#define PA4_clr()          (  PA_ODR &= ~(0x10)  )

#define PA4_input_init()   {  PA_DDR &= ~(0x10);        \
                              PA_CR1 |=   0x10;  }
#define is_PA4()           (( PA_IDR &    0x10) >> 1)


//   PA5 I/O
#define PA5_output_init()  {  PA_DDR |=   0x20;         \
                              PA_CR1 |=   0x20;         \
                              PA_CR2 &= ~(0x20); }
#define PA5_set()          (  PA_ODR |=   0x20   )
#define PA5_clr()          (  PA_ODR &= ~(0x20)  )

#define PA5_input_init()   {  PA_DDR &= ~(0x20);        \
                              PA_CR1 |=   0x20;  }
#define is_PA5()           (( PA_IDR &    0x20) >> 1)


//   PA6 I/O
#define PA6_output_init()  {  PA_DDR |=   0x40;         \
                              PA_CR1 |=   0x40;         \
                              PA_CR2 &= ~(0x40); }
#define PA6_set()          (  PA_ODR |=   0x40   )
#define PA6_clr()          (  PA_ODR &= ~(0x40)  )

#define PA6_input_init()   {  PA_DDR &= ~(0x40);        \
                              PA_CR1 |=   0x40;  }
#define is_PA6()           (( PA_IDR &    0x40) >> 1)


//   PA7 I/O
#define PA7_output_init()  {  PA_DDR |=   0x80;         \
                              PA_CR1 |=   0x80;         \
                              PA_CR2 &= ~(0x80); }
#define PA7_set()          (  PA_ODR |=   0x80   )
#define PA7_clr()          (  PA_ODR &= ~(0x80)  )

#define PA7_input_init()   {  PA_DDR &= ~(0x80);        \
                              PA_CR1 |=   0x80;  }
#define is_PA7()           (( PA_IDR &    0x80) >> 1)


// ##############################################################
//   PORT B
// ##############################################################

//   PB0 I/O
#define PB0_output_init()  {  PB_DDR |=   0x01;         \
                              PB_CR1 |=   0x01;         \
                              PB_CR2 &= ~(0x01); }
#define PB0_set()          (  PB_ODR |=   0x01   )
#define PB0_clr()          (  PB_ODR &= ~(0x01)  )

#define PB0_input_init()   {  PB_DDR &= ~(0x01);        \
                              PB_CR1 |=   0x01;  }
#define is_PB0()           (( PB_IDR &    0x01) )


//   PB1 I/O
#define PB1_output_init()  {  PB_DDR |=   0x02;         \
                              PB_CR1 |=   0x02;         \
                              PB_CR2 &= ~(0x02); }
#define PB1_set()          (  PB_ODR |=   0x02   )
#define PB1_clr()          (  PB_ODR &= ~(0x02)  )

#define PB1_input_init()   {  PB_DDR &= ~(0x02);        \
                              PB_CR1 |=   0x02;  }
#define is_PB1()           (( PB_IDR &    0x02) >> 1)

//   PB2 I/O
#define PB2_output_init()  {  PB_DDR |=   0x04;         \
                              PB_CR1 |=   0x04;         \
                              PB_CR2 &= ~(0x04); }
#define PB2_set()          (  PB_ODR |=   0x04   )
#define PB2_clr()          (  PB_ODR &= ~(0x04)  )

#define PB2_input_init()   {  PB_DDR &= ~(0x04);        \
                              PB_CR1 |=   0x04;  }
#define is_PB2()           (( PB_IDR &    0x04) >> 2)


//   PB3 I/O
#define PB3_output_init()  {  PB_DDR |=   0x08;         \
                              PB_CR1 |=   0x08;         \
                              PB_CR2 &= ~(0x08); }
#define PB3_set()          (  PB_ODR |=   0x08   )
#define PB3_clr()          (  PB_ODR &= ~(0x08)  )

#define PB3_input_init()   {  PB_DDR &= ~(0x08);        \
                              PB_CR1 |=   0x08;  }
#define is_PB3()           (( PB_IDR &    0x08) >> 3)

//   PB4 I/O
#define PB4_output_init()  {  PB_DDR |=   0x10;         \
                              PB_CR1 |=   0x10;         \
                              PB_CR2 &= ~(0x10); }
#define PB4_set()          (  PB_ODR |=   0x10   )
#define PB4_clr()          (  PB_ODR &= ~(0x10)  )

#define PB4_input_init()   {  PB_DDR &= ~(0x10);        \
                              PB_CR1 |=   0x10;  }
#define is_PB4()           (( PB_IDR &    0x10) >> 4)


//   PB5 I/O
#define PB5_output_init()  {  PB_DDR |=   0x20;         \
                              PB_CR1 |=   0x20;         \
                              PB_CR2 &= ~(0x20); }
#define PB5_set()          (  PB_ODR |=   0x20   )
#define PB5_clr()          (  PB_ODR &= ~(0x20)  )

#define PB5_input_init()   {  PB_DDR &= ~(0x20);        \
                              PB_CR1 |=   0x20;  }
#define is_PB5()           (( PB_IDR &    0x20) >> 5)


//   PB6 I/O
#define PB6_output_init()  {  PB_DDR |=   0x40;         \
                              PB_CR1 |=   0x40;         \
                              PB_CR2 &= ~(0x40); }
#define PB6_set()          (  PB_ODR |=   0x40   )
#define PB6_clr()          (  PB_ODR &= ~(0x40)  )

#define PB6_input_init()   {  PB_DDR &= ~(0x40);        \
                              PB_CR1 |=   0x40;  }
#define is_PB6()           (( PB_IDR &    0x40) >> 1)


//   PB7 I/O
#define PB7_output_init()  {  PB_DDR |=   0x80;         \
                              PB_CR1 |=   0x80;         \
                              PB_CR2 &= ~(0x80); }
#define PB7_set()          (  PB_ODR |=   0x80   )
#define PB7_clr()          (  PB_ODR &= ~(0x80)  )

#define PB7_input_init()   {  PB_DDR &= ~(0x80);        \
                              PB_CR1 |=   0x80;  }
#define is_PB7()           (( PB_IDR &    0x80) >> 1)

// ##############################################################
//   PORT C
// ##############################################################

//   PC0 I/O
#define PC0_output_init()  {  PC_DDR |=   0x01;         \
                              PC_CR1 |=   0x01;         \
                              PC_CR2 &= ~(0x01); }
#define PC0_set()          (  PC_ODR |=   0x01   )
#define PC0_clr()          (  PC_ODR &= ~(0x01)  )

#define PC0_input_init()   {  PC_DDR &= ~(0x01);        \
                              PC_CR1 |=   0x01;  }
#define is_PC0()           (( PC_IDR &    0x01) )


//   PC1 I/O
#define PC1_output_init()  {  PC_DDR |=   0x02;         \
                              PC_CR1 |=   0x02;         \
                              PC_CR2 &= ~(0x02); }
#define PC1_set()          (  PC_ODR |=   0x02   )
#define PC1_clr()          (  PC_ODR &= ~(0x02)  )

#define PC1_input_init()   {  PC_DDR &= ~(0x02);        \
                              PC_CR1 |=   0x02;  }
#define is_PC1()           (( PC_IDR &    0x02) >> 1)

//   PC2 I/O
#define PC2_output_init()  {  PC_DDR |=   0x04;         \
                              PC_CR1 |=   0x04;         \
                              PC_CR2 &= ~(0x04); }
#define PC2_set()          (  PC_ODR |=   0x04   )
#define PC2_clr()          (  PC_ODR &= ~(0x04)  )

#define PC2_input_init()   {  PC_DDR &= ~(0x04);        \
                              PC_CR1 |=   0x04;  }
#define is_PC2()           (( PC_IDR &    0x04) >> 2)


//   PC3 I/O
#define PC3_output_init()  {  PC_DDR |=   0x08;         \
                              PC_CR1 |=   0x08;         \
                              PC_CR2 &= ~(0x08); }
#define PC3_set()          (  PC_ODR |=   0x08   )
#define PC3_clr()          (  PC_ODR &= ~(0x08)  )

#define PC3_input_init()   {  PC_DDR &= ~(0x08);        \
                              PC_CR1 |=   0x08;  }
#define is_PC3()           (( PC_IDR &    0x08) >> 3)


//   PC4 I/O
#define PC4_output_init()  {  PC_DDR |=   0x10;         \
                              PC_CR1 |=   0x10;         \
                              PC_CR2 &= ~(0x10); }
#define PC4_set()          (  PC_ODR |=   0x10   )
#define PC4_clr()          (  PC_ODR &= ~(0x10)  )

#define PC4_input_init()   {  PC_DDR &= ~(0x10);        \
                              PC_CR1 |=   0x10;  }
#define is_PC4()           (( PC_IDR &    0x10) >> 4)


//   PC5 I/O
#define PC5_output_init()  {  PC_DDR |=   0x20;         \
                              PC_CR1 |=   0x20;         \
                              PC_CR2 &= ~(0x20); }
#define PC5_set()          (  PC_ODR |=   0x20   )
#define PC5_clr()          (  PC_ODR &= ~(0x20)  )

#define PC5_input_init()   {  PC_DDR &= ~(0x20);        \
                              PC_CR1 |=   0x20;  }
#define is_PC5()           (( PC_IDR &    0x20) >> 5)


//   PC6 I/O
#define PC6_output_init()  {  PC_DDR |=   0x40;         \
                              PC_CR1 |=   0x40;         \
                              PC_CR2 &= ~(0x40); }
#define PC6_set()          (  PC_ODR |=   0x40   )
#define PC6_clr()          (  PC_ODR &= ~(0x40)  )

#define PC6_input_init()   {  PC_DDR &= ~(0x40);        \
                              PC_CR1 |=   0x40;  }
#define is_PC6()           (( PC_IDR &    0x40) >> 6)


//   PC7 I/O
#define PC7_output_init()  {  PC_DDR |=   0x80;         \
                              PC_CR1 |=   0x80;         \
                              PC_CR2 &= ~(0x80); }
#define PC7_set()          (  PC_ODR |=   0x80   )
#define PC7_clr()          (  PC_ODR &= ~(0x80)  )

#define PC7_input_init()   {  PC_DDR &= ~(0x80);        \
                              PC_CR1 |=   0x80;  }
#define is_PC7()           (( PC_IDR &    0x80) >> 7)


// ##############################################################
//   PORT D
// ##############################################################

//   PD0 I/O
#define PD0_output_init()  {  PD_DDR |=   0x01;         \
                              PD_CR1 |=   0x01;         \
                              PD_CR2 &= ~(0x01); }
#define PD0_set()          (  PD_ODR |=   0x01   )
#define PD0_clr()          (  PD_ODR &= ~(0x01)  )

#define PD0_input_init()   {  PD_DDR &= ~(0x01);        \
                              PD_CR1 |=   0x01;  }
#define is_PD0()           (( PD_IDR &    0x01) )



//   PD1 I/O
#define PD1_output_init()  {  PD_DDR |=   0x02;         \
                              PD_CR1 |=   0x02;         \
                              PD_CR2 &= ~(0x02); }
#define PD1_set()          (  PD_ODR |=   0x02   )
#define PD1_clr()          (  PD_ODR &= ~(0x02)  )

#define PD1_input_init()   {  PD_DDR &= ~(0x02);        \
                              PD_CR1 |=   0x02;  }
#define is_PD1()           (( PD_IDR &    0x02) >> 1)

//   PD2 I/O
#define PD2_output_init()  {  PD_DDR |=   0x04;         \
                              PD_CR1 |=   0x04;         \
                              PD_CR2 &= ~(0x04); }
#define PD2_set()          (  PD_ODR |=   0x04   )
#define PD2_clr()          (  PD_ODR &= ~(0x04)  )

#define PD2_input_init()   {  PD_DDR &= ~(0x04);        \
                              PD_CR1 |=   0x04;  }
#define is_PD2()           (( PD_IDR &    0x04) >> 2)


//   PD3 I/O
#define PD3_output_init()  {  PD_DDR |=   0x08;         \
                              PD_CR1 |=   0x08;         \
                              PD_CR2 &= ~(0x08); }
#define PD3_set()          (  PD_ODR |=   0x08   )
#define PD3_clr()          (  PD_ODR &= ~(0x08)  )

#define PD3_input_init()   {  PD_DDR &= ~(0x08);        \
                              PD_CR1 |=   0x08;  }
#define is_PD3()           (( PD_IDR &    0x08) >> 3)


//   PD4 I/O
#define PD4_output_init()  {  PD_DDR |=   0x10;         \
                              PD_CR1 |=   0x10;         \
                              PD_CR2 &= ~(0x10); }
#define PD4_set()          (  PD_ODR |=   0x10   )
#define PD4_clr()          (  PD_ODR &= ~(0x10)  )

#define PD4_input_init()   {  PD_DDR &= ~(0x10);        \
                              PD_CR1 |=   0x10;  }
#define is_PD4()           (( PD_IDR &    0x10) >> 4)


//   PD5 I/O
#define PD5_output_init()  {  PD_DDR |=   0x20;         \
                              PD_CR1 |=   0x20;         \
                              PD_CR2 &= ~(0x20); }
#define PD5_set()          (  PD_ODR |=   0x20   )
#define PD5_clr()          (  PD_ODR &= ~(0x20)  )

#define PD5_input_init()   {  PD_DDR &= ~(0x20);        \
                              PD_CR1 |=   0x20;  }
#define is_PD5()           (( PD_IDR &    0x20) >> 5)


//   PD6 I/O
#define PD6_output_init()  {  PD_DDR |=   0x40;         \
                              PD_CR1 |=   0x40;         \
                              PD_CR2 &= ~(0x40); }
#define PD6_set()          (  PD_ODR |=   0x40   )
#define PD6_clr()          (  PD_ODR &= ~(0x40)  )

#define PD6_input_init()   {  PD_DDR &= ~(0x40);        \
                              PD_CR1 |=   0x40;  }
#define is_PD6()           (( PD_IDR &    0x40) >> 6)


//   PD7 I/O
#define PD7_output_init()  {  PD_DDR |=   0x80;         \
                              PD_CR1 |=   0x80;         \
                              PD_CR2 &= ~(0x80); }
#define PD7_set()          (  PD_ODR |=   0x80   )
#define PD7_clr()          (  PD_ODR &= ~(0x80)  )

#define PD7_input_init()   {  PD_DDR &= ~(0x80);        \
                              PD_CR1 |=   0x80;  }
#define is_PD7()           (( PD_IDR &    0x80) >> 1)

// ##############################################################
//   PORT E
// ##############################################################

//   PE0 I/O
#define PE0_output_init()  {  PE_DDR |=   0x01;         \
                              PE_CR1 |=   0x01;         \
                              PE_CR2 &= ~(0x01); }
#define PE0_set()          (  PE_ODR |=   0x01   )
#define PE0_clr()          (  PE_ODR &= ~(0x01)  )

#define PE0_input_init()   {  PE_DDR &= ~(0x01);        \
                              PE_CR1 |=   0x01;  }
#define is_PE0()           (( PE_IDR &    0x01) )


//   PE1 I/O
#define PE1_output_init()  {  PE_DDR |=   0x02;         \
                              PE_CR1 |=   0x02;         \
                              PE_CR2 &= ~(0x02); }
#define PE1_set()          (  PE_ODR |=   0x02   )
#define PE1_clr()          (  PE_ODR &= ~(0x02)  )

#define PE1_input_init()   {  PE_DDR &= ~(0x02);        \
                              PE_CR1 |=   0x02;  }
#define is_PE1()           (( PE_IDR &    0x02) >> 1)

//   PE2 I/O
#define PE2_output_init()  {  PE_DDR |=   0x04;         \
                              PE_CR1 |=   0x04;         \
                              PE_CR2 &= ~(0x04); }
#define PE2_set()          (  PE_ODR |=   0x04   )
#define PE2_clr()          (  PE_ODR &= ~(0x04)  )

#define PE2_input_init()   {  PE_DDR &= ~(0x04);        \
                              PE_CR1 |=   0x04;  }
#define is_PE2()           (( PE_IDR &    0x04) >> 2)


//   PE3 I/O
#define PE3_output_init()  {  PE_DDR |=   0x08;         \
                              PE_CR1 |=   0x08;         \
                              PE_CR2 &= ~(0x08); }
#define PE3_set()          (  PE_ODR |=   0x08   )
#define PE3_clr()          (  PE_ODR &= ~(0x08)  )

#define PE3_input_init()   {  PE_DDR &= ~(0x08);        \
                              PE_CR1 |=   0x08;  }
#define is_PE3()           (( PE_IDR &    0x08) >> 3)

//   PE4 I/O
#define PE4_output_init()  {  PE_DDR |=   0x10;         \
                              PE_CR1 |=   0x10;         \
                              PE_CR2 &= ~(0x10); }
#define PE4_set()          (  PE_ODR |=   0x10   )
#define PE4_clr()          (  PE_ODR &= ~(0x10)  )

#define PE4_input_init()   {  PE_DDR &= ~(0x10);        \
                              PE_CR1 |=   0x10;  }
#define is_PE4()           (( PE_IDR &    0x10) >> 4)


//   PE5 I/O
#define PE5_output_init()  {  PE_DDR |=   0x20;         \
                              PE_CR1 |=   0x20;         \
                              PE_CR2 &= ~(0x20); }
#define PE5_set()          (  PE_ODR |=   0x20   )
#define PE5_clr()          (  PE_ODR &= ~(0x20)  )

#define PE5_input_init()   {  PE_DDR &= ~(0x20);        \
                              PE_CR1 |=   0x20;  }
#define is_PE5()           (( PE_IDR &    0x20) >> 5)


//   PE6 I/O
#define PE6_output_init()  {  PE_DDR |=   0x40;         \
                              PE_CR1 |=   0x40;         \
                              PE_CR2 &= ~(0x40); }
#define PE6_set()          (  PE_ODR |=   0x40   )
#define PE6_clr()          (  PE_ODR &= ~(0x40)  )

#define PE6_input_init()   {  PE_DDR &= ~(0x40);        \
                              PE_CR1 |=   0x40;  }
#define is_PE6()           (( PE_IDR &    0x40) >> 1)


//   PE7 I/O
#define PE7_output_init()  {  PE_DDR |=   0x80;         \
                              PE_CR1 |=   0x80;         \
                              PE_CR2 &= ~(0x80); }
#define PE7_set()          (  PE_ODR |=   0x80   )
#define PE7_clr()          (  PE_ODR &= ~(0x80)  )

#define PE7_input_init()   {  PE_DDR &= ~(0x80);        \
                              PE_CR1 |=   0x80;  }
#define is_PE7()           (( PE_IDR &    0x80) >> 1)
