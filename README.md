# MoToButton

- MoToButton is now part of the MobaTools Library
 
A library to manage multiple buttons and switches. Up to 8/16/32 Buttons can be controlled in one instance.

The buttons/switches are read in via a user callback function. This enables matrix arrangements and e.g. I2C port expander to be used.

### Functionality:

- Debounce

- Request status of a button/switch

- Events: Pressed, released, pressed long and pressed short
  
#### Overview
 Default is managing up to 16 buttons/switches.
 The default can be changed to save RAM (up to 8 buttons) or to manage up to 32 buttons (with additional RAM consumption). 
 This can be achieved by inserting '#define MAX32BUTTONS' or '#define MAX8BUTTONS'  before the #include <MoToButtons.h>.
  
Reading the hardware state of the buttons is done by a usercallback function. 
This enables designs where the buttons/switches are arranged in a matrix and/or read via a port extender.
The return value of this function has to be a 8-Bit, 16-Bit or 32-Bit value according to the maximum manageable 
number of buttons. Every button/switch is represented by one bit, where '1' means the button is pressed.
'button_t' is automtically set to the correct type and can be used to define the type of the callback function.
