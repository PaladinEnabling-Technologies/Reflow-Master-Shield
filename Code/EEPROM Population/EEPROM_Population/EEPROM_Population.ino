    /******************************************************************************************************************************************************************************
   
    Title:    Solder Reflow Oven Controller
    Version:  1.1
    Date:     2014.09.27
    Company:  Paladin Enabling Techologies
    Author:   Dirk Steyn
    Website:  www.PaladinTech.Wordpress.com
    
    Notes
    ******
    This sketch is used to populate the EEPROM that the solder reflow oven will read upon startup. If this sketch is not run then all the values will be set to zero. By running
    this sketch the values are set to the default values and then you won't have to wait a while for the values to change by programming it via the buttons on of the reflow oven.
                        
    ******************************************************************************************************************************************************************************/  
    
    #include <avr/eeprom.h> 
  
    int led = 13;
   
    void setup() 
    {       
       pinMode(led, OUTPUT);          // initialize the digital pin as an output.
      
       int Stored_Values [12] =       //  Declaring default values to be stored into the EEPROM.
       { 
         150,                         //  PREH Default Temperature
         200,                         //  SOAK Default Temperature
         245,                         //  RFLW Default Temperature
         180,                         //  KEEP Default Temperature
         150,                         //  COOL Default Temperature
          30,                         //  PREH Keep Time
          20,                         //  SOAK Keep Time
          10,                         //  RFLW Keep Time
           0,                         //  PREH Hysteresis Value
          10,                         //  SOAK Hysteresis Value
          14,                         //  RFLW Hysteresis Value
           0                          //  Fan  Status - 1 = ON; 0 = OFF
       };        
       
       eeprom_write_block((const void*)&Stored_Values, (void*)0, sizeof(Stored_Values));    // Store storage array values into EEPROM
    }


    /* 
   
    By the time we reach the void loop, the values are already stored. We flicker LED13 to indicate that the 
    process is completed and that you can now load the main Solder Reflow Oven Controller Sketch onto the board.  
    
    */
        
    void loop()                       
    {
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(200);                // wait for a second
      digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
      delay(200);                // wait for a second
    }
    
    
    
