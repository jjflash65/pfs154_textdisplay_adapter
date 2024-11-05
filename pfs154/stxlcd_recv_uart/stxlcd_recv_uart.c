/* ---------------------------------------------------------
                           stx_uart.c

     Textdisplayadapter fuer HD44780 kompatible Displays.
     Der Adapter wird ueber eine einzelne Leitung UART-
     Leitung mit 9600 Bd angesteuer.


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

    -------------------------------------------------------
      Anschluss Pollin-Display C0802-04 an den Controller
    -------------------------------------------------------

           Display                 PFS154
     Funktion   PIN              PIN    Funktion

        GND      1 ------------
        +5V      2 ------------
     Kontrast    3 ------------
        RS       4 ------------  13     PB0
        R/W*     5 ------------  GND
     (Takt) E    6 ------------  14     PB1
        D4       7 ------------  15     PB2
        D5       8 ------------  16     PB3
        D6       9 ------------  1      PB4
        D7      10 ------------  2      PB5

     -------------------------------------------------------
       Anschluss 14 pol. Standarddisplay
     -------------------------------------------------------

            Display                 PFS154
      Funktion   PIN              PIN    Funktion

          GND     1 ------------
          +5V     2 ------------
       Kontrast   3 ------------
          RS      4 ------------  13     PB0
          R/W*    5 ------------  GND
      (Takt) E    6 ------------  14     PB1
          D0      7 ------------  n.c.
          D1      8 ------------  n.c.
          D2      9 ------------  n.c.
          D3     10 ------------  n.c.
          D4     11 ------------  15     PB2
          D5     12 ------------  16     PB3
          D6     13 ------------  1      PB4
          D7     14 ------------  2      PB5


       * R/W = read/write Anschluss des Displays. 1 liest,
               0 beschreibt das Display. Da das Display
               nur ausschliesslich beschrieben wird, ist
               dieser Pin auf GND zu legen

        ##########################################################################

                         Datenleitung (UART-RxD) PA4

        ##########################################################################

        -------------------------------------------------
                   Steuerung des Adapters
        -------------------------------------------------

        Byte 0: Kommandobyte             || Byte 1: Datenbyte
        ---------------------------------++-----------------------------------------------
                                         ||
                     0x00                || auszugebendes Ascii-Zeichen
           Ausgabe Ascii Zeichen         || Erlaubte Werte sind 0..127, da Bit 7 als
                                         || Kennung eines Kommandos dient. Somit sind die
                                         || Zeichen > 127 nicht erreichbar (meistens die
                                         || chinesischen oder japanischen Zeichen)
                                         ||
        ---------------------------------++-----------------------------------------------
                                         ||
                     0x01                || setzt die Position fuer das naechste auszu-
                    gotoxy               || gebende Zeichen.
                                         || Bit 5:4 => y-Position
                                         || Bit 3:0 => x-Postion
                                         || Position 1,1 ist links oben
                                         ||
        ---------------------------------++-----------------------------------------------
                                         ||
                     0x02                || dem Kommandobyte 0x02 folgt kein weiteres
                    clrscr               || Byte
                                         ||
        ---------------------------------++-----------------------------------------------
                                         ||
                     0x03                || setzt ein vorgefertigtes, hier in einem Array
             set additional char         || definiertes Benutzerzeichen ein ( Bitmap siehe
                                         || userchars.txt )
                                         ||
                                         || Bit 2:0 => Position des Zeichens, unter der
                                         ||            dieses abgerufen werden kann
                                         || Bit 7:3 => Nummer des vordefinierten Zeichens
                                         ||            siehe Array < additional_char >
                                         ||
        ---------------------------------++-----------------------------------------------
                                         ||
                     0x04                || setzt ein im Anschluss an dieses Kommando
                set user char            || gesendetes Benutzerzeichen ein
                                         ||
                                         || Bit 2:0 => Position des Zeichens, unter der
                                         ||            das Benutzerzeichen abgerufen
                                         ||            werden kann
                                         || Dieses Kommando erwartet 8 Bytes auf der
                                         || Datenleitung, die das Bitmap des Zeichens
                                         || enthalten
        ---------------------------------++-----------------------------------------------
                                         ||
                     0x05                || verschiebt das Display nach links
               display shift left        || Bit 7:0 => Anzahl der Zeichen um die der
                                         || Textinhalt nach links verschoben wird
        ---------------------------------++-----------------------------------------------
                                         ||
                     0x06                || verschiebt das Display nach rechts
               display shift right       || Bit 7:0 => Anzahl der Zeichen um die der
                                         || Textinhalt nach rechts verschoben wird
        ---------------------------------++-----------------------------------------------
                                         ||
                     0x07                || erlaubte Werte sind 0..255. 0 repraesentiert
                 set contrast            || 0V (max. Kontrast), 255 entspricht Vcc (min.
                                         || Kontrast)
        ---------------------------------++-----------------------------------------------



     19.10.2024   R. Seelig
   --------------------------------------------------------- */

#include <stdint.h>
#include "pdk_init.h"
#include "pfs1xx_gpio.h"
#include "delay.h"

#include "uart_2.h"

  //  Anschlusspins des Displays selbst
  #define rs_init()      PB0_output_init()           // command / data
  #define e_init()       PB1_output_init()           // clock

  #define d4_init()      PB2_output_init()
  #define d5_init()      PB3_output_init()
  #define d6_init()      PB4_output_init()
  #define d7_init()      PB5_output_init()

  #define rs_set()       PB0_set()
  #define rs_clr()       PB0_clr()

  #define e_set()        PB1_set()
  #define e_clr()        PB1_clr()

  #define d4_set()       PB2_set()
  #define d4_clr()       PB2_clr()

  #define d5_set()       PB3_set()
  #define d5_clr()       PB3_clr()

  #define d6_set()       PB4_set()
  #define d6_clr()       PB4_clr()

  #define d7_set()       PB5_set()
  #define d7_clr()       PB5_clr()


  // Anschlusspin der Kontrastspannung (PWM. zu filtern mit passivem Tiefpass)
  #define pwmout_init()   PB7_output_init()

  // Werte fuer Kontrastspannung : 255 = max. Spannung (kein Kontrast)
  //                                 0 = min. Spannung (max. Kontrast)
  //                                20 = "guter Wert" fuer Display
  #define pwm_set(val)    (TM3B= val)


/* ---------------------------------------------------------
     Prototypen
   --------------------------------------------------------- */

void stxlcd_init(void);
void stxlcd_setuserchar(uint8_t nr, uint8_t *puffer);
void gotoxy(uint8_t x, uint8_t y);
void stxlcd_putchar(char ch);
void stxlcd_shift(uint8_t dir);
void stxlcd_cursor(uint8_t onof);
void stxlcd_clrscr(void);
void linebuf_clr(void);

void pwm_init(void);


//   globale Variable
// ---------------------------------------------------------

volatile uint8_t wherex, wherey;      // nimmt die aktuelle (unsichtbare) Cursorposition auf, an
                                      // der das naechste Zeichen ausgegeben wird

#define linelength   16

volatile uint8_t linebuffer[linelength + 1];


//   festdeklarierte Zeichen, die als "UserChar" verwendet
//   werden koennen. Da im fuer die Addressierung des zu
//   verwendenden Zeichens im Kommando 5 Bit's zur Ver-
//   fuegung stehen, koennen hier max. 32 Zeichen definiert
//   werden
// ---------------------------------------------------------
const uint8_t additional_char[][8]  =
{
  { 0x0c, 0x12, 0x12, 0x0c, 0x00, 0x00, 0x00, 0x00 },   //  0 : hochgestelltes "o" (fuer Gradangaben)
  { 0x0e, 0x11, 0x11, 0x11, 0x0a, 0x0a, 0x1b, 0x00 },   //  1 : Ohmzeichen
  { 0x00, 0x00, 0x1f, 0x0a, 0x0a, 0x0a, 0x13, 0x00 },   //  2: Pi-Zeichen
  { 0x00, 0x09, 0x09, 0x0b, 0x0d, 0x08, 0x10, 0x00 },   //  3: Mikrozeichen
  { 0x01, 0x03, 0x07, 0x0f, 0x07, 0x03, 0x01, 0x00 },   //  4: Pfeil nach links
  { 0x08, 0x0c, 0x0e, 0x0f, 0x0e, 0x0c, 0x08, 0x00 },   //  5: Pfeil nach rechts
  { 0x11, 0x04, 0x0a, 0x11, 0x1f, 0x11, 0x11, 0x00 },   //  6: dt. Umlaut A
  { 0x11, 0x0e, 0x11, 0x11, 0x11, 0x11, 0x0e, 0x00 },   //  7: dt. Umlaut O
  { 0x11, 0x00, 0x11, 0x11, 0x11, 0x11, 0x0e, 0x00 },   //  8: dt. Umlaut U
  { 0x0a, 0x00, 0x0e, 0x01, 0x0f, 0x11, 0x0f, 0x00 },   //  9: dt. Umlaut a
  { 0x0a, 0x00, 0x0e, 0x11, 0x11, 0x11, 0x0e, 0x00 },   // 10: dt. Umlaut o
  { 0x0a, 0x00, 0x00, 0x11, 0x11, 0x11, 0x0e, 0x00 },   // 11: dt. Umlaut u
  { 0x00, 0x0e, 0x11, 0x16, 0x11, 0x16, 0x10, 0x00 },   // 12: dt. Umlaut ss
  { 0x00, 0x00, 0x0a, 0x1f, 0x1f, 0x0e, 0x04, 0x00 },   // 13: Herz
  { 0x00, 0x1f, 0x10, 0x08, 0x04, 0x08, 0x1f, 0x00 },   // 14: Sigma
  { 0x06, 0x01, 0x02, 0x07, 0x00, 0x00 ,0x00, 0x00 },   // 15: hoch 2

  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x1f },   // 16: Balken1
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f ,0x1f },   // 17: Balken2
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f ,0x1f },   // 18: Balken3
  { 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f ,0x1f },   // 19: Balken4
  { 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x1f ,0x1f },   // 20: Balken5
  { 0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f ,0x1f },   // 21: Balken6
  { 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f ,0x1f },   // 22: Balken7
  { 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f ,0x1f, 0x1f },   // 23: Balken7
};

uint8_t userchar[8];         // Pufferspeicher fuer benutzerdefinierbare Zeichen


/* -----------------------------------------------------------------------
                                     main
   ----------------------------------------------------------------------- */
int main(void)
{
  uint8_t  x,y;
  uint8_t  cmd;
  uint8_t  pos, ind;
  uint8_t  u;


  pwm_init();

  stxlcd_init();
  cmd= 0;
  while(1)
  {
    u= uart_getchar();

    if ((u & 0x80) && (cmd==0))      // Bit 7 zeigt Kommando an
    {
      cmd= (u & 0x7f);
      if (cmd == 2)                 // clrscr erwartet keinen weiteren Parameter
      {
        stxlcd_clrscr();
        cmd= 0;
      }
    }
    else
    {
      switch (cmd)
      {
        // Char auf das Display schreiben  (putchar)
        case 0 :
          {
            switch(u)
            {
              // bs, backspace
              case 0x08 : gotoxy(wherex-1, wherey); break;

              // ht, horizontal tab, hier X...X...X...X...
              case 0x09 :
                {
                  x= wherex;
                  if (x < 13)
                  {
                    x--;
                    x= x / 4;
                    x++;
                    x= x * 4;
                    x++;
                  }
                  gotoxy(x, wherey);
                  break;
                }
              // lf, line feed
              case 0x0a :
              {
                if (wherey == 2)
                {
                  x= wherex;
                  gotoxy(1,1);
                  for (ind= 0; ind < linelength; ind++)
                  {
                    stxlcd_putchar(linebuffer[ind]);
                  }
                  linebuf_clr();
                  gotoxy(1,2);
                  for (ind= 0; ind < linelength; ind++)
                  {
                    stxlcd_putchar(' ');
                  }
                  gotoxy(x, 2);
                }
                else
                {
                  gotoxy(wherex, wherey+1);
                }
                break;
              }
              // cr, carriage return
              case 0x0d : gotoxy(1, wherey); break;

              // so, hier Cursor an
              case 0x0e : stxlcd_cursor(3); break;

              // si, hier Cursor aus
              case 0x0f : stxlcd_cursor(0); break;

              default : stxlcd_putchar(u); break;
            }
            break;
          }
        // naechste Ausgabeposition festlegen (gotoxy)
        case 1 :
          {
            x= u & 0x1f;
            y= (u >> 5) & 0x07;
            gotoxy(x,y);
            break;
          }
        // set additional char
        case 3 :
          {

            pos= u & 0x07;
            ind= (u >> 3) & 0x1f;
            stxlcd_setuserchar(pos, &additional_char[ind][0]);
            delay_us(100);
            gotoxy(wherex, wherey);
            break;
          }
        // holen und setzen eines benutzerdefiniertes Zeichen,
        // erwartet weitere 8 Bytes
        case 4 :
          {

            pos= u & 0x07;
            for (x= 0; x< 8; x++)
            {
              u= uart_getchar();
              userchar[x]= u;
            }
            stxlcd_setuserchar(pos, &userchar[0]);
            delay_us(100);
            gotoxy(wherex, wherey);
            break;
          }
        // Displayinhalt nach links schieben
        case 5 :
          {
            for (x= 0; x< u; x++)
            {
              stxlcd_shift(0);
            }
            break;
          }
        // Displayinhalt nach links schieben
        case 6 :
          {
            for (x= 0; x< u; x++)
            {
              stxlcd_shift(1);
            }
            break;
          }
        // set contrast
        case 7 :
          {
            pwm_set(u);
            break;
          }
        default : break;
      }
      cmd= 0;
    }
  }

}

/* --------------------------------------------------------
                          pwm_init

     erzeugt eine PWM ueber Timer 3

     Beteiligte Register: TM3C (timer3 control register)
                          TM3C (timer3 counter register)
                          TM3S (timer3 scalar reigster)
   -------------------------------------------------------- */
void pwm_init(void)
{
  // TM3C
  // Bits 7-4  clock source    0010  =  IHRC
  // Bits 3-2  Timer output    11    =  PB7 / no invers
  // Bits 1    Mode select     1     =  PWM
  // Bits 0    invert pol.     0     =  no invert

  // == 0010.1110   = 0x2e
  TM3C = 0x2e;
  // -----------------------------------------------

  // TM3S
  // 0       00         0001            = 0x01
  // PWM-8   Prescale   divisior
  // f_pwmc = IHRC / 256 = 16MHz / 256 = 62.5 kHz
  // f_pwm  = f_pwm / Prescale / (divisor+1) = 31.25 / 1 / 2 =15.625
  // f_timer= sysclock / 64 / 5 = 25 kHz
  TM3S = 0x00;
  // -----------------------------------------------

  // TM3B
  // 160 = 0xa0 Teile Hi, 255-160 Teile LO
  // 128 = 0x80 Teile Hi, 255-128 Teile LO
  TM3B = 0x80;

  pwm_set(30);
}


#define testbit(reg,pos) ((reg) & (1<<pos))   // testet an der Bitposition pos das Bit auf 1 oder 0


/* -------------------------------------------------------
     linebuf_clr

     beschreibt den Zeilenpuffer mit Leerzeichen
   ------------------------------------------------------- */
void linebuf_clr(void)
{
  uint8_t i;

  for (i= 0; i< linelength; i++)
  {
    linebuffer[i]= ' ';            // Buffer mit Leerzeichen beschreiben
  }
  linebuffer[linelength]= 0;       // und Endekennung setzen
}

/* -------------------------------------------------------
     nibbleout

     sendet ein Halbbyte an das LC-Display

     Uebergabe:
         value  : gesamtes Byte
         nibble : 1 => HiByte wird gesendet
                  0 => LoByte wird gesendet
   ------------------------------------------------------- */
void nibbleout(uint8_t value, uint8_t nibble)
{
  if (nibble)
  {
    if (testbit(value, 7 )) d7_set(); else d7_clr();
    if (testbit(value, 6 )) d6_set(); else d6_clr();
    if (testbit(value, 5 )) d5_set(); else d5_clr();
    if (testbit(value, 4 )) d4_set(); else d4_clr();
  }
  else
  {
    if (testbit(value, 3 )) d7_set(); else d7_clr();
    if (testbit(value, 2 )) d6_set(); else d6_clr();
    if (testbit(value, 1 )) d5_set(); else d5_clr();
    if (testbit(value, 0 )) d4_set(); else d4_clr();
  }

}


/* -------------------------------------------------------
      stxlcd_clock

      gibt einen Clockimpuls an das Display
   ------------------------------------------------------- */
void stxlcd_clock(void)
{
  e_set();
  _delay_us(60);
  e_clr();
  _delay_us(60);
}

/* -------------------------------------------------------
      stxlcd_io

      sendet ein Byte an das Display

      Uebergabe:
         value = zu sendender Wert
   ------------------------------------------------------- */
void stxlcd_io(uint8_t value)
{
  nibbleout(value, 1);
  stxlcd_clock();
  nibbleout(value, 0);
  stxlcd_clock();
}

/* -------------------------------------------------------
     stxlcd_init

     initialisiert das Display im 4-Bitmodus
   ------------------------------------------------------- */
void stxlcd_init(void)
{
  char i;

  d4_init(); d5_init(); d6_init(); d7_init();
  rs_init(); e_init();

  rs_clr();
  for (i= 0; i< 3; i++)
  {
    stxlcd_io(0x20);
    _delay_ms(6);
  }
  stxlcd_io(0x28);
  _delay_ms(6);
  stxlcd_io(0x0c);
  _delay_ms(6);
  stxlcd_io(0x01);
  _delay_ms(6);
  wherex= 1; wherey= 1;

  linebuf_clr();
}

/* -------------------------------------------------------
     gotoxy

     setzt den Textcursor an eine Stelle im Display. Die
     obere linke Ecke hat die Koordinate (1,1)
   ------------------------------------------------------- */
void gotoxy(uint8_t x, uint8_t y)
{
  uint8_t stxlcd_adr;

  stxlcd_adr= (0x80+((y-1)*0x40))+x-1;
  rs_clr();
  stxlcd_io(stxlcd_adr);
  wherex= x;
  wherey= y;
}

/* -------------------------------------------------------
     stxlcd_setuserchar

     kopiert die Bitmap eines benutzerdefiniertes Zeichen
     welches im globalen Array <userchar> gespeichert ist
     in den Charactergenerator des Displaycontrollers

               nr : Position im RAM des Displays an
                    der die Bitmap hinterlegt werden
                    soll.
          *puffer : Zeiger auf einen 8 Byte grossen
                    Pufferspeicher, der das zu setzende
                    Bitmap des Userzeichens hat

   Bsp.:  stxlcd_setuserchar(3, &userchar[0]);
          stxlcd_putchar(3);

   ------------------------------------------------------- */
void stxlcd_setuserchar(uint8_t nr, uint8_t *puffer)
{
  uint8_t b;

  rs_clr();
  _delay_us(60);
  stxlcd_io(0x40+(nr << 3));               // CG-Ram Adresse fuer eigenes Zeichen
  rs_set();
  for (b= 0; b< 8; b++)
  {
    stxlcd_io(*puffer);
    puffer++;
  }
}



/* -------------------------------------------------------
     stxlcd_putchar

     gibt ein Zeichen auf dem Display aus

     Uebergabe:
         ch = auszugebendes Zeichen
   ------------------------------------------------------- */

void stxlcd_putchar(char ch)
{
  rs_set();
  stxlcd_io(ch);

  if (wherey == 2)
  {
    linebuffer[wherex-1]= ch;
  }
  wherex++;
}


/* -------------------------------------------------------
     stxlcd_shift

     verschiebt das sichtbare "Anzeigefenster" des
     Displays entweder um eine Position nach rechts oder
     links

     Uebergabe:
         dir = Verschieberichtung   0 : links
                                    1 : rechts
   ------------------------------------------------------- */
void stxlcd_shift(uint8_t dir)
{
  uint8_t val;

  val= 0x18 | (dir << 2);
  rs_clr();
  stxlcd_io(val);
}

/* -------------------------------------------------------
     stxlcd_cursor

     schaltet den Cursor an/aus

     Uebergabe:

        onoff  0 : Cursor aus
               1 : nicht erlaubt
               2 : Cursor als Strich
               3 : Cursor blinkend
   ------------------------------------------------------- */

void stxlcd_cursor(uint8_t onof)
{
  uint8_t val;

  val= 0x0c | onof;
  rs_clr();
  stxlcd_io(val);
}

void stxlcd_clrscr(void)
{
  stxlcd_init();

  rs_clr();
  stxlcd_io(1);

}
