/* MoTobutton
    manage 8, 16 or 32 Buttons with debouncing, edge detection
    and long/short press detection

*/


#include <MoToButton.h>

MoToButton::MoToButton( button_t (*getHWbuttons)(), uint8_t debTime, uint16_t pressTime ) {
  _getHWbuttons = getHWbuttons;
  _debTime = debTime;
  _pressTime = pressTime / debTime;   // in debTime tics
  _lastReadTime = 0;     // Last time HW state was read
  // Bit fields to hold various button states
  _lastState = 0;
  _actState = 0;
  _longPress = 0;
  _shortPress = 0;
  _leadingEdge = 0;
  _trailingEdge = 0;
  for ( byte i = 0; i < _buttonCnt; i++ ) {
    _buttonTime[ i ] = 0; // Time in debounce tics
  }
}

void    MoToButton::processButtons() {
  // must be called in loop frequently
  if ( millis() - _lastReadTime > (uint32_t) _debTime ) {
    _lastReadTime = millis();
    _actState = _getHWbuttons();    // read button states
    // edge detection
    // leading edge
    _leadingEdge = ~_lastState & _actState;
    // trailing edge
    _trailingEdge = _lastState & ~_actState ;

    _lastState = _actState;

    // process pressing time
    for ( byte i = 0; i < _buttonCnt; i++ ) {
      if ( bitRead( _actState, i ) ) {
        // button is still pressed, update time counter
        bitClear( _longPress, i );
        bitClear( _shortPress, i );
        if ( _buttonTime[i] < _pressTime ) _buttonTime[i]++;
      } else {
        // button was released, check if it was presssd long or short
        if ( _buttonTime[i] > 0 ) { // check only once after releasing
          if (_buttonTime[i] < _pressTime) bitSet( _shortPress, i );
          else bitSet( _longPress, i );
          _buttonTime[i] = 0;
        }
      }
    }
  }
}

boolean MoToButton::state( uint8_t buttonNbr ) {
  if ( buttonNbr >= _buttonCnt ) return 0;
  // get static state of button (debounced)
  return bitRead( _actState, buttonNbr );
}

boolean MoToButton::shortPress( uint8_t buttonNbr ) {
  // if button was pressed short
  if ( buttonNbr >= _buttonCnt ) return 0;
  // get short pressed state of button (debounced)
  boolean temp = bitRead( _shortPress, buttonNbr );
  bitClear( _shortPress, buttonNbr );
  return temp;
}
boolean MoToButton::longPress( uint8_t buttonNbr ) {
  // if button was pressed long
  if ( buttonNbr >= _buttonCnt ) return 0;
  // get short pressed state of button (debounced)
  boolean temp = bitRead( _longPress, buttonNbr );
  bitClear( _longPress, buttonNbr );
  return temp;
}
boolean MoToButton::pressed( uint8_t buttonNbr ) {
  // leading edge of button press
  if ( buttonNbr >= _buttonCnt ) return 0;
  // get momentarily pressed state of button (debounced)
  boolean temp = bitRead( _leadingEdge, buttonNbr );
  bitClear( _leadingEdge, buttonNbr );
  return temp;
}
boolean MoToButton::released( uint8_t buttonNbr ) {  // trailing edge of button press
  // leading edge of button press
  if ( buttonNbr >= _buttonCnt ) return 0;
  // get momentarily released state of button (debounced)
  boolean temp = bitRead( _trailingEdge, buttonNbr );
  bitClear( _trailingEdge, buttonNbr );
  return temp;
}
