/* MoTobutton
 *  manage 8, 16 or 32 Buttons with debouncing, edge detection
 *  and long/short press detection
 *  
 * Readiing the hardeware state of the buttons is done by a usercallback. 
 * This enables designes where the buttons/switches are arranged in a matrix and/or read via an port extender
 * The return value of his function has to be a byte, uint or ulong according to the definition of button_t
 * below. Every button/switch is represented by one bit, where '1' means the button is pressed.

  // Construktor parameter:
  // getHwButtons       Adress of the userfuction the reads the state of the buttons
  // debTime            Debouncetime in ms
  // pressTime          (in ms ) if the button is pressed longer, it is a 'long press'
  //                     max presstime = debTime*255
  MoToButton( button_t (*getHWbuttons)(), uint8_t debTime, uint16_t pressTime );
  
  
  void    processButtons();                 // must be called in the loop frequently
                                            // if its called less then debTime, pressTime will be inaccurate
  boolean state( uint8_t buttonNbr );       // get static state of button (debounced)
  boolean shortPress( uint8_t buttonNbr );  // true if button was pressed short ( set when button is released, reset after call )  
  boolean longPress( uint8_t buttonNbr );   // true if button was pressed long ( set when button is released, reset after call )  
  boolean pressed( uint8_t buttonNbr );     // true if button is pressed ( reset after call )
  boolean released( uint8_t buttonNbr );    // true if button is released ( reset after call )


 */
#ifndef MOTOBUTTON_H
#define MOTOBUTTON_H

#include <Arduino.h>

#ifndef BUTTONCNT
#define BUTTONCNT 16
#endif

#if BUTTONCNT>32
#error "too much buttons"
#elif BUTTONCNT>16
typedef uint32_t button_t;
#warning "button_t = uint32_t!"
#elif BUTTONCNT>8
typedef uint16_t button_t;
#warning "button_t = uint16_t!"
#else
typedef uint8_t button_t;
#warning "button_t = uint8_t!"
#endif

class MoToButton {
  public:
    MoToButton( button_t (*getHWbuttons)(), uint8_t debTime, uint16_t pressTime ) {
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

    void processButtons() {
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

    boolean state( uint8_t buttonNbr ) {
      if ( buttonNbr >= _buttonCnt ) return 0;
      // get static state of button (debounced)
      return bitRead( _actState, buttonNbr );
    }

    boolean shortPress( uint8_t buttonNbr ) {
      // if button was pressed short
      if ( buttonNbr >= _buttonCnt ) return 0;
      // get short pressed state of button (debounced)
      boolean temp = bitRead( _shortPress, buttonNbr );
      bitClear( _shortPress, buttonNbr );
      return temp;
    }
    boolean longPress( uint8_t buttonNbr ) {
      // if button was pressed long
      if ( buttonNbr >= _buttonCnt ) return 0;
      // get short pressed state of button (debounced)
      boolean temp = bitRead( _longPress, buttonNbr );
      bitClear( _longPress, buttonNbr );
      return temp;
    }

    boolean pressed( uint8_t buttonNbr ) {
      // leading edge of button press
      if ( buttonNbr >= _buttonCnt ) return 0;
      // get momentarily pressed state of button (debounced)
      boolean temp = bitRead( _leadingEdge, buttonNbr );
      bitClear( _leadingEdge, buttonNbr );
      return temp;
    }
    boolean released( uint8_t buttonNbr ) {  // trailing edge of button press
      // leading edge of button press
      if ( buttonNbr >= _buttonCnt ) return 0;
      // get momentarily released state of button (debounced)
      boolean temp = bitRead( _trailingEdge, buttonNbr );
      bitClear( _trailingEdge, buttonNbr );
      return temp;
    }

    private:
    uint8_t _debTime;            // Debounce time im ms
    uint8_t _pressTime;          // pressTime measured in debounce tics
    uint32_t _lastReadTime;     // Last time HW state was read
    static const uint8_t   _buttonCnt = sizeof(button_t)*8;        // Number of buttons
    button_t  (*_getHWbuttons)();  // Ptr to user function to read raw state of buttons
    // Bit fields to hold various button states
    button_t  _lastState;
    button_t  _actState;
    button_t  _longPress;
    button_t  _shortPress;
    button_t  _leadingEdge;
    button_t  _trailingEdge;
    uint8_t   _buttonTime[ _buttonCnt ]; // Time in debounce tics

  

};


#endif
