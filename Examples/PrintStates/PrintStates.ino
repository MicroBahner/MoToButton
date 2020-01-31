// Show Events of buttons im serial monitor

#define BUTTON_CNT 4
#include <MoToButton.h>
// define pin numbers
const byte buttonPin [BUTTON_CNT] = { A0, A1, A2, A3 };
char txtBuf[50];
uint32_t lastPrintTime = 0;
button_t getHW( void ) {
  // raw reading of buttons
  button_t buttonTemp = 0;
  for (byte i = 0; i < BUTTON_CNT; i++) {
    bitWrite( buttonTemp,i,!digitalRead(buttonPin[i]) ); 
  }
  return buttonTemp;
}

MoToButton button1( getHW, 20, 500 );

void setup()
{
  Serial.begin(115200);
  while(!Serial);       // only for Leonardo/Micro ( mega32u4 based boards 
  
  for (int i = 0; i < BUTTON_CNT; i++)  {    
    // buttons must switch to Gnc
    pinMode(buttonPin[i], INPUT_PULLUP); 
  }
  Serial.println("Starting loop");
}

void loop() {
  //--------------------------------------------------------
  // read and process buttons
  button1.processButtons();
  // 
  //--------------------------------------------------------
  // print state of buttons every second
  if ( millis()-lastPrintTime > 1000 ) {
    lastPrintTime = millis();
    sprintf( txtBuf, "State: %d %d %d %d", button1.state(0), button1.state(1), button1.state(2), button1.state(3) );
    Serial.println( txtBuf );
  }
  
  // print to serial monitor if an event happens ( pressing or releasing )
  for ( byte btnNbr = 0; btnNbr < BUTTON_CNT; btnNbr++) {
    if ( button1.pressed(btnNbr) ){
      Serial.print(btnNbr); Serial.println(" pressed");
    }
    if ( button1.pressed(btnNbr) ) {
      Serial.println(btnNbr);
      sprintf( txtBuf, "button %d pressed", btnNbr );
      Serial.println(txtBuf);
    }
    if ( button1.released(btnNbr) ) {
      sprintf( txtBuf, "button %d released", btnNbr );
      Serial.println(txtBuf);
    }
    if ( button1.longPress(btnNbr) ) {
      sprintf( txtBuf, "button %d pressed long", btnNbr );
      Serial.println(txtBuf);
    }
    if ( button1.shortPress(btnNbr) ) {
      sprintf( txtBuf, "button %d pressed short", btnNbr );
      Serial.println(txtBuf);
    }
    //delay(500);

  }

}
