Readme
:
*******


1. Congratulations if you've purchased an Paladin Enabling Technologies solder reflow shield / controller, 
   if you decide to make it yourself, goodluck.

2. Before we can start reflowing your boards you will have to first load the EEPROM Population sketch onto the board,
   this is to move the default values into the EEPROM and to save you some time programming it in by hand.

3. If your not using an Arduino UNO but an Arduino Mega, then you will have to copy a replacement config file
   into your openGLCD library folder so that the shield will work.

4. The file is found in the "Master Config File - Same Pinout for all boards" folder and must be copied to the
   "C:\Program Files\Arduino\libraries\openGLCD" folder if this is your installation path of your Arduino IDE.

5. You can replace this file with the original master config file found in the "Master Config File - Original"
   folder if your not going to use the shield anymore and would like to use the default pinout for the Arduino Mega,

6. Next we can load the main Solder Reflow Oven Coding Sketch onto your board. There are two versions available. One for
   the UNO and one for the MEGA. 

7. Please note that the default values are for the toaster oven that we have used,
   you might have to adjust it for the oven your using as well as for the solder paste.

8. Please note that when you're in a submenu, you can navigate through it by pressing the ENT button.
   You can't escape so you must navigate through the menu. (You will notice that the "ESC" will disappear if you need to 
   navigate through a submenu)

9. Please read through the additional documentation in the sketches.