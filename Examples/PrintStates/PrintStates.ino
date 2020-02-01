// Show Events of buttons im serial monitor

#define BUTTON_CNT 4
#include <MoToButton.h>
// define pin numbers
const byte buttonPin [BUTTON_CNT] = { A0, A1, A2, A3 };
char txtBuf[50];

button_t getHW( void ) {
  // raw reading of buttons
  button_t buttonTemp = 0;
  for (byte i = 0; i < BUTTON_CNT; i++) {
    bitWrite( buttonTemp,i,!digitalRead(buttonPin[i]) ); 
  }
  return buttonTemp;
}

MoToButton Buttons( getHW, 20, 500 );

void setup()
{
  Serial.begin(115200);
  while(!Serial);       // only for Leonardo/Micro ( mega32u4 based boards 
  
  for (int i = 0; i < BUTTON_CNT; i++)  {    
    // buttons must switch to Gnc
    pinMode(buttonPin[i], INPUT_PULLUP); 
  }
  Serial.println("Starting loop");
  sprintf( txtBuf, "max. managable buttons: %d", sizeof(button_t)*8 );
  Serial.println( txtBuf );
}

void loop() {
  //--------------------------------------------------------
  // read and process buttons
  Buttons.processButtons();
  // 
  //--------------------------------------------------------
  // print state of buttons if at least one changed
  if ( Buttons.changed() ) {
    sprintf( txtBuf, "State: %d %d %d %d - ", Buttons.state(0), Buttons.state(1), Buttons.state(2), Buttons.state(3) );
    Serial.print( txtBuf ); Serial.println( Buttons.allStates(),BIN );
  }
  
  // print to serial monitor if an event happens ( pressing or releasing )
  for ( byte btnNbr = 0; btnNbr < BUTTON_CNT; btnNbr++) {
    if ( Buttons.pressed(btnNbr) ) {
      sprintf( txtBuf, "button %d pressed", btnNbr );
      Serial.println(txtBuf);
    }
    if ( Buttons.released(btnNbr) ) {
      sprintf( txtBuf, "button %d released", btnNbr );
      Serial.println(txtBuf);
    }
    if ( Buttons.longPress(btnNbr) ) {
      sprintf( txtBuf, "button %d pressed long", btnNbr );
      Serial.println(txtBuf);
    }
    if ( Buttons.shortPress(btnNbr) ) {
      sprintf( txtBuf, "button %d pressed short", btnNbr );
      Serial.println(txtBuf);
    }
    //delay(500);

  }

}
