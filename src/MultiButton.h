/* Multibutton
 *  manage 8, 16 or 32 Buttons with debouncing, edge detection
 *  and long/short press detection
 *  
 */
#ifndef MULTIBUTTON_H
#define MULTIBUTTON_H
#include <Arduino.h>
typedef uint8_t button_t;       // manage up to 8 buttons
//typedef uint16_t button_t;       // manage up to 16 buttons
//typedef uint32_t button_t;       // manage up to 32 buttons

class MultiButton {
  public:
  MultiButton( button_t (*getHWbuttons)(), uint8_t debTime, uint16_t pressTime );
    // max presstime = debTime*255
  void    processButtons();               // must be called in loop frequently
  boolean state( uint8_t buttonNbr );     // get static state of button (debounced)
  boolean shortPress( uint8_t buttonNbr );     // if button was pressed short  
  boolean longPress( uint8_t buttonNbr );      // if button was pressed long
  boolean pressed( uint8_t buttonNbr );   // leading edge of button press
  boolean released( uint8_t buttonNbr );   // trailing edge of button press


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
