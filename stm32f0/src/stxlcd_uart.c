/* ---------------------------------------------------------
                          stxlcd_uart.h

     Softwaremodul zum Ansteuern eines PFS154-
     Textdisplayadapter mittels UART

     MCU   :  STM8S103F3
     Takt  :  interner Takt 16 MHz

     29.10.2024   R. Seelig
   --------------------------------------------------------- */

#include "stxlcd_uart.h"

uint8_t wherex, wherey;


/* --------------------------------------------------
                      stxt_contrast

     stellt den Kontrast des Displays mittels PWM
     ein.

     Uebergabe:

        val : zulaessige Werte im Bereich 0..127.
              0   => hoher Kontrast
              127 => sehr blass und auf den meisten
                     Displays ist mit diesem Wert
                     nichts zu sehen
   -------------------------------------------------- */
void stxt_contrast(uint8_t val)
{
  stxt_putchar(0x87);             // 0x87 CMD, nachfolgendes Byte enthaelt Kontrastwert
  stxt_putchar(val & 0x7f);
}


/* --------------------------------------------------
                      stxt_init

     initialisiert die Uebertragung zum Display-
     adapter
   -------------------------------------------------- */
void stxt_init(void)
{
  uart_init(9600);
  delay(5);
  stxt_putchar(' ');
  stxt_clrscr();
  delay(5);
}


/* --------------------------------------------------
                      stxt_putchar

    sendet ein Zeichen an den Textdisplayadapter
   -------------------------------------------------- */
void stxt_putchar(char ch)
{
  uart_putchar(ch);
  delay(3);
  if (ch == 0x0a)
  {
    if (wherey == 2)
    {
      delay(10);
    }
    else
    {
      wherey++;
    }
  }
  else
  {
    wherex++;
  }
}

/* --------------------------------------------------
                         gotoxy

     Postioniert die Textausgabestelle, an der das
     naechste Zeichen ausgegeben wird

     Uebergabe:
        x,y : Position der Ausgabestelle. 1,1 ent-
              spricht der linken oberen Ecke
   -------------------------------------------------- */
void gotoxy(uint8_t x, uint8_t y)
{
  stxt_putchar(0x81);             // 0x81 CMD, nachfolgendes Byte enthaelt x,y-Position
  stxt_putchar((y<<5) | x);
  wherex= x;
  wherey= y;
}

/* --------------------------------------------------
                 stxt_addch

     setzt ein im Textdisplayadapter vordefiniertes
     Userzeichen im Display ein

     Uebergabe:
        ch      : Indexnummer des Zeichen im Array
                  additional_chars des Adapters
        pos     : Ascii-Codenummer unter der das
                  Zeichen abrufbar ist

     Usage:

             stxt_addch(9, 1);         // dt. Umlaut a
             printf("Z%chler",1);
   -------------------------------------------------- */
void stxt_addch(uint8_t ch, uint8_t pos)
{
  stxt_putchar(0x83);
  stxt_putchar((ch << 3) | (pos & 0x07));
  delay(5);
}

/* --------------------------------------------------
                 stxt_senduserchar

     sendet ein Userzeichen bestehend aus 8 Bytes
     an ein an einem PFS154 angeschlossenes
     Textdisplay

     Uebergabe:
        nr      : Ascii-Codenummer, unter der das
                  Zeichen abrufbar ist
        *userch : Zeiger auf ein 8 Byte grosses
                  Array, das das Bitmap des User-
                  zeichens enthaelt.

     Usage:

             stxt_senduserchar(1, &charbitmap[0]);
             printf("Userzeichen 1: %c",1);
   -------------------------------------------------- */
void stxt_senduserchar(uint8_t nr, const uint8_t *userch)
{
  uint8_t i;

  stxt_putchar(0x84);             // Kommando fuer setuserchar
  stxt_putchar(nr);               // Ascii-Nummer unter der das Zeichen verfuegbar sein soll
  for (i= 0; i< 8; i++)
  {
    stxt_putchar(*userch);
    userch++;
  }
  delay(5);
}

void stxt_shiftleft(uint8_t anz)
{
  stxt_putchar(0x85);             // Kommando fuer Display shift left
  stxt_putchar(anz);
  delay(5);
}

void stxt_shiftright(uint8_t anz)
{
  stxt_putchar(0x86);             // Kommando fuer Display shift left
  stxt_putchar(anz);
  delay(5);
}

#if (enable_stxt_printf == 1)

  char printfkomma = 1;


  /* ------------------------------------------------------------
       stxt_putint

       gibt einen Integer dezimal aus. Ist Uebergabe
       "komma" != 0 wird ein "Kommapunkt" mit ausgegeben.

       Bsp.: 12345 wird, bei printfkomma == 2,  als 123.45
       ausgegeben.  (ermoeglicht Pseudofloatausgaben)
     ------------------------------------------------------------ */
  void stxt_putint(int16_t i, char komma)
  {
    typedef enum boolean { FALSE, TRUE }bool_t;

    static uint16_t zz[]      = { 10000, 1000, 100, 10 };
    bool_t          not_first = FALSE;

    uint8_t    zi;
    int16_t    z, b;

    komma= 5-komma;

    if (!i)
    {
      stxt_putchar('0');
    }
    else
    {
      if(i < 0)
      {
        stxt_putchar('-');
        i = -i;
      }

      for(zi = 0; zi < 4; zi++)
      {
        z = 0;
        b = 0;

        if  ((zi== komma) && komma)
        {
          if (!not_first) stxt_putchar('0');
          stxt_putchar('.');
          not_first= TRUE;
        }

        while(z + zz[zi] <= i)
        {
          b++;
          z += zz[zi];
        }
        if(b || not_first)
        {
          stxt_putchar('0' + b);
          not_first = TRUE;
        }
        i -= z;
      }
      if (komma== 4) stxt_putchar('.');
      stxt_putchar('0' + i);
    }
  }



  /* --------------------------------------------------
       hexnibbleout

       gibt die unteren 4 Bits eines chars als Hexa-
       ziffer aus. Eine Pruefung ob die oberen vier
       Bits geloescht sind erfolgt NICHT !
    --------------------------------------------------  */
  void hexnibbleout(uint8_t b)
  {
    if (b< 10) b+= '0'; else b+= 55;
    stxt_putchar(b);
  }

  /* --------------------------------------------------
        puthex

        gibt einen Integer hexadezimal aus. Ist die
        auszugebende Zahl >= 0xff erfolgt die Ausgabe
        2-stellig, ist sie groesser erfolgt die
        Ausgabe 4-stellig.
     -------------------------------------------------- */
  void puthex(uint16_t h)
  {
    uint8_t b;

    if (h> 0xff)                    // 16 Bit-Wert
    {
      b= (h >> 12);
      hexnibbleout(b);
      b= (h >> 8) & 0x0f;
      hexnibbleout(b);
    }
    b= h;
    b= (h >> 4) & 0x0f;
    hexnibbleout(b);
    b= h & 0x0f;
    hexnibbleout(b);
  }

  /* --------------------------------------------------
       stxt_putramstring

       gibt einen String aus dem RAM aus
     -------------------------------------------------- */
  void stxt_putramstring(char *p)
  {
    do
    {
      if (*p) stxt_putchar( *p );
    } while( *p++);
  }


  /* --------------------------------------------------
       stxt_printf

       alternativer Ersatz fuer printf.

       Aufruf:

           stxt_printf(PSTR("Ergebnis= %d"),zahl);

       oder durch define-Makro (besser):

           stxttiny_printf("Ergebnis= %d",zahl);

       Platzhalterfunktionen:

          %s     : Ausgabe Textstring
          %d     : dezimale Ausgabe
          %x     : hexadezimale Ausgabe
                   ist Wert > 0xff erfolgt 4-stellige
                   Ausgabe
                   is Wert <= 0xff erfolgt 2-stellige
                   Ausgabe
          %k     : Integerausgabe als Pseudokommazahl
                   12345 wird als 123.45 ausgegeben
          %c     : Ausgabe als Asciizeichen
          %p     : setze X-Position
          %P     : setze Y-Position
          %l     : loesche aktuelle Zeile und gehe zu x-Position 1
     -------------------------------------------------- */
  void stxt_printf(const char *s,...)
  {
    int       arg1;
    char      *arg2;
    char      ch;
    va_list   ap;
    uint8_t   i;

    va_start(ap,s);
    do
    {
      ch= *s;
      if(ch== 0) return;

      if(ch=='%')            // Platzhalterzeichen
      {
        s++;
        uint8_t token= *s;
        switch(token)
        {
          case 'd':          // dezimale Ausgabe
          {
            arg1= va_arg(ap,int);
            stxt_putint(arg1,0);
            break;
          }
          case 'x':          // hexadezimale Ausgabe
          {
            arg1= va_arg(ap,int);
            puthex(arg1);
            break;
          }
          case 'k':          // Integerausgabe mit Komma: 12896 zeigt 128.96 an
          {
            arg1= va_arg(ap,int);
            stxt_putint(arg1, printfkomma);
            break;
          }
          case 'c':          // Zeichenausgabe
          {
            arg1= va_arg(ap,int);
            stxt_putchar(arg1);
            break;
          }
          case '%':
          {
            stxt_putchar(token);
            break;
          }
          case 's':
          {
            arg2= va_arg(ap,char *);
            stxt_putramstring(arg2);
            break;
          }
          case 'p':          // gehe zu x-Position
          {
            arg1= va_arg(ap,int);
            gotoxy(arg1, wherey);
            break;
          }
          case 'P':          // gehe zu y-Position
          {
            arg1= va_arg(ap,int);
            gotoxy(wherex, arg1);
            break;
          }
          case 'l' :
          {
            gotoxy(1, wherey);
            for (i= 0; i< 16; i++) { stxt_putchar(' '); }
            gotoxy(1, wherey);
          }
        }
      }
      else
      {
        stxt_putchar(ch);
      }
      s++;
    } while (ch != '\0');
  }

#endif
