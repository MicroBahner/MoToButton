# MoToButton
 
A library to manage many buttons and switches. Up to 8/16/32 Buttons can be controlled in one instance.

The buttons/switches are read in via a user callback function. This enables matrix arrangements and e.g. I2C port expanders to be used.

Functionality:
Request status of the button/switch
Events: Pressed, released, pressed long and pressed short
