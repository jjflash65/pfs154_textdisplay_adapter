/* ---------------------------------------------------------
                            stxlcd.c

     Softwaremodul zum Ansprechen eines ueber einen
     Padauk PFS154 Controller gesteuerten Textdisplays
     mittels einer single-wire Verbindung

     Datenleitung: PD2 (in stxlcd.h aenderbar)

     MCU      : AVR
     Compiler : avr-gcc

     zusaetzliche Hardware:
        PFS154 - Textdisplay

     19.10.2024   R. Seelig
   --------------------------------------------------------- */

#include "stxlcd.h"

uint8_t wherex, wherey;

/* --------------------------------------------------
                     stxt_sendval

     sendet einen Wert an ein an einen PFS154 ange-
     schlossenes Textdisplay

     Uebergabe:
        val  : Integervariable, die Steuer- und
               datenbits enthalten. Es werden
               die niederwertigen 12 Bits seriell auf
               dem Datenpin gesendet.

               Protokoll siehe in stxlcd.recv.c
   -------------------------------------------------- */
void stxt_sendval(uint16_t val)
{
  uint8_t  i;
  uint16_t mask;

  stx_clr();
  _delay_us(200);

  mask= 0x800;
  for (i= 0; i< 12; i++)
  {

    if (val & mask)
    {
      stx_set();
      _delay_us(pulselength);
      stx_clr();
      _delay_us(pulsepause);
    }
    else
    {
      stx_set();
      _delay_us(pulsepause);
      stx_clr();
      _delay_us(pulselength);
    }
    mask= mask >> 1;
  }
  stx_set();
  _delay_ms(3);
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
  uint8_t  i, cx;
  uint8_t  ch;
  uint8_t  mask;

  stxt_sendval(0x0400 | nr);

  for (cx= 0; cx<8; cx++)
  {
    ch= *userch;
    userch++;
    stx_set();
    _delay_us(500);
    stx_clr();
    _delay_us(200);

    mask= 0x80;
    for (i= 0; i< 8; i++)
    {

      if (ch & mask)
      {
        stx_set();
        _delay_us(pulselength);
        stx_clr();
        _delay_us(pulsepause);
      }
      else
      {
        stx_set();
        _delay_us(pulsepause);
        stx_clr();
        _delay_us(pulselength);
      }
      mask= mask >> 1;
    }
  }
  stx_set();
  delay(10);

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
  uint16_t val;

  val= (y << 5) | x | 0x0100;
  stxt_sendval(val);
  delay(1);
  wherex= x;
  wherey= y;
}

void stxt_putchar(uint8_t ch)
{
  stxt_sendval(ch);
  _delay_us(2000);
  if (ch == 0x0a)
  {
    if (wherey == 2)
    {
      _delay_ms(15);
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
  void stxt_putramstring(uint8_t *p)
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
  void stxt_printf(const uint8_t *s,...)
  {
    int       arg1;
    uint8_t   *arg2;
    char      ch;
    va_list   ap;
    uint8_t   i;

    va_start(ap,s);
    do
    {
      ch= pgm_read_byte(s);
      if(ch== 0) return;

      if(ch=='%')            // Platzhalterzeichen
      {
        uint8_t token= pgm_read_byte(++s);
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
