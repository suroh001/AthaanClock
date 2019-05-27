// ============================================================
// Project:     Athaan Clock
// ============================================================
// Author:      Habeeb Surooprajally
// Project start date: 25.05.2019
// ============================================================
// Description:
// The final result will be a highly configurable device which  
// will play the islamic call to prayer at the appropriate time
// day.
// ============================================================

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

//U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, 13, 11, 10, 8);
U8G2_ST7920_128X64_F_8080 u8g2(U8G2_R0, 22, 3, 4, 5, 6, 7, 8, 9, 13, U8X8_PIN_NONE, 12, 11);

volatile boolean TurnDetected;
volatile boolean up;

const int pinA = 2; // Used for generating interrupts using CLK signal
const int pinB = 24; // Used for reading DT signal
const int PinSW = 26; // Used for the push button switch

//U8X8_PIN_NONE
/*
void setup(void) {
  u8g2.begin();
}

void loop(void) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,24,"Hello World!");
  } while ( u8g2.nextPage() );
}
*/

void isr0()
{
  up = (digitalRead(pinA) == digitalRead(pinB));
  TurnDetected = true;
}

void setup(void)
{
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(PinSW, INPUT);
  attachInterrupt(0, isr0, CHANGE); // interrupt 0 is always connected to pin 2 on Arduino UNO
  Serial.begin(9600);
  Serial.println("Start");
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.begin(/*Select=*/ 26, /*Right/Next=*/ 2, /*Left/Prev=*/ 24, /*Up=*/ 24, /*Down=*/ 2, /*Home/Cancel=*/ A6); 
  u8g2.begin();
}

void loop(void)
{
  static long virtualPosition = 0; // without STATIC it does not count correctly!!!

  if (!(digitalRead(PinSW)) && (virtualPosition != 0))
  {                           // check if pushbutton is pressed
    virtualPosition = 0;      // if YES, then reset counter to ZERO
    Serial.print("Reset = "); // Using the word RESET instead of COUNT here to find out a buggy encoder

    Serial.println(virtualPosition);
  }

  if (TurnDetected)
  { // do this only if rotation was detected
    if (up)
      virtualPosition++;
    else
      virtualPosition--;
    TurnDetected = false; // do NOT repeat IF loop until new rotation detected
    Serial.print("Count = ");
    Serial.println(virtualPosition);
  }

  int integerPosition = int(virtualPosition);
  enum
  {
    BufSize = 6
  }; // If a is short use a smaller number, eg 5 or 6
  char positionString[BufSize];
  snprintf(positionString, BufSize, "%d", integerPosition);

    current_selection = u8g2.userInterfaceSelectionList(
    "Cloud Types",
    current_selection, 
    string_list);

  if ( current_selection == 0 ) {
    u8g2.userInterfaceMessage(
	"Nothing selected.", 
	"",
	"",
	" ok ");
  } else {
    u8g2.userInterfaceMessage(
	"Selection:", 
	u8x8_GetStringLineStart(current_selection-1, string_list ),
	"",
	" ok \n cancel ");
  }

}



const char *string_list = 
  "Altocumulus\n"
  "Altostratus\n"
  "Cirrocumulus\n"
  "Cirrostratus\n"
  "Cirrus\n"
  "Cumulonimbus\n"
  "Cumulus\n"
  "Nimbostratus\n"
  "Stratocumulus\n"
  "Stratus";

uint8_t current_selection = 0;




