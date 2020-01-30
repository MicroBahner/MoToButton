# MoToButton
 
A library to manage multiple buttons and switches. Up to 8/16/32 Buttons can be controlled in one instance.

The buttons/switches are read in via a user callback function. This enables matrix arrangements and e.g. I2C port expander to be used.

### Functionality:

- Debounce

- Request status of a button/switch

- Events: Pressed, released, pressed long and pressed short
 