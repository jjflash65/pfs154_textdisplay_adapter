/* -------------------------------------------------------
                      uart.c

     Library fuer die serielle Schnittstelle

     MCU   :  STM8S103F3
     Takt  :  interner Takt 16 MHz

     18.05.2016  R. Seelig
   ------------------------------------------------------ */

#include <stdarg.h>
#include "stm8s.h"
#include "uart.h"


  #if (use_usart == 1)

    #define USARTx_SR           USART1_SR
    #define USARTx_DR           USART1_DR
    #define USARTx_BRR1         USART1_BRR1
    #define USARTx_BRR2         USART1_BRR2
    #define USARTx_CR1          USART1_CR1
    #define USARTx_CR2          USART1_CR2
    #define USARTx_CR3          USART1_CR3
    #define USARTx_CR4          USART1_CR4
    #define USARTx_CR5          USART1_CR5
    #define USARTx_GTR          USART1_GTR
    #define USARTx_PSCR         USART1_PSCR

  #endif


  #if (use_usart == 2)

    #define USARTx_SR           USART2_SR
    #define USARTx_DR           USART2_DR
    #define USARTx_BRR1         USART2_BRR1
    #define USARTx_BRR2         USART2_BRR2
    #define USARTx_CR1          USART2_CR1
    #define USARTx_CR2          USART2_CR2
    #define USARTx_CR3          USART2_CR3
    #define USARTx_CR4          USART2_CR4
    #define USARTx_CR5          USART2_CR5
    #define USARTx_GTR          USART2_GTR
    #define USARTx_PSCR         USART2_PSCR

  #endif



void uart_putchar(char ch)
{
  while(!(USARTx_SR & USART_SR_TXE));   // warten bis letztes Zeichen gesendet ist
  USARTx_DR = ch;                       // Zeichen senden
}


void uart_putstring(char *p)
{
  do
  {
    uart_putchar( *p );
  } while( *p++);
}



/* ------------------------------------------------------------
                         UART_GETCHAR
     wartet so lange, bis auf der seriellen Schnittstelle
     ein Zeichen eintrifft und liest dieses ein
   ------------------------------------------------------------ */
char uart_getchar(void)
{
  while(!(USARTx_SR & USART_SR_RXNE));    // solange warten bis ein Zeichen eingetroffen ist
  return USARTx_DR;
}

/* ------------------------------------------------------------
                         UART_ISCHAR
      testet, ob auf der seriellen Schnittstelle ein Zeichen
      zum Lesen ansteht (liest dieses aber nicht und wartet
      auch nicht darauf)
   ------------------------------------------------------------ */
char uart_ischar(void)
{
  return (USARTx_SR & USART_SR_RXNE);
}


/* ------------------------------------------------------------
                            UART_INIT
      Initialisieren der seriellen Schnittstelle. Mit internem
      Taktgeber funktioniert die Schnittstelle nur bis
      57600 Baud
   ------------------------------------------------------------ */

void uart_init(unsigned int baudrate)
{

  PD_DDR |= (1 << PD4);                                 // TxD Leitung als Output
  PD_CR1 |= (1 << PD4);

  USARTx_CR2 |= USART_CR2_TEN;                          // TxD enabled
  USARTx_CR2 |= USART_CR2_REN;                          // RxD enable
  USARTx_CR3 &= ~(USART_CR3_STOP1);                     // Stopbit

  // Baudratendivisor:
  //   Divisior = F_CPU / Baudrate
  //   Nibbles des Divisors N3 N2 N1 N0
  //   Bsp.:   16 MHz / 9600 bd = 1667d = 0683h
  //      Hier waere dann:
  //        N3 = 0   / N2 = 6  / N1 = 8  / N0 = 3

  //      BRR1 bekaeme N2 und N1 => 0x68
  //      BRR2 bekaeme N3 und N0 => 0x03

  //  USARTx_BRR1 = 0x68; // 9600 Baud
  //  USARTx_BRR2 = 0x03;

  switch (baudrate)
  {
    case 11520 :
    {
      USARTx_BRR1 = 0x08;
      USARTx_BRR2 = 0x0b;
      break;
    }
    case 57600 :
    {
      USARTx_BRR1 = 0x11;
      USARTx_BRR2 = 0x06;
      break;
    }
    case 38400 :
    {
      USARTx_BRR1 = 0x1a;
      USARTx_BRR2 = 0x0a;
      break;
    }
    case 28800 :
    {
      USARTx_BRR1 = 0x22;
      USARTx_BRR2 = 0x0c;
      break;
    }
    case 19200 :
    {
      USARTx_BRR1 = 0x34;
      USARTx_BRR2 = 0x02;
      break;
    }
    case 9600 :
    {
      USARTx_BRR1 = 0x68;
      USARTx_BRR2 = 0x03;
      break;
    }
    case 4800 :
    {
      USARTx_BRR1 = 0xd0;
      USARTx_BRR2 = 0x05;
      break;
    }
    default : break;
  }

}

#if (readint_enable == 1)
  /* ------------------------------------------------------------
                              readint

       liest einen 16-Bit signed Integer auf der seriellen
       Schnittstelle ein (allerdings reicht der Eingabebereich
       nur von -32767 .. +32767).

       Korrektur ist mit der Loeschtaste nach links moeglich.
     ------------------------------------------------------------ */
  int16_t readint(void)
  {
    uint8_t   signflag= 0;
    uint16_t  sum = 0;
    uint8_t   zif;
    uint8_t   ch;

    do
    {
      ch= uart_getchar();
      if (ch== 0x0a) ch= 0x0d;

      // Ziffern auswerten
      if ((ch>= '0') && (ch<= '9'))
      {
        zif= ch-'0';

        if ((sum== 0) && (zif))        // erste Ziffer
        {
          sum= zif;
          uart_putchar(ch);
        }
        else
        if (sum < 3277)
        {
          if  (!( ((sum * 10) > 32750) && (zif > 7) ))
          {
            sum= (sum*10) + zif;
            uart_putchar(ch);
          }
        }
      }

      // letzte Eingabe loeschen
      if ((ch== 127) || (ch== 8))    // letzte Eingabe loeschen
      {
        if (sum)
        {
          sum /= 10;
          uart_putchar(8);
          uart_putchar(' ');
          uart_putchar(8);
        }
        else
        if (signflag)
        {
          uart_putchar(8);
          uart_putchar(' ');
          uart_putchar(8);
          signflag= 0;
        }
      }

      // Eingabe Minuszeichen
      if ((ch== '-') && (sum == 0))
      {
        signflag= 1;
          uart_putchar('-');
      }

    } while (ch != 0x0d);              // wiederholen bis Returnzeichen eintrifft
    if (signflag) return sum *(-1); else return sum;
  }

#endif
