/*--------------------------------------------------------
                        stxlcd_recv.c

     Proprietaerer Textdisplayadapter fuer HD44780
     kompatible Displays. Der Adapter wird ueber eine
     einzelne Leitung (single-wire) angesteuert

     Compiler  : SDCC 4.0.3
     MCU       : PFS154

                 +----------------+
             PB4 | 1   |___|   16 | PB3
             PB5 | 2           15 | PB2
             PB6 | 3           14 | PB1
             PB7 | 4  PFS154   13 | PB0
             VDD | 5           12 | GND
             PA7 | 6           11 | PA0
     ICPDA / PA6 | 7           10 | PA4
       Vpp / PA5 | 8            9 | PA3 / ICPCK
                 +----------------+


     Protokoll:            [--- insgesamt 12 Datenbits ---]

     ----|                 |----------|           ~~~~    |-----------
     ....|_________________|          |__________ ~~~~____|

         |                                                |
    Start (lo. max. 2 ms)   Hi fuer mehr                Stop
                            als 40us                    Datenframe
                            entspricht
                            logisch 1,
                            kuerzer 40us
                            entspricht
                            logisch 0

    Datentransfer erfolgt mit MSB zuerst.

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

                         Datenleitung (single-wire) PA4

        ##########################################################################

        -------------------------------------------------
                   Steuerung des Adapters
        -------------------------------------------------

        Der Adapter erwartet staendig ein serielles 12-Telegramm in mit dem oben
        aufgefuehrten Protokoll

        Bit  |  11  |  10  |   9  |   8  ||  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
        -----+------+------+------+------++-----+-----+-----+-----+-----+-----+-----+-----+
             |     Command-nibble        ||                    Value                      |


        Kommando (hoeherwertigen 4 Bits) ||                 Bedeutung
        ---------------------------------++-----------------------------------------------
                                         ||
                       0                 || gibt ein Zeichen an der aktuellen Position aus
                                         ||
        ---------------------------------++-----------------------------------------------
                                         ||
                       1                 || setzt die Position fuer das naechste auszu-
                                         || gebende Zeichen (gotoxy).
                                         || Bit 5:4 => y-Position
                                         || Bit 3:0 => x-Postion
                                         || Position 1,1 ist links oben
                                         ||
        ---------------------------------++-----------------------------------------------
                                         ||
                       2                 || loescht den Displayinhalt durch erneutes
                                         || initialisieren
                                         ||
        ---------------------------------++-----------------------------------------------
                                         ||
                       3                 || set additional char
                                         || setzt ein vorgefertigtes, hier in einem Array
                                         || definiertes Benutzerzeichen ein ( Bitmap siehe
                                         || userchars.txt )
                                         ||
                                         || Bit 2:0 => Position des Zeichens, unter der
                                         ||            dieses abgerufen werden kann
                                         || Bit 7:3 => Nummer des vordefinierten Zeichens
                                         ||            siehe Array < additional_char >
                                         ||
        ---------------------------------++-----------------------------------------------
                                         ||
                       4                 || set user char
                                         || setzt ein im Anschluss an dieses Kommando
                                         || gesendetes Benutzerzeichen ein
                                         ||
                                         || Bit 2:0 => Position des Zeichens, unter der
                                         ||            das Benutzerzeichen abgerufen
                                         ||            werden kann
                                         || Dieses Kommando erwartet 8 Bytes auf der
                                         || Datenleitung, die das Bitmap des Zeichens
                                         || enthalten
        ---------------------------------++-----------------------------------------------
                                         ||
                       5                 || Display shift left
                                         || verschiebt das Display nach links
                                         || Bit 7:0 => Anzahl der Zeichen um die der
                                         || Textinhalt nach links verschoben wird
        ---------------------------------++-----------------------------------------------
                                         ||
                       6                 || Display shift right
                                         || verschiebt das Display nach rechts
                                         || Bit 7:0 => Anzahl der Zeichen um die der
                                         || Textinhalt nach rechts verschoben wird
        ---------------------------------++-----------------------------------------------
                                         ||
                       7                 || set contrast
                                         || erlaubte Werte sind 0..255. 0 repraesentiert
                                         || 0V (max. Kontrast), 255 entspricht Vcc (min.
                                         || Kontrast)
        ---------------------------------++-----------------------------------------------


      17.10.2024        R. Seelig
  -------------------------------------------------------- */

#include <stdint.h>
#include "pdk_init.h"
#include "pfs1xx_gpio.h"
#include "delay.h"


  //   Defines und Makros
  // -------------------------------------------------------

  /* -------------------------------------------------------
       Anschlusspins
     ------------------------------------------------------- */

  //  Anschlusspin des seriellen Eingangs auf dem Displaydaten gesendet werden
  #define rx_init()      PA4_input_init()
  #define is_rx()        is_PA4()

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

  /* -------------------------------------------------------
       diverse Defines und Makros
     ------------------------------------------------------- */

  #define timeout         50                    // maximale Zeitlaenge * 10ms fuer ein Datenframe
  #define pulselength      4                    // Zeitlaenge * 10us welches eine 1 im Sendetelegramm
                                                // repraesentiert

  #define testbit(reg,pos) ((reg) & (1<<pos))   // testet an der Bitposition pos das Bit auf 1 oder 0

  // Werte fuer Kontrastspannung : 255 = max. Spannung (kein Kontrast)
  //                                 0 = min. Spannung (max. Kontrast)
  //                                20 = "guter Wert" fuer Display
  #define pwm_set(val)    (TM3B= val)


  //   Ende - Defines und Makros
  // -------------------------------------------------------


//   globale Variable
// ---------------------------------------------------------

uint8_t wherex, wherey;      // nimmt die aktuelle (unsichtbare) Cursorposition auf, an
                             // der das naechste Zeichen ausgegeben wird


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

#define linelength   16

volatile uint8_t linebuffer[linelength + 1];


/* ---------------------------------------------------------
     Prototypen
   --------------------------------------------------------- */

uint16_t stxlcd_getval(void);
uint8_t stxlcd_getuserchar(void);

void stxlcd_init(void);
void stxlcd_setuserchar(uint8_t nr, uint8_t *puffer);
void gotoxy(uint8_t x, uint8_t y);
void stxlcd_putchar(char ch);
void stxlcd_shift(uint8_t dir);
void stxlcd_cursor(uint8_t onof);
void stxlcd_clrscr(void);
void linebuf_clr(void);

void pwm_init(void);

// ----------------------------------------------------------
//     putstring

//     gibt einen Text aus, zu Testzwecken. Bei Bedarf
//     "entkommentieren"
//   ---------------------------------------------------------
/*
void putstring(char *p)
{
  while(*p)
  {
    stxlcd_putchar( *p );
    p++;
  }
}
*/

/* -----------------------------------------------------------------------------
                                          main
   ----------------------------------------------------------------------------- */
void main(void)
{
  uint16_t val;
  uint8_t  userchar_nr;
  uint8_t  addch_pos;      // Index auf das additional Char-Array
  uint8_t  txcmd;
  uint8_t  i;
  uint8_t  err;
  uint8_t  xp, yp;

  delay(50);
  pwm_init();

  stxlcd_init();
  linebuf_clr();
  rx_init();

  while(1)
  {
    val= stxlcd_getval();
    txcmd= val >> 8;

    switch (txcmd)
    {
      // putchar
      case 0 :
        {
          val= val & 0x00ff;
          if (val< 127)
          {
            switch(val)
            {
              // bs, backspace
              case 0x08 : gotoxy(wherex-1, wherey); break;

              // ht, horizontal tab, hier X...X...X...X...
              case 0x09 :
                {
                  xp= wherex;
                  if (xp < 13)
                  {
                    xp--;
                    xp= xp / 4;
                    xp++;
                    xp= xp * 4;
                    xp++;
                  }
                  gotoxy(xp, wherey);
                  break;
                }
              // lf, line feed
              case 0x0a :
              {
                if (wherey == 2)
                {
                  xp= wherex;
                  gotoxy(1,1);
                  for (i= 0; i < linelength; i++)
                  {
                    stxlcd_putchar(linebuffer[i]);
                  }
                  gotoxy(1,2);
                  linebuf_clr();
                  for (i= 0; i < linelength; i++)
                  {
                    stxlcd_putchar(' ');
                  }
                  gotoxy(xp, 2);
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

              default : stxlcd_putchar(val); break;
            }
          }
          break;
        }

      // gotoxy
      case 1 :
        {
          xp = val & 0x01f;
          yp = (val >> 5) & 0x007;
          gotoxy(xp,yp);
          break;
        }

      // clear
      case 2 :
        {
          stxlcd_clrscr();
          break;
        }

      // set additional char
      case 3 :
        {
          userchar_nr= val & 0x0007;
          addch_pos= (val & 0x00f8) >> 3;
          stxlcd_setuserchar(userchar_nr, &additional_char[addch_pos][0]);
          break;
        }

      // setuserchar
      case 4 :
        {
          userchar_nr= val & 0x00ff;
          err= stxlcd_getuserchar();

          stxlcd_setuserchar(userchar_nr, &userchar[0]);
          break;
        }

      // display shift left
      case 5 :
        {
          val &= 0x00ff;
          for (i= 0; i< val; i++)
          {
            stxlcd_shift(0);
            delay(4);
          }
          break;
        }
      // display shift right
      case 6 :
        {
          val &= 0x00ff;
          for (i= 0; i< val; i++)
          {
            stxlcd_shift(1);
            delay(4);
          }
          break;
        }
      // set contrast
      case 7 :
        {
          val &= 0xff;
          pwm_set(val);
          break;
        }
      default : break;
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
  // 0       00         00001            = 0x01
  // PWM-8   Prescale   divisior
  // f_pwmc = IHRC / 256 = 16MHz / 256 = 62.5 kHz
  // f_pwm  = f_pwm / Prescale / (divisor+1) = 31.25 / 1 / 2 =15.625
  // f_timer= sysclock / 64 / 5 = 25 kHz
  TM3S = 0x1f;
  // -----------------------------------------------

  // TM3B
  // 160 = 0xa0 Teile Hi, 255-160 Teile LO
  // 128 = 0x80 Teile Hi, 255-128 Teile LO
  TM3B = 30;
}


/* --------------------------------------------------------
                         stxlcd_getval

     liest auf dem Receive-Pin 12 Bits ein.

     Wird ein Datentransfer initiert (mit negativer Flanke)
     und es erfolgt innerhalb von 2 ms keine zu lesende
     Datenbits, erfolgt ein Timeout.

     Protokoll:             [--- insgesamt 12 Datenbits ---]

     ----|                  |----------|           ~~~~    |-----------
     ....|__________________|          |__________ ~~~~____|

         |                                                 |
    Start (lo. max. 500 us)  Hi fuer mehr                Stop
                             als 40us                    Datenframe
                             entspricht
                             logisch 1,
                             kuerzer 40us
                             entspricht
                             logisch 0

     Datentransfer erfolgt mit MSB Digit zuerst

   -------------------------------------------------------- */
uint16_t stxlcd_getval(void)
{
  uint8_t  i;
  uint16_t inval;
  uint16_t timocnt;

  while (is_rx());                              // auf Start Datenframe warten

  inval = 0;
  // 12 Datenbits einlesen
  for (i= 0; i< 12; i++ )
  {
    inval= inval << 1;
    timocnt= 0;
    while ((!is_rx()) && (timocnt < timeout))     // Startflanke detektieren
    {
      _delay_us(10);
      timocnt++;
    }
    if (timocnt== timeout) {inval= 254; goto timeout_err; }

    timocnt= 0;
    while ((is_rx()) && (timocnt < timeout))     // Pulsedauer erfassen
    {
      _delay_us(10);
      timocnt++;
    }

    if (timocnt== timeout) { inval= 255; goto timeout_err; }

    if (timocnt> pulselength)
    {
      inval |= 0x01;
    }
  }
  timeout_err:
  return inval;
}

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
         HILO= 1 => oberen 4 Bits werden gesendet
         HILO= 0 => untere 4 Bits werden gesendet
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
  wherex= 0; wherey= 0;
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
  stxlcd_io(0x40+(nr << 3));               // CG-Ram Adresse fuer eigenes Zeichen
  rs_set();
  for (b= 0; b< 8; b++)
  {
    stxlcd_io(*puffer);
    puffer++;
  }

  // nachfolgendes gotoxy ist ein "ToDo", wird dieses
  // nach einem Definieren eines neuen Benutzerzeichens
  // nicht gemacht, haengt sich das Display danach fuer
  // normale Zeichenausgabe auf.
  gotoxy(wherex, wherey);
}


/* --------------------------------------------------------
                         stxlcd_getuserchar

     liest auf dem Receive-Pin 8 x 8 Datenbits auf dem
     Receive-Pin ein und speichert diese in der globalen
     Variable < userchar >

     Wird ein Datentransfer initiert (mit negativer Flanke)
     und es erfolgt innerhalb von 2 ms keine zu lesende
     Datenbits, erfolgt ein Timeout.

     Protokoll:             [--- insgesamt 8 Datenbits  ---]

     ----|                  |----------|           ~~~~    |-----------
     ....|__________________|          |__________ ~~~~____|

         |                                                 |
    Start (lo. max. 500 us)  Hi fuer mehr                Stop
                             als 40us                    Datenframe
                             entspricht
                             logisch 1,
                             kuerzer 40us
                             entspricht
                             logisch 0

     Datentransfer erfolgt mit MSB Digit zuerst

   -------------------------------------------------------- */
uint8_t stxlcd_getuserchar(void)
{
  uint8_t  i,cx;
  uint8_t inval;
  uint16_t timocnt;

  _delay_us(1500);

  for (cx= 0; cx<8; cx++)
  {
    while (is_rx());                              // auf Start Datenframe warten

    inval = 0;
    // 8 Datenbits einlesen
    for (i= 0; i< 8; i++ )
    {
      inval= inval << 1;
      timocnt= 0;
      while ((!is_rx()) && (timocnt < timeout))     // Startflanke detektieren
      {
        _delay_us(10);
        timocnt++;
      }
      if (timocnt== timeout) {inval= 254; goto timeout_err; }

      timocnt= 0;
      while ((is_rx()) && (timocnt < timeout))     // Pulsedauer erfassen
      {
        _delay_us(10);
        timocnt++;
      }

      if (timocnt== timeout) { inval= 255; goto timeout_err; }

      if (timocnt> pulselength)
      {
        inval |= 0x01;
      }
      while(!is_rx());
    }
    userchar[cx]= inval;
  }
  return 0;
  timeout_err:
  return inval;
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

void stxlcd_cursor(uint8_t onof)
{
  uint8_t val;

  val= 0x0c | onof;
  rs_clr();
  stxlcd_io(val);
}

void stxlcd_clrscr(void)
{
//  stxlcd_init();

  rs_clr();
  stxlcd_io(1);

  linebuf_clr();

}
