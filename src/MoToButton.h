/* MoTobutton
 *  manage 8, 16 or 32 Buttons with debouncing, edge detection
 *  and long/short press detection
 *  
 * Readiing the hardeware state of the buttons is done by a usercallback. 
 * This enables designes where the buttons/switches are arranged in a matrix and/or read via an port extender
 * The return value of his function has to be a byte, uint or ulong according to the definition of button_t
 * below. Every button/switch is represented by one bit, where '1' means the button is pressed.
 */
#ifndef MOTOBUTTON_H
#define MOTOBUTTON_H
#include <Arduino.h>
//typedef uint8_t button_t;       // manage up to 8 buttons
typedef uint16_t button_t;       // manage up to 16 buttons
//typedef uint32_t button_t;       // manage up to 32 buttons

class MoToButton {
  public:
  // Construktor parameter:
  // getHwButtons       Adress of the userfuction the reads the state of the buttons
  // debTime            Debouncetime in ms
  // pressTime          (in ms ) if the button is pressed longer, it is a 'long press'
  //                     max presstime = debTime*255
  MoToButton( button_t (*getHWbuttons)(), uint8_t debTime, uint16_t pressTime );
  
  
  void    processButtons();                 // must be called in the loop frequently
                                            // if its called less frequently than debTime, pressTime will be inaccurate
  boolean state( uint8_t buttonNbr );       // get static state of button (debounced)
  boolean shortPress( uint8_t buttonNbr );  // true if button was pressed short ( set when button is released, reset after call )  
  boolean longPress( uint8_t buttonNbr );   // true if button was pressed long ( set when button is released, reset after call )  
  boolean pressed( uint8_t buttonNbr );     // true if button is pressed ( reset after call )
  boolean released( uint8_t buttonNbr );    // true if button is released ( reset after call )


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
