PFS154-Textdisplayadapter
<br><br>
Aus 2 Anlaessen heraus entstand ein "intelligenter" Adapter für HD44780 
kompatible Textdisplay (wobei sich über "intelligent" trefflich streiten 
laesst).
<br>
1. Ein einfaches Displaymodul für das Steckbrett zu haben, das sich nur 
mit einer einzelnen Leitung steuern laesst (um den Kabelverhau deutlich zu 
verringern).
Natürlich kann man hier auch I2C-Displays verwenden, aber hier bedarf es 
dann wieder mehr oder weniger aufwaendige Software im steuernden 
Controller
<br>
und
<br>
2. etwas mit dem Ultra-Low-Cost Controller PFS154 zu realisieren. Preis 
bei lcsc.com, Stand 23.10.2024 für PFS154-8 bei Abnahme von 50 Stück 
5,76 Cent, für den PFS154-16 bei Abnahme von 50 Stueck 7,99 Cent
<br>
Den Adapter gibt es in 2 Versionen:
<br>
- Ansteuerung ueber eine einzelne UART-Leitung (TxD vom Master, RxD hier
beim Displayadapter. Das UART-Protokoll ist 8N1 / 9600 Bd
<br>
<br>
- Proprietaeres 12-Bit PWM Protokoll
<br>
<br>
Zur Ansteuerung des Displayadapters sind in den entsprechenden
Verzeichnissen Demoprogramme fuer die Mikrocontrollerfamilien STM8,
AVR, STM32F0 und PFS154 vorhanden (jeweils in PWM und in UART-Version,
PFS154 nur in PWM-Version)
<br>
<br>
Proprietaeres Protokoll
<br>
Der Displayadapter hat nur eine Eingangsdatenleitung auf der die 
Information über ein proprietaeres 12-Bit PWM Protokoll uebertragen wird. 
Der steuernde Microcontroller benoetigt also nur - wie bereits erwaehnt - 
eine einzelne Steuerleitung zum Ansprechen des Adapters.
<br>
Die "Intelligenz" bezieht sich darauf, dass dem Adapter keine 
Initialisierung und kein Beschreiben der Displayregister vorgegeben 
wird, dieses wird vom Adapter selbst erledigt.
<br>
Dem Adapter muss nur mitgeteilt werden, welches Zeichen ausgeben werden 
soll oder wohin die nächste Ausgabestelle positioniert wird.
<br>
Saemtliche Software ist unter Linux mit den Compilern AVR-GCC und SDCC, 
die Programme koennen mit einem einfachen Aufruf von < make > compiliert 
werden.
<br>
Hinweis: Ich habe kein Demoprogramm für einen Arduino gemacht, aber 
vielleicht mache ich das noch (vielleicht).
<br>
Eine PCB die das ganze auf einem Steckbrett nutzbar macht (und die 
Gerber-Files hierzu) werde ich demnaechst hinzufuegen.
<br>
Ein (unspektakulaeres) Video mit einem Arduino-Nano (allerdings mit
reiner C-Software) kann man hier sehen:
<br>
<br>
Um die Darstellung auf dem Display etwas schoener zu gestalten sind in
der Adaptersoftware Benutzerzeichen vordefiniert, die einfach per
Kommando abrufbar sind.
<br>
Grundsaetzlich: eine etwas ausfuehrlichere Beschreibung gibt es im 
PDF-File im Ordner "doku"
<br>
Viel Spass damit, JJFlash
<br>
