  /******************************************************************************************************************************************************************************
  
  Title:    Reflow Master Shield - Solder Reflow Oven Controller
  Version:  1.1
  Date:     2014.09.27
  Company:  Paladin Enabling Techologies
  Author:   Dirk Steyn
  Website:  www.PaladinTech.Wordpress.com
  
  Notes
  ******
  This firmware was written for an open source solder reflow oven controller to match the commercial offerings at a much better offering. The default programmed curve is for lead-free 
  solder paste. The curve is completely configurable to the user's preference. This includes the stage temperatures as well as the holding times. The controller works as follows. A PREH 
  temperaure is the first setpoint. Once the setpoint is reached a countdown timer is started and the temperature is held at the first setpoint. Once the countdown timer reaches 0 min and 
  0 sec the next stage's setpoint is implemented. The next stage being the SOAK stage which is 200 Degrees Celcius. So the temperature is increased until the SOAK setpoint is reached. At 
  this stage the SOAK stage countdown timer is started. Once again when it reaches 0 min and 0 sec, the next stage is implemented. This being the RFLW stage. The setpoint is set to the 
  RFLW temperature being 245 Deg Celcius. Once the setpoint is reached the RFLW countdown timer starts until it reaches 0 min and 0 sec. Once this is reached the PID controller switches
  off and the oven cools down naturaly.
  
  There is also a KEEP setpoint and a COOL setpoint. The KEEP setpoint is set for 180 Degrees Celcius while the COOL setpoint is set for 150 Degrees Celcius. The controller has a build-in 
  secondary relay control circuit to operate an extraction fan to quickly cool from the KEEP setpoint to the COOL setpoint. Once the KEEP setpoint is reached the fan relay circuit switches 
  and activates the relay to activate the extraction fan to extract the hot fumes quickly so that the temperature drops to the COOL temperature setpoint. At this stage it will switch off 
  again to naturally cool down. You have the choice to either use the fan relay switching circuit by activating it or to not use it by leaving it deactivated.
  
  Once the temperature drops below 50 Degrees Celcius or the the final count up timer reaches 30 minutes, the program will exit to the main menu. The unit also has a function to store all 
  the program variables to the EEPROM so that if you power down the unit and power it back up you don't lose your program variables. This allows the unit to be a standalone unit without the 
  need to use a computer.
  
  
  Temperature (Degree Celcius)
  
  245-     |                                                             * * *
           |                                                           * |     | *
           |                                                         *   |     |   *
           |                                                       *     |     |    *
  200-     |                                           * * * * * *       |     |      *
           |                                         * |         |       |     |        *
           |                                       *   |         |       |     |        |*
           |                                     *     |         |       |     |        | *
  150-     |               * * * * * * * * * * *       |         |       |     |        |  *
           |             * |                   |       |         |       |     |        |    *
           |           *   |                   |       |         |       |     |        |      *
           |         *     |                   |       |         |       |     |        |      |  *
           |       *       |                   |       |         |       |     |        |      |     *
           |     *         |                   |       |         |       |     |        |      |         *
           |   *           |                   |       |         |       |     |        |      |              *
  Ambient- | *             |                   |       |         |       |     |        |      |                   *
           |               |                   |       |         |       |     |        |      |
           |       1       |         2         |   3   |    4    |   5   |  6  |   7    |  8   |           9
   0       |_ _ _ _ _ _ _ _|_ _ _ _ _ _ _ _ _ _|_ _ _ _|_ _ _ _ _| _ _ _ |_ _ _|_ _ _ _ | _ _ _|_ _ _ _ _ _ _ _ _ _ _
  
                                                                 Time (Seconds)
  
  
  Brief Operation:
  *****************
  
  1.    The temperature ramps up to the specified PREH temperature.
  2.    Once the PREH temperature is reached a holding timer starts to count down to keep the temperature constant.
  3.    When the count down timer reaches 0 the SOAK setpoint is implemented and the temperature ramps up to the desired setpoint.
  4.    When the setpoint is reached the appropriate count down timer is initiated.
  5.    Once the timer reaches 0 the RFLW setpoint is implemented and the temperature ramps up towards the setpoint.
  6.    Once the sepotint is reached the appropriate count down timer is initiated.
  7.    Once the count down timer reaches 0 a count up timer is initiated and the PID controller is switched off.
  8.    Once the KEEP setpoint is reached an extractor fan is activated (optional).
  9.    When the COOL setpoint is reached the extractor fan is de-activated and the status changes to DONE.

  
  Hysteresis Function:
  *********************
  There is a built in hysteresis function for each ramping stage. It can be used if the PID controller is struggling to reach the specific ramp setpoint. What it does is, it adds the 
  hysteresis temp to the setpoint during the ramp stage only. So that the PID controller will be fully on a bit longer than it normally would, ensuring that you reach your setpoint quicker 
  and that the PID controller doesn't linger just beneath the setpoint. Once the setpoint is reached and the count down timer is initiated the hysteresis value is discarded and the setpoint 
  is set back to the orginal setpoint. The hysteresis for each ramp stage is limited to 30 Degrees Celcius. Setting it too high might result in overshooting your setpoint by a bit.
  
  Libraries:
  ***********
  The firmware came into being from other design references but at the heart of the controller lies Brett Beauregard's (www.brettbeauregard.com) PID library as well as the openGLCD library.
  So to get the code to work you will have to download the following libraries from here:
  
  Arduino PID Library:
  >> https://github.com/br3ttb/Arduino-PID-Library
  
  openGLCD Library:
  >> https://bitbucket.org/bperrybap/openglcd/wiki/Home
  
  As far as open source goes, this firmware is opensource as well as the hardware, BUT, the reproducing of the hardware for commerical use is limited to
  Paladin Enabling Technologies and whoever Paladin Enabling Technologies might endorse with an production license.
  
  
  DISCLAIMER:
  ***********
  Working with high voltage is DANGEROUS. Make sure that you know what you are doing and that you have the proper knowledge before hand.  Your use of any
  information or materials on this reflow oven controller is entirely at your own risk for which Paladin Enabling Technologies will not be held liable for.
  
  Licences:
  *********
  The hardware and firmware designs of this solder reflow oven controller are released under the Creative Commons Share Alike v3.0 license
  
  http://creativecommons.org/licenses/by-sa/3.0/
  
  You are free to take this code, use it and modify it. All we ask is attribution including the supporting libraries used in this
  firmware. Reproducing the hardware commercially however is prohibited.

  ******************************************************************************************************************************************************************************/
  
  #include <PID_v1.h>

  #include <openGLCD.h>
  #include <include/openGLCD_GLCDv3.h> // header to provide GLCDv3 compatibility

  #include <fonts/allFonts.h>          // include the Fonts
  #include <avr/eeprom.h>    
  
  #define RelayPin 2
  
  unsigned long StartofTimer_Seconds = 0;
  unsigned long StartofTimer_Graphing_Interval = 0;

  //******************************************************************************************************************************************************************************
  //**************************************                        User programmable Setable Values                       *********************************************************
  //******************************************************************************************************************************************************************************

  byte Reflow_Program_Cutout_Time = 30;        //  The time that the process must keep on displaying before exiting to the main menu after reflow process is completed.
  //  (Note temperature takes a while to cool down to below 50 Deg C, about 30 min with the oven I'm using. (50 Deg C = Own madeup safe temp).
  
  byte Reflow_Program_Cutout_Temp = 50;        //  The temp that must be reached once the reflow process have been completed before exiting to the main menu.
  //  (Note that the program will quit to the main menu either when the temp. drops below the cutout temp value or when the timer goes above the cutout time value. Timer value in minutes.)
  
  byte Temp_Offset = 3;                        //  Offset to compensate if the PID controller is not keeping the temperature on target. Can be a positive or a negative value. The value
  //  shouldn't exceed 5 Deg C.
                                             
  //******************************************************************************************************************************************************************************
  //**************************************                        Declaring further global variables                     *********************************************************
  //******************************************************************************************************************************************************************************
    
  int Stored_Values [12];    //  Array to store variables into the EEPROM

  //*********************************************************************************************************************************************************    

  char* String_Lookup_Table_Array[] =     // String array with 21 elements.
  {
    "MENU:",                              // String 0
    "ESC",                                // String 1
    "ENT",                                // String 2
    "PREH",                               // String 3
    "SOAK",                               // String 4
    "RFLW",                               // String 5
    "KEEP",                               // String 6
    "COOL",                               // String 7
    "DONE",                               // String 8
    "HYSTERESIS",                         // String 9
    "EDIT",                               // String 10
    "VIEW",                               // String 11
    "RUN",                                // String 12
    "STORE",                              // String 13
    "PROGRAM",                            // String 14
    "PROFILE",                            // String 15
    "FAN STATUS",                         // String 16
    "OFF",                                // String 17
    " ON",                                // String 18
    "STORING VARIABLES",                  // String 19
    "TO EEPROM",                          // String 20
    "STORE",                              // String 21
    "VARIABLES"                           // String 22
  };

  //*********************************************************************************************************************************************************  
  
  const int numReadings = 10;     // Averaging Variable

  int readings[numReadings];      // the readings from the analog input
  int index = 0;                  // the index of the current reading
  int total = 0;                  // the running total
  int Raw_Value_Avg = 0;          // the average  
  int Raw_Value = 0;
  
  //******************************************************************************************************************************************************************************
  //**************************************                           Declaring PID Controller P                          *********************************************************
  //******************************************************************************************************************************************************************************
  
  // ***** PID PARAMETERS *****
  // ***** PRE-HEAT STAGE *****
  #define PID_KP_PREH 100            
  #define PID_KI_PREH 0.025
  #define PID_KD_PREH 20
  // ***** SOAKING STAGE *****
  #define PID_KP_HEAT 300            
  #define PID_KI_HEAT 0.020
  #define PID_KD_HEAT 350            
  // ***** REFLOW STAGE *****        
  #define PID_KP_SLDR 300            
  #define PID_KI_SLDR 0.01
  #define PID_KD_SLDR 350            
  
  //*************************************************************

  double kp = PID_KP_PREH;                                                                                                              //  Set PID paramaters to the PREH PID paramaters.
  double ki = PID_KI_PREH;
  double kd = PID_KD_PREH;
  
  //*************************************************************
  
  //Define Variables we'll be connecting to
  double Setpoint, Input, Output;
  
  //Specify the links and initial tuning parameters - Rocketscream example
  PID myPID(&Input, &Output, &Setpoint, kp, ki, kd, DIRECT);
  
  int WindowSize = 2000;
  unsigned long windowStartTime;

  //******************************************************************************************************************************************************************************
  
  const byte ESC_Button   = 0;
  const byte UP_Button    = 1;
  const byte DWN_Button   = 12;
  const byte ENT_Button   = 13;

  int sensorPin     =  A5;                                                                                                                // select the input pin for the potentiometer  
  byte FanPin       =   3;
  
  int Stage_Temperature_Setpoints[] =            { 150, 180, 210, 180, 150, 20, 0, 20, 10 };                                               //  Various Stage temperatures - Presetable
  byte Button_States_Selectors_and_Detectors[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0 };                                //  Various button states and detector variables + Temp step calculator variables
  byte XY_Coordinates[] =                        { 0, 0, 5, 47, 23, 37, 0, 46, 60, 0, 70, 78, 0, 93, 0, 100, 0, 120, 47 };    //  Pixal coordinates for graphing calculations
           
  String Status;    
 
  int     Temp;
  int     Seconds;
  byte    Minutes;
  byte    ENT_Through_Status;

  int    Temp_Limit1 = 0;
  int    Temp_Limit2 = 0;
  int    Temp_Step = 0;

  int Variable;      
  int Min_Variable;   
  int Increment_Variable = 1;              //  Increment variable generaly has a value larger than 255 so you can't use an byte and will have to use an integer. (Time in milliseconds devided by pixals)
  int Ramp_Temp_Range    = 1;              //  Can't be a byte, the value may be smaller than 255 but a byte is from 0 - 255 and not from -255 to +255. Next one up is an integer that goes from - 32k to
  int Void_Time;

  byte PREH_Pixals               = 18;
  byte PREH_Keep_Pixals          = 15; 
  byte HEAT_Pixals               =  8;
  byte HEAT_Keep_Pixals          = 14;  
  byte SLDR_Pixals               =  9;
  byte SLDR_Keep_Pixals          =  8;  
  byte KEEP_Pixals               = 15;
  byte COOL_Pixals               =  7;
  byte DONE_Pixals               = 21;
  byte Increment_Detector        =  1;  
  byte Timer_Decrement_Detector  =  0;
  byte Second_Detector           =  0;
  byte Loop_Through_Detector     =  1;    
  byte Fan_Status                =  0;


  byte PREH_Hysteresis;                        //  Hysteresis value for PREHEAT = PREHEAT setpoint - PREH_Hysteresis. This value will trigger the count down timer /  hold timer for the PREHEAT stage and prevent the elements from 
                                               //  struggling to reach the exact Setpoint should your oven struggle to reach it in a relative short period of time.
  
  byte HEAT_Hysteresis;                        //  Hysteresis value for HEAT = HEAT setpoint - HEAT_Hysteress. Does the same as PREH_Hysteresis but for the HEAT stage.
  byte SLDR_Hysteresis;                        //  Hysteresis value for SLDR = SLDR setpoint - SLDR_Hysteresis. Does the same as PREH_Hysteresis but for the SLDR stage.

  //****************************************************************************************************************************************************************************
  
  void setup() 
  {
    GLCD.Init(NON_INVERTED);     // Initialize the GLCD 
    GLCD.ClearScreen(); 
    GLCD.Init();
  
    // initialize all the readings to 0: 
    for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;   
          
    eeprom_read_block((void*)&Stored_Values, (void*)0, sizeof(Stored_Values));          // Reading information stored in the EEPROM and storing it into array Stored_Values. Start reading from address 0        
    
    Store_Storage_Array_Elements_into_Variables();                                      // Storing Stored_Values array values into variables.
    
    if ( Button_States_Selectors_and_Detectors[12] != 0 || Button_States_Selectors_and_Detectors[13]  != 0 || Button_States_Selectors_and_Detectors[14] != 0 )              // Setting Minute variables back to zero should you start up with a minute variable that is not at zero.  
    {
         Button_States_Selectors_and_Detectors[12] = 0;
         Button_States_Selectors_and_Detectors[13] = 0;
         Button_States_Selectors_and_Detectors[14] = 0;       
    }
    
    Convert_Seconds_to_Minutes();                                                       // Converts seconds back to minutes for usage in the program  
      
    pinMode ( ESC_Button, INPUT );                                                      // Sets digital I/O points to digital inputs
    pinMode ( UP_Button,  INPUT );
    pinMode ( DWN_Button, INPUT );
    pinMode ( ENT_Button, INPUT );    
                                                                          
    // Notes: Setting RelayPin and FanPin as outputs, back to inputs and then back to outputs again since the relays don't always engage from startup if I don't do this.

    pinMode(RelayPin, OUTPUT);                                                          // Sets digital I/O points to digital outputs   
    pinMode(FanPin,   OUTPUT);
    
    Temp_Limit1              =   0;
    Temp_Limit2              =  15;   
    XY_Coordinates[0]                                      =   5;
    XY_Coordinates[1]                                      =  48;  
    
    digitalWrite(RelayPin, LOW);                                                        //  Sets relay outputs low so that it will activate first time without having to reset the controller.
    digitalWrite(FanPin,   LOW); 

  }

  void loop()
  {          
    
       Read_Buttons();        
       Draw_Menu_Blocks();
       
       if( Button_States_Selectors_and_Detectors[11]  == 1)
       {
         GLCD.SelectFont(System5x7, WHITE);                                                                       
         GLCD.CursorToXY(50, 3);
         GLCD.print(String_Lookup_Table_Array[0]);   
       }
       
        Test_UP_DOWN_Counter();
        Test_ENT_ESC_Counter_Value();
        
        if( Button_States_Selectors_and_Detectors[11]  <= 1 )
        {
          Button_States_Selectors_and_Detectors[11] = 1;
        }       
       
        if( Button_States_Selectors_and_Detectors[11]  >=3 )
        {
          Button_States_Selectors_and_Detectors[11] = 1;
        }            
            
        if (Button_States_Selectors_and_Detectors[10] < 1 )
        {
          Button_States_Selectors_and_Detectors[10] = 4;
        }

        if ( Button_States_Selectors_and_Detectors[10] > 4 )
        {
          Button_States_Selectors_and_Detectors[10] = 1;
        } 
                
        Main_Menu_Case_Switch();
        
  //****************************************************************************************************************************************************************************
  //**************************************                         Entering Submenu Edit Paramaters                      *******************************************************
  //****************************************************************************************************************************************************************************
  
        if( Button_States_Selectors_and_Detectors[11]  == 2 && Button_States_Selectors_and_Detectors[10] == 1 )
        {
              GLCD.ClearScreen();          
              Button_States_Selectors_and_Detectors[10] = 1;
              Button_States_Selectors_and_Detectors[8] = 1;

              Draw_Submenu_Lines();
              
              while( Button_States_Selectors_and_Detectors[11]  == 2 ) 
              {
                Main_Menu_Case_Switch();
                
                Read_Buttons();
    
                GLCD.DrawLine( 31, 52,  31, 62);    // Lines to draw four blocks at the bottom.
                GLCD.DrawLine( 63, 52,  63, 62);
                GLCD.DrawLine( 94, 52,  94, 62);
                
                Test_UP_DOWN_Counter();
                Test_ENT_ESC_Counter_Value();
              
                Not_Highlighted_Up_Button();
                Not_Highlighted_Down_Button();
                         
                if (Button_States_Selectors_and_Detectors[10] < 1 )
                {
                  Button_States_Selectors_and_Detectors[10] = 3;
                }
                if (Button_States_Selectors_and_Detectors[10] > 3 )
                {
                  Button_States_Selectors_and_Detectors[10] = 1;
                }               
                
                SubMenu_Edit_Program();
                SubMenu_Edit_Hysteresis();
                SubMenu_Fan_Status();
                 
              }              
              
              GLCD.ClearScreen();
              Draw_Menu_Blocks(); 
              Button_States_Selectors_and_Detectors[10] = 1;   
             
           }        

  //****************************************************************************************************************************************************************************
  //**************************************                         Entering Submenu View Program                        ********************************************************
  //****************************************************************************************************************************************************************************
     
        if( Button_States_Selectors_and_Detectors[11]  == 2 && Button_States_Selectors_and_Detectors[10] == 2 )
        {
          Button_States_Selectors_and_Detectors[10] = 1;
          GLCD.ClearScreen();

                 
          while( Button_States_Selectors_and_Detectors[11]  == 2 ) 
          {
            Read_Buttons();
    
            GLCD.DrawLine( 31, 52,  31, 62);    // Lines to draw four blocks at the bottom.
            GLCD.DrawLine( 63, 52,  63, 62);
            GLCD.DrawLine( 94, 52,  94, 62);            
              
            Test_UP_DOWN_Counter();
            Test_ENT_ESC_Counter_Value();
            
            if (Button_States_Selectors_and_Detectors[10] < 1 )
            {
              Button_States_Selectors_and_Detectors[10] = 4;
            }
            if (Button_States_Selectors_and_Detectors[10] > 4 )
            {
              Button_States_Selectors_and_Detectors[10] = 1;
            }            
                
            View_Program();              
          }
          GLCD.ClearScreen();
          Draw_Menu_Blocks(); 
        }

  //****************************************************************************************************************************************************************************
  //**************************************                              RUNNING CYCLE SCREEN                             *******************************************************
  //****************************************************************************************************************************************************************************

        if( Button_States_Selectors_and_Detectors[11]  == 2 && Button_States_Selectors_and_Detectors[10] == 3 )
        {        
          GLCD.ClearScreen();
          
          while( Button_States_Selectors_and_Detectors[11]  == 2 ) 
          {
            Read_Buttons();    
           
            RunningCycleScreen();            
            
            if ( Button_States_Selectors_and_Detectors[0] == LOW ) 
            {
              Button_States_Selectors_and_Detectors[11]--;
              GLCD.ClearScreen(); 
             
              if ( Button_States_Selectors_and_Detectors[11] < 0 )
              {
                Button_States_Selectors_and_Detectors[11] = 0;
              }
            }
              
            Button_States_Selectors_and_Detectors[0]   = 0;
            Button_States_Selectors_and_Detectors[10]   = 1;

          }  
              GLCD.ClearScreen();
              Draw_Menu_Blocks();           
        }

  //****************************************************************************************************************************************************************************
  //**************************************                      STORING VARIABLE VARIABLES TO EEPROM                     *******************************************************
  //****************************************************************************************************************************************************************************

        if( Button_States_Selectors_and_Detectors[11]  == 2 && Button_States_Selectors_and_Detectors[10] == 4 )
        {        
          GLCD.ClearScreen();

          Convert_Minutes_to_Seconds();                                                        // Converts minutes into seconds for storage
          
          Store_Variables_into_Storage_Array_Elements();                                       // Store variables to storage array
          
          eeprom_write_block((const void*)&Stored_Values, (void*)0, sizeof(Stored_Values));    // Store storage array values into EEPROM
          
          Convert_Seconds_to_Minutes();                                                        // Converts seconds back to minutes for usage in the program          

          GLCD.SelectFont(System5x7, BLACK);

          GLCD.CursorToXY(2, 15);

          GLCD.CursorToXY(14, 25);
          GLCD.print(String_Lookup_Table_Array[19]);       
          GLCD.CursorToXY(38, 35);
          GLCD.print(String_Lookup_Table_Array[20]);  
       
          GLCD.DrawRect( 10, 21, 108, 23, BLACK);
          GLCD.DrawRect(  8, 19, 112, 27, BLACK);     
 
          delay(2000);

          Button_States_Selectors_and_Detectors[11]     = 1;
          Button_States_Selectors_and_Detectors[10]     = 1;

          GLCD.ClearScreen();
          Draw_Menu_Blocks();           
        } 
  }

  //****************************************************************************************************************************************************************************
  //**************************************                        Draw Submenu Lines and text    *******************************************************************************
  //****************************************************************************************************************************************************************************

            void Draw_Submenu_Lines()
            {
              Draw_Menu_Blocks();   
              GLCD.SelectFont(System5x7, WHITE);  
              GLCD.DrawLine(  29, 2, 106, 2);                  // Line above Edit Program
              GLCD.FillRect(  26, 2,   2,  8, BLACK);             
              GLCD.CursorToXY(29, 3 );
              GLCD.print(String_Lookup_Table_Array[10]);              
              GLCD.print(" ");
              GLCD.print(String_Lookup_Table_Array[14]);
              GLCD.print(":");              
            }
            
            void Hysteresis_Space_Calculator ( int Hysteresis_Variable )
            {
              if ( Hysteresis_Variable < 10 )
              {
                GLCD.print(" "); 
                GLCD.print(Hysteresis_Variable);
              }
                                
              else
              {
                GLCD.print(Hysteresis_Variable);
              } 
            }            

            void Hysteresis_Limiter()
            {
              if ( Button_States_Selectors_and_Detectors[9] <= 0 )
              {
                Button_States_Selectors_and_Detectors[9] = 0;
              }
             
              if ( Button_States_Selectors_and_Detectors[9] >= 30 )
              {
                Button_States_Selectors_and_Detectors[9] = 30;
              } 
            }

  //****************************************************************************************************************************************************************************
  //**************************************                               SubMenu Edit Program                           ********************************************************
  //****************************************************************************************************************************************************************************

  void SubMenu_Edit_Program()
  {
        if( Button_States_Selectors_and_Detectors[11]  == 3 && Button_States_Selectors_and_Detectors[10] == 1 )
        {
              Button_States_Selectors_and_Detectors[10] = 1;
              Button_States_Selectors_and_Detectors[8] = 1;
              GLCD.ClearScreen();
              
              while( Button_States_Selectors_and_Detectors[11]  == 3 ) 
              {
                Read_Buttons();
    
                GLCD.DrawLine( 31, 52,  31, 62);    // Lines to draw four blocks at the bottom.
                GLCD.DrawLine( 63, 52,  63, 62);
                GLCD.DrawLine( 94, 52,  94, 62);
                
                Test_ENT_ESC_Counter_Value();
                
                View_Program(); 
                
                Not_Highlighted_Up_Button();
                Not_Highlighted_Down_Button();
                                
                Edit_Program();
                
              }
              
              GLCD.ClearScreen();
              Draw_Submenu_Lines();
             }               
    
  }
  
  //****************************************************************************************************************************************************************************
  //**************************************                               SubMenu Edit Hysteresis                        ********************************************************
  //****************************************************************************************************************************************************************************

  void SubMenu_Edit_Hysteresis()
  {
        if( Button_States_Selectors_and_Detectors[11]  == 3 && Button_States_Selectors_and_Detectors[10] == 2 )
        {
              Button_States_Selectors_and_Detectors[9] = PREH_Hysteresis;
              Button_States_Selectors_and_Detectors[8] = 1;
              GLCD.FillRect  (  1, 13, 125, 37, WHITE);   
              
              while( Button_States_Selectors_and_Detectors[8]  == 1 ) 
              {
                Read_Buttons();
    
                GLCD.DrawLine( 31, 52,  31, 62);    // Lines to draw four blocks at the bottom.
                GLCD.DrawLine( 63, 52,  63, 62);
                GLCD.DrawLine( 94, 52,  94, 62);
                
             
                Test_UP_DOWN_Counter();
                Test_ENT_ESC_Counter_Value();            
                
                Not_Highlighted_Up_Button();
                Not_Highlighted_Down_Button();
                
                HysMenu_Line1_Highlighted();
                HysMenu_Line2_Not_Highlighted();
                HysMenu_Line3_Not_Highlighted();
                
                Hysteresis_Limiter();
                
                PREH_Hysteresis = Button_States_Selectors_and_Detectors[9];
                GLCD.CursorToXY( 102, 15 );    
                Hysteresis_Space_Calculator ( PREH_Hysteresis );

                GLCD.CursorToXY( 102, 24 ); 
                Hysteresis_Space_Calculator ( HEAT_Hysteresis);
                              
                GLCD.CursorToXY( 102, 33 );       
                Hysteresis_Space_Calculator ( SLDR_Hysteresis);
                
                if ( Button_States_Selectors_and_Detectors[8]  <= 1 )
                {
                  Button_States_Selectors_and_Detectors[8] = 1;
                }

                if ( Button_States_Selectors_and_Detectors[11]  < 3 || Button_States_Selectors_and_Detectors[11]  > 3 )
                {
                  Button_States_Selectors_and_Detectors[11] = 3;
                }
                
                if ( Button_States_Selectors_and_Detectors[8] == 2 )
                {
                  Button_States_Selectors_and_Detectors[9] = HEAT_Hysteresis;
                  
                  while ( Button_States_Selectors_and_Detectors[8] == 2 )
                  {
                    
                    Read_Buttons();
                    
                    Test_UP_DOWN_Counter();
                    Test_ENT_ESC_Counter_Value();            
                    
                    Not_Highlighted_Up_Button();
                    Not_Highlighted_Down_Button();
                    
                    HysMenu_Line1_Not_Highlighted();
                    HysMenu_Line2_Highlighted();
                    HysMenu_Line3_Not_Highlighted();
                    
                    Hysteresis_Limiter();
                    
                    
                    GLCD.CursorToXY( 102, 15 );    
                    Hysteresis_Space_Calculator ( PREH_Hysteresis );
    
                    HEAT_Hysteresis = Button_States_Selectors_and_Detectors[9];
                    GLCD.CursorToXY( 102, 24 ); 
                    Hysteresis_Space_Calculator ( HEAT_Hysteresis);
                                  
                    GLCD.CursorToXY( 102, 33 );       
                    Hysteresis_Space_Calculator ( SLDR_Hysteresis);                    
                    
                    if ( Button_States_Selectors_and_Detectors[8]  <= 2 )
                    {
                      Button_States_Selectors_and_Detectors[8] = 2;
                    }   
   
                    if ( Button_States_Selectors_and_Detectors[11]  < 3 || Button_States_Selectors_and_Detectors[11]  > 3 )
                    {
                      Button_States_Selectors_and_Detectors[11] = 3;
                    }                    
                    
                    if ( Button_States_Selectors_and_Detectors[8] == 3 )
                    {
                      Button_States_Selectors_and_Detectors[9] = SLDR_Hysteresis;
                      
                      while ( Button_States_Selectors_and_Detectors[8] == 3 )
                      {
                        
                        Read_Buttons();
                        
                        Test_UP_DOWN_Counter();
                        Test_ENT_ESC_Counter_Value();            
                        
                        Not_Highlighted_Up_Button();
                        Not_Highlighted_Down_Button();
                        
                        HysMenu_Line1_Not_Highlighted();
                        HysMenu_Line2_Not_Highlighted();
                        HysMenu_Line3_Highlighted();
                        
                        Hysteresis_Limiter();
                                                
                        GLCD.CursorToXY( 102, 15 );    
                        Hysteresis_Space_Calculator ( PREH_Hysteresis );
        
                        GLCD.CursorToXY( 102, 24 ); 
                        Hysteresis_Space_Calculator ( HEAT_Hysteresis);

                        SLDR_Hysteresis = Button_States_Selectors_and_Detectors[9];                                      
                        GLCD.CursorToXY( 102, 33 );       
                        Hysteresis_Space_Calculator ( SLDR_Hysteresis);                    

                        if ( Button_States_Selectors_and_Detectors[8]  <= 3 )
                        {
                          Button_States_Selectors_and_Detectors[8] = 3;
                        }    
                        
                        if ( Button_States_Selectors_and_Detectors[11]  < 3 || Button_States_Selectors_and_Detectors[11]  > 3 )
                        {
                          Button_States_Selectors_and_Detectors[11] = 3;
                        }                        

                        if ( Button_States_Selectors_and_Detectors[8] == 4 )
                        {
                          Button_States_Selectors_and_Detectors[11]  = 2;
                          Button_States_Selectors_and_Detectors[8] = 0;
                          Button_States_Selectors_and_Detectors[10] = 1;
                        }                     
                        
                      }
                    }
                  }
                }              
              }              
             }                   
  }

  //****************************************************************************************************************************************************************************
  //**************************************                               SubMenu Edit Fan Status                        ********************************************************
  //****************************************************************************************************************************************************************************

  void SubMenu_Fan_Status()
  {
    if( Button_States_Selectors_and_Detectors[11]  == 3 && Button_States_Selectors_and_Detectors[10] == 3 )
    {
      Button_States_Selectors_and_Detectors[9] = Fan_Status;
      Button_States_Selectors_and_Detectors[8] = 1;
      GLCD.FillRect  (  1, 13, 125, 37, WHITE);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
        
      while ( Button_States_Selectors_and_Detectors[8] == 1 )
      {
        Read_Buttons();
                         
        Test_UP_DOWN_Counter();
        Test_ENT_ESC_Counter_Value();          
                          
        GLCD.SelectFont(System5x7,     WHITE);   
        GLCD.FillRect(  17, 15,   2,  7, BLACK);          
        GLCD.CursorToXY( 20, 15 );
        GLCD.print(String_Lookup_Table_Array[16]);
        GLCD.print(":");
      
        GLCD.SelectFont(System5x7, BLACK);        
        GLCD.CursorToXY( 92, 15 );          
        
        if ( Button_States_Selectors_and_Detectors[9] <= 0 )
        {
          Button_States_Selectors_and_Detectors[9] = 0; 
          GLCD.print(String_Lookup_Table_Array[17]);                
        }                  
        
        if ( Button_States_Selectors_and_Detectors[9] >= 1 )
        {
          Button_States_Selectors_and_Detectors[9] = 1;
          GLCD.print(String_Lookup_Table_Array[18]);                   
        }       
        
        Fan_Status = Button_States_Selectors_and_Detectors[9];
        
        if ( Button_States_Selectors_and_Detectors[8]  <= 1 )
        {
          Button_States_Selectors_and_Detectors[8] = 1;
        } 
     
        if ( Button_States_Selectors_and_Detectors[11]  < 3 || Button_States_Selectors_and_Detectors[11]  > 3 )
        {
          Button_States_Selectors_and_Detectors[11] = 3;
        }        
        
        if ( Button_States_Selectors_and_Detectors[8] == 2 )  
        {
          Button_States_Selectors_and_Detectors[11]  = 2;
          Button_States_Selectors_and_Detectors[8] = 0;
          Button_States_Selectors_and_Detectors[10] = 1;
        }      
      }
    }
  }  

  //****************************************************************************************************************************************************************************
  //**************************************                            Hysteresis lines highlighted                       *******************************************************
  //****************************************************************************************************************************************************************************

  void HysMenu_Line1_Highlighted()
  {   
    GLCD.SelectFont(System5x7,     WHITE);                                                                       
    GLCD.CursorToXY( 3, 15 );
    GLCD.print(String_Lookup_Table_Array[3]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[9]);    
    GLCD.print(":");    
    GLCD.DrawCircle( 117, 16, 1);
    GLCD.SelectFont(System5x7, BLACK);        
    GLCD.CursorToXY( 120, 15 );       
    GLCD.print("C");      
  }
  
  void HysMenu_Line2_Highlighted()
  {    
    GLCD.SelectFont(System5x7,     WHITE);                                                                       
    GLCD.CursorToXY( 3, 24 ); 
    GLCD.print(String_Lookup_Table_Array[4]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[9]);    
    GLCD.print(":");     
    GLCD.DrawCircle( 117, 25, 1);
    GLCD.SelectFont(System5x7, BLACK);        
    GLCD.CursorToXY( 120, 24 );       
    GLCD.print("C");    
  }
  
  void HysMenu_Line3_Highlighted()
  {    
    GLCD.SelectFont(System5x7,        WHITE);                                                                       
    GLCD.CursorToXY( 3, 33 );
    GLCD.print(String_Lookup_Table_Array[5]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[9]);    
    GLCD.print(":");     
    GLCD.DrawCircle( 117, 34, 1);
    GLCD.SelectFont(System5x7, BLACK);        
    GLCD.CursorToXY( 120, 33 );       
    GLCD.print("C");    
  }
  
  //****************************************************************************************************************************************************************************
  //**************************************                         Hysteresis lines Not highlighted                      *******************************************************
  //****************************************************************************************************************************************************************************
                
  void HysMenu_Line1_Not_Highlighted()
  {     
    GLCD.SelectFont(System5x7,     BLACK);                                                                       
    GLCD.CursorToXY( 3, 15 );
    GLCD.print(String_Lookup_Table_Array[3]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[9]);    
    GLCD.print(":");      
    GLCD.DrawCircle( 117, 16, 1);
    GLCD.SelectFont(System5x7, BLACK);        
    GLCD.CursorToXY( 120, 15 );       
    GLCD.print("C");    
  }
  
  void HysMenu_Line2_Not_Highlighted()
  { 
    GLCD.SelectFont(System5x7,     BLACK);    
    GLCD.CursorToXY( 3, 24 );
    GLCD.print(String_Lookup_Table_Array[4]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[9]);    
    GLCD.print(":");  
    GLCD.DrawCircle( 117, 25, 1);
    GLCD.SelectFont(System5x7, BLACK);        
    GLCD.CursorToXY( 120, 24 );       
    GLCD.print("C");  
  }
  
  void HysMenu_Line3_Not_Highlighted()
  {   
    GLCD.SelectFont(System5x7,        BLACK);   
    GLCD.CursorToXY( 3, 33 );
    GLCD.print(String_Lookup_Table_Array[5]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[9]);    
    GLCD.print(":");   
    GLCD.DrawCircle( 117, 34, 1);
    GLCD.SelectFont(System5x7, BLACK);        
    GLCD.CursorToXY( 120, 33 );       
    GLCD.print("C");   
  }  
    
  //****************************************************************************************************************************************************************************
  //**************************************                             SubMenu lines highlighted                         *******************************************************
  //****************************************************************************************************************************************************************************

  void SubMenu_Line1_Highlighted()
  {
    GLCD.DrawLine( 1, 14, 126, 14, BLACK);   
    GLCD.FillRect  (  1, 15,   12,  7, BLACK); 
    GLCD.FillRect  (121, 15,   5,  7, BLACK);     
    GLCD.SelectFont(System5x7,     WHITE);                                                                       
    GLCD.CursorToXY( 13, 15 );
    GLCD.print("   ");
    GLCD.print(String_Lookup_Table_Array[10]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[15]);
    GLCD.print("   ");
  }
  
  void SubMenu_Line2_Highlighted()
  {
    GLCD.DrawLine( 1, 23, 126, 23, BLACK);  
    GLCD.FillRect  (  1, 24,   2,  7, BLACK);    
    GLCD.FillRect  (124, 24,   2,  7, BLACK);     
    GLCD.SelectFont(System5x7,     WHITE);                                                                       
    GLCD.CursorToXY( 4, 24 );    
    GLCD.print("   ");
    GLCD.print(String_Lookup_Table_Array[10]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[9]);
    GLCD.print("  ");    
  }  
  
  void SubMenu_Line3_Highlighted()  
  {
    GLCD.DrawLine  ( 1, 32, 126, 32, BLACK);
    GLCD.FillRect  (  1, 33,   2,  7, BLACK);    
    GLCD.FillRect  (124, 33,   2,  7, BLACK); 
    GLCD.SelectFont(System5x7,     WHITE);                                                                       
    GLCD.CursorToXY( 4, 33 );
    GLCD.print("   ");
    GLCD.print(String_Lookup_Table_Array[10]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[16]);
    GLCD.print("  ");        
  } 

  //****************************************************************************************************************************************************************************
  //**************************************                           SubMenu lines NOT highlighted                       *******************************************************
  //****************************************************************************************************************************************************************************

  void SubMenu_Line1_Not_Highlighted()
  {
    GLCD.DrawLine( 1, 14, 126, 14, WHITE);
    GLCD.FillRect  (  1, 15,   12,  7, WHITE); 
    GLCD.FillRect  (121, 15,   5,  7, WHITE);     
    GLCD.SelectFont(System5x7,     BLACK);                                                                       
    GLCD.CursorToXY( 13, 15 );
    GLCD.print("   ");
    GLCD.print(String_Lookup_Table_Array[10]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[15]);
    GLCD.print("   ");  
  }
  
  void SubMenu_Line2_Not_Highlighted()
  {
    GLCD.DrawLine( 1, 23, 126, 23, WHITE);        
    GLCD.FillRect  (  1, 24,   2,  7, WHITE);    
    GLCD.FillRect  (124, 24,   2,  7, WHITE); 
    GLCD.SelectFont(System5x7,     BLACK);    
    GLCD.CursorToXY( 4, 24 );
    GLCD.print("   ");
    GLCD.print(String_Lookup_Table_Array[10]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[9]);
    GLCD.print("  "); 
  }

  void SubMenu_Line3_Not_Highlighted()  
  {
    GLCD.DrawLine  ( 1, 32, 126, 32, WHITE);
    GLCD.FillRect  (  1, 33,   2,  7, WHITE);    
    GLCD.FillRect  (124, 33,   2,  7, WHITE); 
    GLCD.SelectFont(System5x7, BLACK );                                                                       
    GLCD.CursorToXY( 4, 33 );
    GLCD.print("   ");
    GLCD.print(String_Lookup_Table_Array[10]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[16]);
    GLCD.print("  "); 
  }
  
  //****************************************************************************************************************************************************************************
  //**************************************                    Menu Case Switch - Used to draw sub menu                   *******************************************************
  //****************************************************************************************************************************************************************************

  void Main_Menu_Case_Switch() 
  {
    switch ( Button_States_Selectors_and_Detectors[10] )
    {
        case 1:
          
          if ( Button_States_Selectors_and_Detectors[11]  == 1 )
          {
            Menu_Line1_Highlighted();  
            Menu_Line2_Not_Highlighted(); 
            Menu_Line3_Not_Highlighted(); 
            Menu_Line4_Not_Highlighted();             
          }               

          if ( Button_States_Selectors_and_Detectors[11]  == 2 )
          {
            SubMenu_Line1_Highlighted();  
            SubMenu_Line2_Not_Highlighted(); 
            SubMenu_Line3_Not_Highlighted();           
          }               
          break;                       
          
        case 2:
          
          if ( Button_States_Selectors_and_Detectors[11]  == 1 )
          {
            Menu_Line1_Not_Highlighted();  
            Menu_Line2_Highlighted(); 
            Menu_Line3_Not_Highlighted(); 
            Menu_Line4_Not_Highlighted();  
          } 
          
          if ( Button_States_Selectors_and_Detectors[11]  == 2 )
          {
            SubMenu_Line1_Not_Highlighted();  
            SubMenu_Line2_Highlighted(); 
            SubMenu_Line3_Not_Highlighted(); 
          } 
          break;  
          
        case 3:
          
          if ( Button_States_Selectors_and_Detectors[11]  == 1 )
          {
            Menu_Line1_Not_Highlighted();  
            Menu_Line2_Not_Highlighted(); 
            Menu_Line3_Highlighted(); 
            Menu_Line4_Not_Highlighted();   
          }
          
          if ( Button_States_Selectors_and_Detectors[11]  == 2 )
          {
            SubMenu_Line1_Not_Highlighted();  
            SubMenu_Line2_Not_Highlighted(); 
            SubMenu_Line3_Highlighted(); 
          }          
          break; 

          case 4:
          
          if ( Button_States_Selectors_and_Detectors[11]  == 1 )
          {
            Menu_Line1_Not_Highlighted();  
            Menu_Line2_Not_Highlighted(); 
            Menu_Line3_Not_Highlighted(); 
            Menu_Line4_Highlighted();             
          }       
          break;
    }  
  }    
        
  //****************************************************************************************************************************************************************************
  //**************************************                       Code to draw up the menu                                *******************************************************
  //****************************************************************************************************************************************************************************
    
  void Draw_Menu_Blocks()
  {
    // Part above menu   
   
    GLCD.DrawRect( 0,  0, 127, 63);                   // draw an open rectangle that spans the extremties of the LCD
    
    GLCD.FillRect( 47,  2,   2,  8); 
    GLCD.DrawLine( 49,  2,  79,  2); 
    GLCD.DrawLine( 0, 12, 126, 12);  
    GLCD.DrawLine( 0, 51, 126, 51);         
  
    GLCD.DrawLine( 31, 52,  31, 62);                  // Lines to draw four blocks at the bottom.
    GLCD.DrawLine( 63, 52,  63, 62);
    GLCD.DrawLine( 94, 52,  94, 62); 
 
    GLCD.SelectFont(System5x7, BLACK);        
    GLCD.CursorToXY(8, 54);                           // Write ESC in the first block

    if( Button_States_Selectors_and_Detectors[11]  != 2 )
    {
      GLCD.print("   ");
    }

    else
    {    
      GLCD.print(String_Lookup_Table_Array[1]);   
    }
    
    GLCD.DrawLine( 47, 54,  47, 60);                  // Draw Up arrow in second block from left
    GLCD.DrawLine( 46, 55,  48, 55);
    GLCD.DrawLine( 45, 56,  49, 56);
    
    GLCD.DrawLine( 78, 54,  78, 60);                  //  Draw Down arrow in third block from left
    GLCD.DrawLine( 77, 59,  79, 59);
    GLCD.DrawLine( 76, 58,  80, 58);
  
    GLCD.CursorToXY(103, 54);                          // Write ENT in thte last block
    GLCD.print(String_Lookup_Table_Array[2]);   
  
  }
  
  //****************************************************************************************************************************************************************************
  //****************************                                            Read Button Inputs                                       *******************************************
  //****************************************************************************************************************************************************************************
  
  void Read_Buttons()
  {  
    Button_States_Selectors_and_Detectors[0]            = digitalRead(ESC_Button);
    Button_States_Selectors_and_Detectors[2]            = digitalRead(UP_Button);
    Button_States_Selectors_and_Detectors[4]            = digitalRead(DWN_Button);  
    Button_States_Selectors_and_Detectors[6]            = digitalRead(ENT_Button);        
  }

  //****************************************************************************************************************************************************************************
  //****************************     Test_ENT_ESC_Counter_Value to determine whether ENT or ESC was pressed to use in case function    *****************************************
  //****************************************************************************************************************************************************************************  
  
  void Test_ENT_ESC_Counter_Value ()
  {  
    Not_Highlighted_Escape_Button();
    Not_Highlighted_Enter_Button();
    
    // Can't use a external pull down resistor on this button, so you use the internal pull-up resistor and read for a digital low since when the button is not pushed, your getting in a digital high.
    
    if ( Button_States_Selectors_and_Detectors[0] != Button_States_Selectors_and_Detectors[1] && Button_States_Selectors_and_Detectors[0] == LOW )   
    { 
      delay(150);
      Button_States_Selectors_and_Detectors[11]--;
      Button_States_Selectors_and_Detectors[8]--;
      
      if ( Button_States_Selectors_and_Detectors[11] < 0 )
      {
        Button_States_Selectors_and_Detectors[11] = 0;
      }
    }
    
    Button_States_Selectors_and_Detectors[1] = Button_States_Selectors_and_Detectors[0];
  
    if ( Button_States_Selectors_and_Detectors[6] != Button_States_Selectors_and_Detectors[7] && Button_States_Selectors_and_Detectors[6] == HIGH )
    {
        delay(150); 
        Button_States_Selectors_and_Detectors[11]++;
        Button_States_Selectors_and_Detectors[8]++;
        
        if (Button_States_Selectors_and_Detectors[11] > 4 )
        {
          Button_States_Selectors_and_Detectors[11] = 4;
        }
    }
        
    Button_States_Selectors_and_Detectors[7] = Button_States_Selectors_and_Detectors[6];    
  }
 
  //****************************************************************************************************************************************************************************
  //****************************     Tests Main Menu Line Counter Value to determine which line in the menu should be highlighted    *******************************************
  //****************************************************************************************************************************************************************************
   
  void Test_UP_DOWN_Counter ()
  {
    Not_Highlighted_Up_Button();    
    Not_Highlighted_Down_Button();
    
    if ( Button_States_Selectors_and_Detectors[2] != Button_States_Selectors_and_Detectors[3] && Button_States_Selectors_and_Detectors[2] == LOW )
    {
        delay(150);
        Button_States_Selectors_and_Detectors[10]--;
        Button_States_Selectors_and_Detectors[9]++;
    }
    
    Button_States_Selectors_and_Detectors[3] = Button_States_Selectors_and_Detectors[2];
    
    if ( Button_States_Selectors_and_Detectors[4] != Button_States_Selectors_and_Detectors[5] && Button_States_Selectors_and_Detectors[4] == HIGH )
    {
        delay(150);      
        Button_States_Selectors_and_Detectors[10]++;
        Button_States_Selectors_and_Detectors[9]--;        
    }    
        
    Button_States_Selectors_and_Detectors[5] = Button_States_Selectors_and_Detectors[4]; 
        
  }
 
  //****************************************************************************************************************************************************************************
  //**************************************                         NOT Highlighted Buttons                               *******************************************************
  //****************************************************************************************************************************************************************************  
  
  void Not_Highlighted_Escape_Button() 
  {
    GLCD.SelectFont(System5x7, BLACK);      
    GLCD.CursorToXY(8, 54);                   
   
    if( Button_States_Selectors_and_Detectors[11]  != 2 )
    {
      GLCD.print("   ");
    }

    else
    {    
      GLCD.print(String_Lookup_Table_Array[1]);   
    }
  }
  
  void Not_Highlighted_Up_Button() 
  {
    
    GLCD.DrawRoundRect( 37, 53, 21, 8, 2,WHITE);  
    GLCD.FillRect( 38, 54, 19, 7,        WHITE );
    GLCD.DrawLine( 47, 54,  47, 60,      BLACK);  
    GLCD.DrawLine( 46, 55,  48, 55,      BLACK);
    GLCD.DrawLine( 45, 56,  49, 56,      BLACK);    
  }
  
  void Not_Highlighted_Down_Button()
  {
    
    GLCD.DrawRoundRect( 68, 53, 21, 8, 2,WHITE);  
    GLCD.FillRect( 69, 54, 19, 7 ,       WHITE);
    GLCD.DrawLine( 78, 54,  78, 60,      BLACK); 
    GLCD.DrawLine( 77, 59,  79, 59,      BLACK);
    GLCD.DrawLine( 76, 58,  80, 58,      BLACK);  
  }
  
  void Not_Highlighted_Enter_Button()
  {
    
    GLCD.SelectFont(System5x7, BLACK);      
    GLCD.CursorToXY(103, 54);               
    GLCD.print(String_Lookup_Table_Array[2]);  
  }
  
  //****************************************************************************************************************************************************************************
  //**************************************                         Menu lines highlighted                                *******************************************************
  //****************************************************************************************************************************************************************************


  void Menu_Line1_Highlighted()
  {
    GLCD.DrawLine  (  1, 14, 126, 14, BLACK);
    GLCD.FillRect  (  1, 15,   5,  7, BLACK);    
    GLCD.FillRect  (121, 15,   2,  7, BLACK);     
    GLCD.SelectFont(System5x7,     WHITE);                                                                       
    GLCD.CursorToXY( 7, 15 );
    GLCD.print("    ");                             
    GLCD.print(String_Lookup_Table_Array[10]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[14]);
    GLCD.print("    ");     
  }
  
  void Menu_Line2_Highlighted()
  {
    GLCD.DrawLine( 1, 23, 126, 23, BLACK);    
    GLCD.SelectFont(System5x7,     WHITE);                                                                       
    GLCD.CursorToXY( 1, 24 );
    GLCD.print("     ");                                
    GLCD.print(String_Lookup_Table_Array[11]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[15]);
    GLCD.print("    ");     
  }
  
  void Menu_Line3_Highlighted()
  {
    GLCD.DrawLine  (  1, 32, 126, 32, BLACK); 
    GLCD.FillRect  (  1, 33,   2,  7, BLACK); 
    GLCD.FillRect  (124, 33,   2,  7, BLACK);     
    GLCD.SelectFont(System5x7,        WHITE);                                                                       
    GLCD.CursorToXY( 4, 33 );
    GLCD.print("     ");                               
    GLCD.print(String_Lookup_Table_Array[12]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[15]);
    GLCD.print("    ");       
  }
  
  void Menu_Line4_Highlighted()  
  {
    GLCD.DrawLine  ( 1, 41, 126, 41, BLACK);
    GLCD.FillRect  (  1, 42,   2,  7, BLACK);    
    GLCD.FillRect  (124, 42,   2,  7, BLACK); 
    GLCD.SelectFont(System5x7,     WHITE);                                                                       
    GLCD.CursorToXY( 4, 42 );
    GLCD.print("   ");                                
    GLCD.print(String_Lookup_Table_Array[21]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[22]);
    GLCD.print("  ");       
    
  }
  
  
  //****************************************************************************************************************************************************************************
  //**************************************                         Menu lines NOT highlighted                            *******************************************************
  //****************************************************************************************************************************************************************************


  void Menu_Line1_Not_Highlighted()
  {
    GLCD.DrawLine( 1, 14, 126, 14, WHITE);
    GLCD.FillRect  (  1, 15,   5,  7, WHITE);    
    GLCD.FillRect  (124, 15,   2,  7, WHITE);     
    GLCD.SelectFont(System5x7,     BLACK);                                                                       
    GLCD.CursorToXY( 7, 15 );
    GLCD.print("    ");                               
    GLCD.print(String_Lookup_Table_Array[10]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[14]);
    GLCD.print("    "); 
  }
  
  void Menu_Line2_Not_Highlighted()
  {
    GLCD.DrawLine( 1, 23, 126, 23, WHITE);    
    GLCD.SelectFont(System5x7,     BLACK);                                                                       
    GLCD.CursorToXY( 1, 24 );
    GLCD.print("     ");                              
    GLCD.print(String_Lookup_Table_Array[11]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[15]);
    GLCD.print("    ");  
  }
  
  void Menu_Line3_Not_Highlighted()
  {
    GLCD.DrawLine  (  1, 32, 126, 32, WHITE);  
    GLCD.FillRect  (  1, 33,   2,  7, WHITE); 
    GLCD.FillRect  (124, 33,   2,  7, WHITE);    
    GLCD.SelectFont(System5x7,        BLACK);   
    GLCD.CursorToXY( 4, 33 );
    GLCD.print("     ");                               
    GLCD.print(String_Lookup_Table_Array[12]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[15]);
    GLCD.print("    ");       

  }
  
  void Menu_Line4_Not_Highlighted()  
  {
    GLCD.DrawLine  (  1, 41, 126, 41, WHITE);
    GLCD.FillRect  (  1, 42,   2,  7, WHITE);    
    GLCD.FillRect  (124, 42,   2,  7, WHITE); 
    GLCD.SelectFont(System5x7       , BLACK);                                                                       
    GLCD.CursorToXY( 4, 42 );
    GLCD.print("   ");                              
    GLCD.print(String_Lookup_Table_Array[21]);
    GLCD.print(" ");
    GLCD.print(String_Lookup_Table_Array[22]);
    GLCD.print("  ");    
  }

  //****************************************************************************************************************************************************************************
  //**************************************             Displays the Running Cycle of the Reflow Cycle                   ********************************************************
  //****************************************************************************************************************************************************************************

  void RunningCycleScreen()
  {    
    int PREH_Keep_SEC_Run;
    int PREH_Keep_MIN_Run;
      
    int HEAT_Keep_SEC_Run;
    int HEAT_Keep_MIN_Run;
      
    int SLDR_Keep_SEC_Run;
    int SLDR_Keep_MIN_Run;
      
    int KEEP_SEC = 0;
    int COOL_SEC = 0;
    
    GLCD.DrawRect( 0,  0, 127, 63);                                // draw an open rectangle that spans the extremties of the LCD
    
    Status  = String_Lookup_Table_Array[3];

    Setpoint = Stage_Temperature_Setpoints[0] + PREH_Hysteresis;

    Raw_Value = analogRead(sensorPin);  

    Averaging_RAW_Value();
    
    Temp = Raw_Value_Avg / 2.04;
    
    digitalWrite(RelayPin, LOW);                                   //  Sets relay outputs low to double check that the relay switches are off
    digitalWrite(FanPin, LOW);     

    windowStartTime = millis();
    
    //tell the PID to range between 0 and the full window size
    myPID.SetOutputLimits(0, WindowSize);
    
    //turn the PID on
    myPID.SetMode(AUTOMATIC);   
   
    
    //*************************************************               Upper Graph drawing functions          ********************************************************************     


    GLCD.FillRect( 3,  2,   2,  8);  
    GLCD.DrawLine( 6,  2,  47,  2);  
    GLCD.FillRect(78,  0,   2, 11);  
    GLCD.DrawLine( 0, 12, 126, 12);  


    //*************************************************              Graph Structure drawing functions        ********************************************************************   

    Reference_Graph();     
  
    //*************************************************             Onder die graph drawing functions        ********************************************************************   

    GLCD.FillRect( 3, 53,   2,  8);  
    GLCD.DrawLine( 6, 53,  35, 53);  
    GLCD.FillRect(71, 51,   2, 11);  
    GLCD.DrawLine( 0, 51, 126, 51);  
 
    GLCD.SelectFont(System5x7, WHITE);                                                                       
    GLCD.CursorToXY(6, 3);
    GLCD.print("STATUS:");                                                                                   // Status Header
   
    GLCD.SelectFont(System5x7, BLACK);
    GLCD.CursorToXY(51, 3);
    GLCD.print(Status);                                                                                      // Print status or reflow cycle

    GLCD.CursorToXY(84, 3);  
    GLCD.print("T:");

    Temp_Printing();
    
    
    GLCD.DrawCircle(GLCD.Width/2 +53,GLCD.CenterY- 28, min(GLCD.CenterX/2 + 51, GLCD.CenterY)-31);           // Draws a little circle to be the Degree symbol
  
    GLCD.CursorToXY(GLCD.Width/2 +56, 3);                                                                    // Print unit of measurent
    GLCD.print("C");  
  
    //********************************************************                        Printing intial TIME                        *******************************************************************************   
    
    GLCD.SelectFont(System5x7, WHITE);                                                                       // Print time header
    GLCD.CursorToXY(GLCD.Width/2 - 58, 54);
    GLCD.print("TIME:");

    GLCD.SelectFont(System5x7, BLACK);    
    GLCD.CursorToXY(51, 54);
    GLCD.print(":");       
     
    ENT_Through_Status = 1; 
    
    PREH_Keep_MIN_Run = Button_States_Selectors_and_Detectors[12];  
    PREH_Keep_SEC_Run = Stage_Temperature_Setpoints[6]; 

    HEAT_Keep_MIN_Run = Button_States_Selectors_and_Detectors[13]; 
    HEAT_Keep_SEC_Run = Stage_Temperature_Setpoints[7]; 

    SLDR_Keep_MIN_Run = Button_States_Selectors_and_Detectors[14]; 
    SLDR_Keep_SEC_Run = Stage_Temperature_Setpoints[8];     
    
    KEEP_SEC = 0;
    COOL_SEC = 0;      
    Minutes = 0;

    Temp_Limit1              =   0;            //  These values are set back to their defaults so that the graph can plot correctly when the program is run again.
    Temp_Limit2              =  15;   
    XY_Coordinates[0]        =   6;         
    XY_Coordinates[1]        =  48;
    
    
    //***********************************************************************************************************************************************************************************************************  
    // If you enter the run program function then you wouldn't have tested if seconds are >= 60 so as to print the time in minutes, so here you retest it if the user goes straigt into running the program.
    //***********************************************************************************************************************************************************************************************************

    Minutes = PREH_Keep_MIN_Run;    
    Seconds = PREH_Keep_SEC_Run; 

    Time_Printing();                        
           
    PID_Calculations();                                                      // Call the PID Calculation function.   

    while ( Temp < Stage_Temperature_Setpoints[0] )     // Entering the PREH RAMP Stage of the process                                                     
    {
      Setpoint = Stage_Temperature_Setpoints[0] + PREH_Hysteresis;
      
      //turn the PID on
      myPID.SetMode(AUTOMATIC);   
    
      Raw_Value = analogRead(sensorPin);  

      Averaging_RAW_Value();
    
      Temp = Raw_Value_Avg / 2.04;
        
      Temp_Printing();
      
//********************************************************************************************************************************************************************************************

      Ramp_Temp_Range = Stage_Temperature_Setpoints[0];  // From the starting point we can't get a negative temperature so PREH will always be larger than 0 or a positive value so we don't have to test it and we can just call the Ramp Up Validator function.
     
      Temp_Ramp_Up_Graph_Validator ( 0 , Stage_Temperature_Setpoints[0], PREH_Pixals);

//********************************************************************************************************************************************************************************************
     
   }

    if ( Temp >= Stage_Temperature_Setpoints[0] )     // Entering the PREH KEEP Stage of the process                                                     
    {      
      Increment_Detector = 1;
      Loop_Through_Detector = 1;
      
      StartofTimer_Seconds = millis();                                  //  Set timers to millis again so that timing can be accurate
      StartofTimer_Graphing_Interval = millis(); 
     
      if (  XY_Coordinates[0] < 22 )
      {
        XY_Coordinates[0] = 22;
      }
      
      double kp = PID_KP_HEAT;
      double ki = PID_KI_HEAT;
      double kd = PID_KD_HEAT;
      
     while( ENT_Through_Status == 1)    
     {            
       Raw_Value = analogRead(sensorPin);  
    
       Averaging_RAW_Value();
      
       Temp = Raw_Value_Avg / 2.04;       

       Setpoint = Stage_Temperature_Setpoints[0] + Temp_Offset;
       
       PID_Calculations();  

//***********************************************************************************************************************************************************************************************************

       Convert_Minutes_to_Seconds();

       Temp_Constant_Graph_Validator ( Stage_Temperature_Setpoints[6], PREH_Keep_Pixals );

//***********************************************************************************************************************************************************************************************************                   

       if( Minutes >= 0 && Seconds >= -1 && Second_Detector == 1 )
       {                    
         Seconds--;
        
         if ( Minutes != 0 )
         {
           while (Seconds < 0)  
           {
             Minutes--;           
             Seconds = Seconds + 60;
            }
          }           
       }           
      
       Temp_Printing();
       Time_Printing();                  

       if( Minutes == 0 && Seconds <= 0 )                //  Setting Increment Variable back to 1 so that the ramp function can run again.
       {   
           Increment_Detector = 1;
           Loop_Through_Detector = 1;
           
           if (  XY_Coordinates[0] < 37 )
           {
             XY_Coordinates[0] = 37;
           }           
       }    
             
       while( Minutes == 0 && Seconds <= 0 )             // Entering the SOAK RAMP Stage of the process                                                                                                   
       {         
         Status = String_Lookup_Table_Array[4];
         GLCD.CursorToXY(51, 3);
         GLCD.print(Status); 
         
         Raw_Value = analogRead(sensorPin);  

         Averaging_RAW_Value();
    
         Temp = Raw_Value_Avg / 2.04;
    
         Setpoint = Stage_Temperature_Setpoints[1] + HEAT_Hysteresis;         

         PID_Calculations(); // Call the PID Calculation function.     
         
         //********************************************************************************************************************************************************************************************

         // For stages after the PREH stage the following procedure must be implemented since it is assumed that the standard reflow curve is followed i.e. PREH < HEAT < SLDR > KEEP > COOL > DONE. 
         // For this reason the Ramp_Temp_Range is tested to see whether is has a negative or a positive value. If it negative, it is an indication that we're ramping downwards so we call the 
         // Ramp Down Validator function and set the Ramp_Temp_Range back to a positive value. If not we call the Ramp Up Validator function.
      
         Ramp_Temp_Range = Stage_Temperature_Setpoints[1] - Stage_Temperature_Setpoints[0];
        
         if ( Ramp_Temp_Range >= 0 )                                                                            
         {
           Temp_Ramp_Up_Graph_Validator ( Stage_Temperature_Setpoints[0], Stage_Temperature_Setpoints[1], HEAT_Pixals );           
         }

         else         
         {
           Ramp_Temp_Range = Ramp_Temp_Range * (-1);                                     
           Temp_Ramp_Down_Graph_Validator ( Stage_Temperature_Setpoints[0], Stage_Temperature_Setpoints[1], HEAT_Pixals );         //  Call the Ramp Down Validator Function. We call it during the ramping stage only so as to not interfere with the time dependent graphing validator.
         }            
         

        //********************************************************************************************************************************************************************************************

         Temp_Printing();
         Time_Printing();                      
                       
         //***********************************************************************************************************************************************************************************************************
         
         if ( Temp >= Stage_Temperature_Setpoints[1] )      // Entering the SOAK KEEP Stage of the process                                                     
         {           
           ENT_Through_Status++;  
           
           if (  XY_Coordinates[0] < 45 )
           {
             XY_Coordinates[0] = 45;
           }         
  
           Increment_Detector = 1;           
           
           StartofTimer_Seconds = millis();                              //  Set timers to millis again so that timing can be accurate
           StartofTimer_Graphing_Interval = millis();  
           
           double kp = PID_KP_SLDR;
           double ki = PID_KI_SLDR;
           double kd = PID_KD_SLDR;
           
           while ( ENT_Through_Status == 2 )
           { 
             Minutes  = HEAT_Keep_MIN_Run;    
             Seconds  = HEAT_Keep_SEC_Run;                                            
           
             Raw_Value = analogRead(sensorPin);  
        
             Averaging_RAW_Value();
            
             Temp = Raw_Value_Avg / 2.04;
            
             Setpoint = Stage_Temperature_Setpoints[1] + Temp_Offset;        

             PID_Calculations();  // Call the PID Calculation function.

//***********************************************************************************************************************************************************************************************************

             Convert_Minutes_to_Seconds();
      
             Temp_Constant_Graph_Validator ( Stage_Temperature_Setpoints[7], HEAT_Keep_Pixals );      

//***********************************************************************************************************************************************************************************************************

             Temp_Printing();
             Time_Printing();
             
             if( Minutes >= 0 && Seconds >= -1 && Second_Detector == 1 )
             {                    
               HEAT_Keep_SEC_Run--;

               if ( HEAT_Keep_MIN_Run != 0 )
               {
                 while (HEAT_Keep_SEC_Run < 0)  
                 {
                   HEAT_Keep_MIN_Run--;           
                   HEAT_Keep_SEC_Run = HEAT_Keep_SEC_Run + 60;
                 }
               }               
             }            
             
             if( Minutes == 0 && Seconds <= 0 )                //  Setting Increment Variable back to 1 so that the ramp function can run again.
             {   
                 Increment_Detector = 1;
                 Loop_Through_Detector = 1;
                 
                 if (  XY_Coordinates[0] < 61 )
                 {
                   XY_Coordinates[0] = 61;
                 }                        
             }                
                       
             while( Minutes == 0 && Seconds <= 0 )     // Entering the SLDR RAMP Stage of the process                                                     
             {
         
               Status  = String_Lookup_Table_Array[5];
               GLCD.CursorToXY(51, 3);
               GLCD.print(Status); 
         
               Raw_Value = analogRead(sensorPin);  
          
               Averaging_RAW_Value();
              
               Temp = Raw_Value_Avg / 2.04;             
   
               Setpoint = Stage_Temperature_Setpoints[2] + SLDR_Hysteresis;               

             //********************************************************************************************************************************************************************************************

             // For stages after the PREH stage the following procedure must be implemented since it is assumed that the standard reflow curve is followed i.e. PREH < HEAT < SLDR > KEEP > COOL > DONE. 
             // For this reason the Ramp_Temp_Range is tested to see whether is has a negative or a positive value. If it negative, it is an indication that we're ramping downwards so we call the 
             // Ramp Down Validator function and set the Ramp_Temp_Range back to a positive value. If not we call the Ramp Up Validator function.
        
             Ramp_Temp_Range = Stage_Temperature_Setpoints[2] - Stage_Temperature_Setpoints[1];
          
             if ( Ramp_Temp_Range < 0 )                                                      
             {
               Ramp_Temp_Range = Ramp_Temp_Range * (-1);                                     
               Temp_Ramp_Down_Graph_Validator ( Stage_Temperature_Setpoints[1], Stage_Temperature_Setpoints[2], SLDR_Pixals );         //  Call the Ramp Down Validator Function. We call it during the ramping stage only so as to not interfere with the time dependent graphing validator.
             }
          
             else                                                                            
             {
               Temp_Ramp_Up_Graph_Validator ( Stage_Temperature_Setpoints[1], Stage_Temperature_Setpoints[2], SLDR_Pixals );    
             }


             //********************************************************************************************************************************************************************************************


               PID_Calculations();  // Call the PID Calculation function.               
                
               Temp_Printing();
               Time_Printing();
         
               //***********************************************************************************************************************************************************************************************************
         
               if ( Temp >= Stage_Temperature_Setpoints[2] )     // Entering the SLDR KEEP Stage of the process                                                     
               {
                 ENT_Through_Status++;
                 Increment_Detector = 1;
                 Loop_Through_Detector = 1;                 
                 
                 StartofTimer_Seconds = millis();                      //  Set timers to millis again so that timing can be accurate
                 StartofTimer_Graphing_Interval = millis();     
                 
                 if (  XY_Coordinates[0] < 70 )
                 {
                   XY_Coordinates[0] = 70;
                 }                        
           
                 while ( ENT_Through_Status == 3 )
                 {
                   Minutes  = SLDR_Keep_MIN_Run;    
                   Seconds  = SLDR_Keep_SEC_Run;               
                             
                   Temp_Printing();
                   Time_Printing();
           
                   Raw_Value = analogRead(sensorPin);  

                   Averaging_RAW_Value();
    
                   Temp = Raw_Value_Avg / 2.04;
    
                   Setpoint = Stage_Temperature_Setpoints[2] + Temp_Offset;               

                   PID_Calculations();  // Call the PID Calculation function. 
                   
                   //***********************************************************************************************************************************************************************************************************

                   Convert_Minutes_to_Seconds();
            
                   Temp_Constant_Graph_Validator ( Stage_Temperature_Setpoints[8], SLDR_Keep_Pixals  );
            
                   Convert_Seconds_to_Minutes();       

                   //***********************************************************************************************************************************************************************************************************

                   if( Minutes >= 0 && Seconds >= -1 && Second_Detector == 1 )
                   {                    
                     SLDR_Keep_SEC_Run--;
                          
                     if ( SLDR_Keep_MIN_Run != 0 )
                     {
                       while (SLDR_Keep_SEC_Run < 0)  
                       {
                         SLDR_Keep_MIN_Run--;           
                         SLDR_Keep_SEC_Run = SLDR_Keep_SEC_Run + 60;
                       }
                     }
                   }                   
                   
                  //***********************************************************************************************************************************************************************************************************

                   if( Minutes == 0 && Seconds <= 0 )    // Entering the KEEP Stage of the process
                   {
                     Increment_Detector = 1;   
                     Loop_Through_Detector = 1;                                                
                   }                   
                   
                   while( Minutes == 0 && Seconds <= 0 )                                                      
                   {                          
                       while ( Temp >= Stage_Temperature_Setpoints[3] )
                        {
                         Raw_Value = analogRead(sensorPin);  

                         Averaging_RAW_Value();
    
                         Temp = Raw_Value_Avg / 2.04;                              

                         Setpoint = 0;

                         //********************************************************************************************************************************************************************************************

                         // For stages after the PREH stage the following procedure must be implemented since it is assumed that the standard reflow curve is followed i.e. PREH < HEAT < SLDR > KEEP > COOL > DONE. 
                         // For this reason the Ramp_Temp_Range is tested to see whether is has a negative or a positive value. If it negative, it is an indication that we're ramping downwards so we call the 
                         // Ramp Down Validator function and set the Ramp_Temp_Range back to a positive value. If not we call the Ramp Up Validator function.
                      
                         Ramp_Temp_Range = Stage_Temperature_Setpoints[3] - Stage_Temperature_Setpoints[2];
                        
                         if ( Ramp_Temp_Range < 0 )                                                      
                         {
                           Ramp_Temp_Range = Ramp_Temp_Range * (-1);                                     
                           Temp_Ramp_Down_Graph_Validator ( Stage_Temperature_Setpoints[2], Stage_Temperature_Setpoints[3], KEEP_Pixals );         //  Call the Ramp Down Validator Function. We call it during the ramping stage only so as to not interfere with the time dependent graphing validator.
                         }
                        
                         else                                                                            
                         {
                           Temp_Ramp_Up_Graph_Validator ( Stage_Temperature_Setpoints[2], Stage_Temperature_Setpoints[3], KEEP_Pixals );           
                         }     
            
                         delay(1000);                         

                         //********************************************************************************************************************************************************************************************
                         
                         PID_Calculations();  // Call the PID Calculation function.                      
                         
                         Status  = String_Lookup_Table_Array[6];
                         GLCD.CursorToXY(51, 3);
                         GLCD.print(Status);  
                                                   
                         Minutes  = 0;       
                          
                         Seconds  = KEEP_SEC;
                         KEEP_SEC++;
                                                                                   
                         Temp_Printing();
                         Time_Printing();
                      
                        }                    
                        
                        if( Temp >= Stage_Temperature_Setpoints[4] )    // Entering the COOL Stage of the process
                        {
                          Increment_Detector = 1;    
                        }                        
                        
                        while ( Temp >= Stage_Temperature_Setpoints[4] )
                        {                          
                          Raw_Value = analogRead(sensorPin);  
                      
                          Averaging_RAW_Value();
                          
                          Temp = Raw_Value_Avg / 2.04;

                          if ( Fan_Status == 1 )
                          {
                            digitalWrite(FanPin, HIGH);
                          }
                          
                          //********************************************************************************************************************************************************************************************

                          // For stages after the PREH stage the following procedure must be implemented since it is assumed that the standard reflow curve is followed i.e. PREH < HEAT < SLDR > KEEP > COOL > DONE. 
                          // For this reason the Ramp_Temp_Range is tested to see whether is has a negative or a positive value. If it negative, it is an indication that we're ramping downwards so we call the 
                          // Ramp Down Validator function and set the Ramp_Temp_Range back to a positive value. If not we call the Ramp Up Validator function.
                      
                          Ramp_Temp_Range = Stage_Temperature_Setpoints[4] - Stage_Temperature_Setpoints[3];
                        
                          if ( Ramp_Temp_Range < 0 )                                                      
                          {
                            Ramp_Temp_Range = Ramp_Temp_Range * (-1);                                     
                            Temp_Ramp_Down_Graph_Validator ( Stage_Temperature_Setpoints[3], Stage_Temperature_Setpoints[4], COOL_Pixals );         //  Call the Ramp Down Validator Function. We call it during the ramping stage only so as to not interfere with the time dependent graphing validator.
                          }
                        
                          else                                                                            
                          {
                            Temp_Ramp_Up_Graph_Validator ( Stage_Temperature_Setpoints[3], Stage_Temperature_Setpoints[4], COOL_Pixals );           
                          }             
               
                          delay(1000);                          


                          //********************************************************************************************************************************************************************************************    
 
                          Status  = String_Lookup_Table_Array[7];
                          GLCD.CursorToXY(51, 3);
                          GLCD.print(Status);                                         
                                  
                          Minutes  = 0;    
                                                  
                          Seconds  = COOL_SEC;      
                          COOL_SEC++;
                                                                
                          Temp_Printing();
                          Time_Printing();

                          if( Temp >= 0 && Temp < Stage_Temperature_Setpoints[4] )    // Entering the DONE Stage of the process
                          {
                            Increment_Detector = 1;    
                          }     
      
                          while ( Temp >= 0 && Temp < Stage_Temperature_Setpoints[4] )
                          {                                                            
                            Raw_Value = analogRead(sensorPin);  
                        
                            Averaging_RAW_Value();
                            
                            Temp = Raw_Value_Avg / 2.04;
                            
                            digitalWrite(FanPin, LOW);

//********************************************************************************************************************************************************************************************

                            // For stages after the PREH stage the following procedure must be implemented since it is assumed that the standard reflow curve is followed i.e. PREH < HEAT < SLDR > KEEP > COOL > DONE. 
                            // For this reason the Ramp_Temp_Range is tested to see whether is has a negative or a positive value. If it negative, it is an indication that we're ramping downwards so we call the 
                            // Ramp Down Validator function and set the Ramp_Temp_Range back to a positive value. If not we call the Ramp Up Validator function.
                      
                            Ramp_Temp_Range = Stage_Temperature_Setpoints[5] - Stage_Temperature_Setpoints[4];
                        
                            if ( Ramp_Temp_Range < 0 )                                                      
                            {
                              Ramp_Temp_Range = Ramp_Temp_Range * (-1);                                     
                              Temp_Ramp_Down_Graph_Validator ( Stage_Temperature_Setpoints[4], Stage_Temperature_Setpoints[5], DONE_Pixals );         //  Call the Ramp Down Validator Function. We call it during the ramping stage only so as to not interfere with the time dependent graphing validator.
                            }
                        
                            else                                                                            
                            {
                              Temp_Ramp_Up_Graph_Validator ( Stage_Temperature_Setpoints[4], Stage_Temperature_Setpoints[5], DONE_Pixals );           
                            }      
        
                            delay(1000);                  

//********************************************************************************************************************************************************************************************

                            Status  = String_Lookup_Table_Array[8];
                            GLCD.CursorToXY(51, 3);
                            GLCD.print(Status);  
                                                               
                            Minutes  = 0;   
                            
                            Seconds  = COOL_SEC;
                            COOL_SEC++;
                                                                
                            Temp_Printing();
                            Time_Printing();
                            
                            
                            if (Temp <= Reflow_Program_Cutout_Temp || Minutes >= Reflow_Program_Cutout_Time )                          // Predefined cut-off time where unit exits back to the main menu again.
                            {
                              Button_States_Selectors_and_Detectors[11]  = 1;                                                          // Paramaters that needs to be set so that all the while loops can be exited
                              Button_States_Selectors_and_Detectors[10] = 1;
                              ENT_Through_Status = 0;           
                              Temp = -10;                                                                                              // Sets the temperature to a value below zero so that the while can be exited
                              Increment_Detector = 1;
                              
                              double kp = PID_KP_PREH;
                              double ki = PID_KI_PREH;
                              double kd = PID_KD_PREH;                              
                            }                                                     
                          }                           
                   }
                 }
               }                             
              }
             }
           }
        }     
      }     
      
    }
    }
  }  

  //****************************************************************************************************************************************************************************
  //**************************************************                        Averaging Temp Reading                    ********************************************************
  //**************************************************************************************************************************************************************************** 
    
  void Averaging_RAW_Value()
  {     
    // subtract the last reading:
    total= total - readings[index];               

    readings[index] = Raw_Value;

    // add the reading to the total:
    total= total + readings[index];       
    // advance to the next position in the array:  
    index = index + 1;                    
  
    // if we're at the end of the array...
    if (index >= numReadings)              
    // ...wrap around to the beginning: 
    index = 0;                           
  
    // calculate the average:
    Raw_Value_Avg = total / numReadings;         
    // send it to the computer as ASCII digits    
  }   
  
//****************************************************************************************************************************************************************************
//**************************************************                     Reference Graphing function                 *********************************************************
//****************************************************************************************************************************************************************************  


  void Reference_Graph()      
  { 
  
    Temp_Limit1              =   0;            //  These values are set back to their defaults so that the graph can plot correctly when the program is run again.
    Temp_Limit2              =  15;   
    XY_Coordinates[0]                        =   5;
    XY_Coordinates[1]                        =  48;
      
    
    //*************************************************              Graph Structure drawing functions        ********************************************************************   
  
    GLCD.DrawLine( 4, 48, 122, 48);      // Graph X line
  
    GLCD.DrawLine( 4, 15,   4, 48);      // Graph Y line
  
    GLCD.DrawLine( 4, 16,   6, 16);      // Graph indicator line - 260 Deg C
    GLCD.DrawLine( 4, 21,   5, 21);      // Graph indicator line - 235 Deg C
  
    GLCD.DrawLine( 4, 26,   6, 26);      // Graph indicator line - 210 DEG C
    GLCD.DrawLine( 4, 32,   5, 32);      // Graph indicator line - 180 DEG C
  
    GLCD.DrawLine( 4, 38,   6, 38);      // Graph indicator line - 150 DEG C
    GLCD.DrawLine( 4, 43,   5, 43);      // Graph indicator line - 075 DEG C    
  
    Temp = Stage_Temperature_Setpoints[0];
    Y_Pixel_Calculator();
    XY_Coordinates[6] = XY_Coordinates[1];
  
    Temp = Stage_Temperature_Setpoints[1];
    Y_Pixel_Calculator();    
    XY_Coordinates[9] = XY_Coordinates[1];
  
    Temp = Stage_Temperature_Setpoints[2];
    Y_Pixel_Calculator();    
    XY_Coordinates[12] = XY_Coordinates[1];  
  
    Temp = Stage_Temperature_Setpoints[3];
    Y_Pixel_Calculator();    
    XY_Coordinates[14] = XY_Coordinates[1];   
  
    Temp = Stage_Temperature_Setpoints[4];
    Y_Pixel_Calculator();  
    XY_Coordinates[16] = XY_Coordinates[1];  
  
    GLCD.DrawLine(   XY_Coordinates[2]   , XY_Coordinates[3],  XY_Coordinates[4]   ,  XY_Coordinates[6]);  // PREH Line
    GLCD.DrawLine(   XY_Coordinates[4]   , XY_Coordinates[6],  XY_Coordinates[5]   ,  XY_Coordinates[6]);  // PREH KEEP Time Line
  
    GLCD.DrawLine(   XY_Coordinates[5]   , XY_Coordinates[6],  XY_Coordinates[7]   ,  XY_Coordinates[9]);  // HEAT Line
    GLCD.DrawLine(   XY_Coordinates[7]   , XY_Coordinates[9],  XY_Coordinates[8]   ,  XY_Coordinates[9]);  // HEAT KEEP Time Line
  
    GLCD.DrawLine(   XY_Coordinates[8]   , XY_Coordinates[9],  XY_Coordinates[10]   ,  XY_Coordinates[12]);  // SLDR Line
    GLCD.DrawLine(   XY_Coordinates[10]  , XY_Coordinates[12],  XY_Coordinates[11]  ,  XY_Coordinates[12]);  // SLDR KEEP Time line
  
    GLCD.DrawLine(   XY_Coordinates[11]  , XY_Coordinates[12],  XY_Coordinates[13]  ,  XY_Coordinates[14]);  // KEEP Line
    GLCD.DrawLine(   XY_Coordinates[13]  , XY_Coordinates[14],  XY_Coordinates[15]  ,  XY_Coordinates[16]);  // COOL line    
  
    GLCD.DrawLine(   XY_Coordinates[15]  , XY_Coordinates[16],  XY_Coordinates[17]  ,  XY_Coordinates[18]);     // DONE line       
  
    XY_Coordinates[1] = 48; 
    Temp = 15;
   
  }

//****************************************************************************************************************************************************************************   
//**************************************************                          Y Pixel Calculator                      ********************************************************
//****************************************************************************************************************************************************************************  

  void Y_Pixel_Calculator() 
  {      
    while ( Temp > Temp_Limit1 )
    {   
      if ( Temp_Limit1 <= 150 ); 
      {
        Temp_Step = 15 ;
      }
  
      if ( Temp > 150 && Temp_Limit1 >= 150)
      {
        Temp_Step = 5;
      }       
  
      XY_Coordinates[1]--;    
  
      Temp_Limit1 = Temp_Limit1 + Temp_Step;
      Temp_Limit2 = Temp_Limit2 + Temp_Step;
  
      if ( XY_Coordinates[1] <= 15 )    
      {
        XY_Coordinates[1] = 15; 
      }      
    }
  
    //****************************************************************************************************************************************************************************
  
    while ( Temp < Temp_Limit1 )
    {       
  
      if (  Temp <= 150 && Temp_Limit1 <= 150 ); 
      {
        Temp_Step = 15 ;
      }        
  
      if ( Temp_Limit1 >= 150)
      {
        Temp_Step = 5;
      }       
  
      XY_Coordinates[1]++;     
  
      Temp_Limit1 = Temp_Limit1 - Temp_Step;
      Temp_Limit2 = Temp_Limit2 - Temp_Step;
  
      if ( XY_Coordinates[1] >= 48 )    
      {
        XY_Coordinates[1] = 48; 
      }       
    }     
}

//****************************************************************************************************************************************************************************
//****************************************                      Graphing function qualifier during ramp stages                ************************************************
//****************************************************************************************************************************************************************************  

  void Temp_Ramp_Up_Graph_Validator ( int Temp_Setpoint_Old, int Temp_Setpoint_New, int Pixals )
  { 
    Increment_Variable = Ramp_Temp_Range / Pixals ;                        // Devides the temp travel range by the amount of pixals to get an incremental amount which will be incremented and looped by the amount of pixals available.
    
    if ( Temp >= ( Temp_Setpoint_Old + ( Increment_Variable * Increment_Detector )))            // Detects whether the temperature is bigger than Increment_Variable x N Increment_Detector to determine whether to plot or not. This is to stay within the graph limits.
    {
      Graphing_Function();    
      Increment_Detector++;                                              // Increment the Increment_Detector so that the limit value will be moved on.
        
      if ( Increment_Detector >= Pixals )                                // Test whether the graph limit have been exceeded or not.
      {
        Increment_Variable = Temp_Setpoint_New;
      }
    }
  }

//************************************************************************************************************************************************************************************************************************************************************************

  void Temp_Ramp_Down_Graph_Validator ( int Temp_Setpoint_Old, int Temp_Setpoint_New, int Pixals )
  {    
    Increment_Variable = Ramp_Temp_Range / Pixals ;                                            // Devides the value set for PREH by the amount of available pixals to get the temp increment limits.    
    
    if ( Temp <= ( Temp_Setpoint_Old - ( Increment_Variable * Increment_Detector )))           // Detects whether the temperature is bigger than Increment_Variable x N Increment_Detector to determine whether to plot or not. This is to stay within the graph limits.
    {
      Graphing_Function();
      
      Increment_Detector++;                                                                    // Increment the Increment_Detector so that the limit value will be moved on.
        
      if ( Increment_Detector >= Pixals )                                                      // Test whether the graph limit have been exceeded or not.
      {
        Increment_Variable = Temp_Setpoint_New;    
      }
    }
  }
  
//****************************************************************************************************************************************************************************
//***********************************                      Graphing function qualifier during constant temp stages                ********************************************
//****************************************************************************************************************************************************************************  

  void Temp_Constant_Graph_Validator ( long Time_Range, int KEEP_Pixals )               //  The Time range needs to be declared as a long to store a bigger value (4 byte value) since it will be multiplied by 1000.
  { 
    
    int Void_Time_Elapsed;                                                              //  Variable to store the total time that have elapsed in the constant temp function.
   
    Second_Detector = 0;                                                                //  Set the Second_Detector back to zero each time the loop loops through
  
    //****************************************************************************************************************************************************************************  
    Time_Range *= 1000;                                                                 //  Multiply Time_Range with 1000 to change the increment unit of measurement from seconds to milliseconds to prevent working with a float and rather with an integer.
  
    if ( Loop_Through_Detector == 1 )
    {
      Increment_Variable = ( Time_Range / KEEP_Pixals );                                //  Devides the time range by the amount of pixals to get an incremental time amount which will be incremented and looped by the amount of pixals available.                                                                                      
      Loop_Through_Detector = 0;
    }
    
    //****************************************************************************************************************************************************************************  
   
    if ( Increment_Detector <= KEEP_Pixals )
    {
      if ( millis() - StartofTimer_Graphing_Interval >= Increment_Variable )                //  Test whether millis() - StartifTimer_Graphing_Interval is greater than Increment_Variable or not, if so call the Graphing_Function
      {
        Graphing_Function();                                                                //  We call the graphing function so that we can graph each time the loop is looped through.
        StartofTimer_Graphing_Interval += Increment_Variable;                               //  We increment the timer with the test value i.e. Increment_Variable so that we will be able to detect it again.
        Increment_Detector++;
      }
    }
   //**************************************************************************************************************************************************************************** 
    if ( Seconds > 0 )
    {
      if ( millis() - StartofTimer_Seconds >= 1000 )                                       //  Test whether millis() - StartifTimer_Seconds is greater than 1000 or not, if so make Second_Detecto 1 so that we can decrement seconds.
      {
        StartofTimer_Seconds += 1000;
        Second_Detector = 1;                                                               //  We call the graphing function so that we can graph each time the loop is looped through.
      }
    }
  }

//****************************************************************************************************************************************************************************
//**************************************************                          Graphing function                      *********************************************************
//****************************************************************************************************************************************************************************  

  void Graphing_Function()
  {      
    Graphing_Downwards();
    Graphing_Upwards();
    Graphing_Forward();
  
    //****************************************************************************************************************************************************************************
    //***********************************                     Testing whether X have exceeded the horizontal endpoint of the graph                 *******************************
    //****************************************************************************************************************************************************************************  
  
    XY_Coordinates[0]++;                                                                  // X Value is moving too fast for the process.
  
    if ( XY_Coordinates[0] >= 120 )
    {
      XY_Coordinates[0] = 120;
    }
  
  }
 
//****************************************************************************************************************************************************************************
//**************************************************                  Graphing a lowering temperature                *********************************************************
//****************************************************************************************************************************************************************************  
  
  void Graphing_Downwards()
  {
    if ( Temp < Temp_Limit1 )
    {
  
      if ( Temp_Limit1 <= 150 ); 
      {
        Temp_Step = 15 ;
      }
  
      if ( Temp > 150 && Temp_Limit1 >= 150)
      {
        Temp_Step = 5;
      }           
  
      XY_Coordinates[1]++;        
  
      Temp_Limit1 = Temp_Limit1 - Temp_Step;
      Temp_Limit2 = Temp_Limit2 - Temp_Step;
  
      if ( Temp_Limit1 < 0 )
      {
        Temp_Limit1 =  0; 
        Temp_Limit2 = 15;   
      }
  
      if ( XY_Coordinates[1] >= 48 )    
      {
        XY_Coordinates[1] = 48; 
      }  
  
      GLCD.DrawLine(   XY_Coordinates[0],  48,   XY_Coordinates[0],  XY_Coordinates[1], BLACK);   
    }
  }

  //****************************************************************************************************************************************************************************
  //**************************************************                   Graphing a rising temperature                 *********************************************************
  //****************************************************************************************************************************************************************************  

  void Graphing_Upwards()
  {
    if ( Temp > Temp_Limit1 )
    {
  
      if (  Temp <= 150 && Temp_Limit1 <= 150 ); 
      {
        Temp_Step = 15 ;
      }        
  
      if ( Temp_Limit1 >= 150)
      {
        Temp_Step = 5;
      }        
  
      XY_Coordinates[1]--;        
  
      Temp_Limit1 = Temp_Limit1 + Temp_Step;
      Temp_Limit2 = Temp_Limit2 + Temp_Step;
  
      if ( XY_Coordinates[1] <= 15 )    
      {
        XY_Coordinates[1] = 15; 
      }
      GLCD.DrawLine(   XY_Coordinates[0],  48,   XY_Coordinates[0],  XY_Coordinates[1], BLACK);                 
    }
  }


  //****************************************************************************************************************************************************************************
  //**************************************************                  Graphing a constant temperature                *********************************************************
  //****************************************************************************************************************************************************************************  

  void Graphing_Forward()
  {
    if ( Temp >= Temp_Limit1 && Temp <= Temp_Limit2 )
    {
      GLCD.DrawLine(   XY_Coordinates[0],  48,   XY_Coordinates[0],  XY_Coordinates[1], BLACK);       
    }                 
  }

  //****************************************************************************************************************************************************************************
  //**************************************************                   PID Calculation to switch Output               ********************************************************
  //****************************************************************************************************************************************************************************  
  
  void PID_Calculations()
  {    
      Input = Temp;
      myPID.Compute();
            
      /************************************************
       * turn the output pin on/off based on pid output
       ************************************************/
      if(millis() - windowStartTime>WindowSize)
      { //time to shift the Relay Window
        windowStartTime += WindowSize;
      }
      
      if(Output < millis() - windowStartTime)
      {
       digitalWrite(RelayPin, LOW);
      }
      
      if(Output > millis() - windowStartTime)
      {
       digitalWrite(RelayPin,HIGH);
      }    
  }
  
  //****************************************************************************************************************************************************************************
  //**************************************************                          TEMP Print spacing                      ********************************************************
  //****************************************************************************************************************************************************************************  
  
  void Temp_Printing()
  {       
    GLCD.CursorToXY(96, 3);    
    
    if ( Temp <= 0 )
    {
      Temp = 0;
      GLCD.print("  ");
      GLCD.print(Temp);
    }

    if ( Temp < 10 && Temp > 0 )
    {
      GLCD.print("  ");
      GLCD.print(Temp);
    }
    
    if ( Temp < 100 && Temp > 10  )
    {
      GLCD.print(" ");
      GLCD.print(Temp);
    }

    if ( Temp >= 100 )
    {
      GLCD.print(Temp);                                                                                         // Print the Temp. reading
    }
  }

  //****************************************************************************************************************************************************************************
  //**************************************************                        TIME Print spacing                        ********************************************************
  //****************************************************************************************************************************************************************************  

  void Time_Printing()
  {
    while (Seconds >= 60  )
    {
      Minutes++;
      Seconds = Seconds - 60;                                                      
    }

    if (Minutes < 10)
    {
      GLCD.CursorToXY(38, 54);
      GLCD.print("0");
      GLCD.print(Minutes);        
    }
                                                        
    if (Minutes >= 10)
    {
      GLCD.CursorToXY(38, 54);      
      GLCD.print(Minutes);
    }                                                              
                                                     
    if (Seconds < 10)
    {
      GLCD.CursorToXY(57, 54);
      GLCD.print("0");
      GLCD.print(Seconds);
    }
                
    if (Seconds >= 10)
    {
      GLCD.CursorToXY(57, 54);
      GLCD.print(Seconds);
    }   
  }


  //****************************************************************************************************************************************************************************
  //**************************************************                          View Program                            ********************************************************
  //****************************************************************************************************************************************************************************  
   
  void View_Program() 
  {   
   
  //*************************************************           Changing seconds to minutes and seconds     ********************************************************************   
  
    while ( Stage_Temperature_Setpoints[6] >= 60 )
    {
      Button_States_Selectors_and_Detectors[12]++;
      Stage_Temperature_Setpoints[6] = Stage_Temperature_Setpoints[6] - 60;
    }

    while ( Stage_Temperature_Setpoints[7] >= 60 )
    {
      Button_States_Selectors_and_Detectors[13]++;
      Stage_Temperature_Setpoints[7] = Stage_Temperature_Setpoints[7] - 60;
    }

    while ( Stage_Temperature_Setpoints[8] >= 60 )
    {
      Button_States_Selectors_and_Detectors[14]++;
      Stage_Temperature_Setpoints[8] = Stage_Temperature_Setpoints[8] - 60;
    }

    GLCD.DrawRect( 0,  0, 127, 63);  
    GLCD.DrawLine( 0, 51, 126, 51);   
    GLCD.DrawLine( 42, 10, 42, 50);  
    GLCD.DrawLine( 83, 10, 83, 50);
    
  //*************************************************                        Printing Headers               ********************************************************************   
 
    GLCD.FillRect( 1, 1,   125,  8); 
    GLCD.SelectFont(System5x7, WHITE);                                                                      
    GLCD.CursorToXY(8, 2);
    GLCD.print("STAGE");                                                                                   
 
    GLCD.SelectFont(System5x7, WHITE); 
    GLCD.CursorToXY(51, 2);     // 60
    GLCD.print("T");
    GLCD.CursorToXY(59, 2);  
    GLCD.print(":");
    GLCD.DrawCircle(66, 3, min(GLCD.CenterX/2 + 51, GLCD.CenterY)-31,WHITE);    
    GLCD.CursorToXY(69, 2);
    GLCD.print("C");  
    GLCD.CursorToXY(90, 2);
    GLCD.print("S:");  
    GLCD.CursorToXY(102, 2);
    GLCD.print("MIN");      

    //*************************************************                         Printing PREH                 ********************************************************************   
  
    GLCD.SelectFont(System5x7); 
    GLCD.CursorToXY(11, 11);
    GLCD.print(String_Lookup_Table_Array[3]); //"PREH"
    
    GLCD.CursorToXY(54, 11);
    GLCD.print(Stage_Temperature_Setpoints[0]);     

    GLCD.CursorToXY(91, 11);    
    
    if (Button_States_Selectors_and_Detectors[12] < 10)
    {
      GLCD.print("0");
      GLCD.print(Button_States_Selectors_and_Detectors[12]);
    }

    else
    {
      GLCD.print(Button_States_Selectors_and_Detectors[12]);
    }
    
    GLCD.CursorToXY(103, 11);    
    GLCD.print(":");     
    GLCD.CursorToXY(109, 11);    
    
    if (Stage_Temperature_Setpoints[6] < 10)
    {
      GLCD.print("0");
      GLCD.print(Stage_Temperature_Setpoints[6]);
    }
    
    else
    {
      GLCD.print(Stage_Temperature_Setpoints[6]);
    }
       
    //*************************************************                          Printing HEAT                ********************************************************************
  
    GLCD.CursorToXY(11, 19);
    GLCD.print(String_Lookup_Table_Array[4]);  //HEAT 
    
    GLCD.CursorToXY(54, 19);
    GLCD.print(Stage_Temperature_Setpoints[1]);     

    GLCD.CursorToXY(91, 19);    
    
    if (Button_States_Selectors_and_Detectors[13] < 10)
    {
      GLCD.print("0");
      GLCD.print(Button_States_Selectors_and_Detectors[13]);
    }
    
    else
    {
      GLCD.print(Button_States_Selectors_and_Detectors[13]);
    }
    
    GLCD.CursorToXY(103, 19);    
    GLCD.print(":");     
    GLCD.CursorToXY(109, 19);    
    
    if (Stage_Temperature_Setpoints[7] < 10)
    {
      GLCD.print("0");
      GLCD.print(Stage_Temperature_Setpoints[7]);
    }

    else
    {
      GLCD.print(Stage_Temperature_Setpoints[7]);
    }

    //*************************************************                          Printing SLDR                ********************************************************************    
  
    GLCD.CursorToXY(11, 27);
    GLCD.print(String_Lookup_Table_Array[5]);   //SLDR
    
    GLCD.CursorToXY(54, 27);
    GLCD.print(Stage_Temperature_Setpoints[2]);     

    GLCD.CursorToXY(91, 27);    
    
    if (Button_States_Selectors_and_Detectors[14] < 10)
    {
      GLCD.print("0");
      GLCD.print(Button_States_Selectors_and_Detectors[14]);
    }
    
    else
    {
      GLCD.print(Button_States_Selectors_and_Detectors[14]);
    }
    
    GLCD.CursorToXY(103, 27);    
    GLCD.print(":");     
    GLCD.CursorToXY(109, 27);    
    
    if (Stage_Temperature_Setpoints[8] < 10)
    {
      GLCD.print("0");
      GLCD.print(Stage_Temperature_Setpoints[8]);
    }
    
    else
    {
      GLCD.print(Stage_Temperature_Setpoints[8]);    
    }

    //*************************************************                      Printing KEEP & COOL             ********************************************************************   
  
    GLCD.CursorToXY(11, 35);
    GLCD.print(String_Lookup_Table_Array[6]);   //"KEEP"

    GLCD.CursorToXY(54, 35);
    GLCD.print(Stage_Temperature_Setpoints[3]); 
    GLCD.CursorToXY(91, 35);     
    
    GLCD.CursorToXY(11, 43);
    GLCD.print(String_Lookup_Table_Array[7]);   //"COOL"
  
    GLCD.CursorToXY(54, 43);
    GLCD.print(Stage_Temperature_Setpoints[4]);    
    GLCD.CursorToXY(91, 43);     
  }

  //****************************************************************************************************************************************************************************
  //***************************************************                         Edit Programs                            *******************************************************
  //****************************************************************************************************************************************************************************
 
   void Edit_Program()
   {   
      //*************************************                    EDIT PROGRAM PREH TEMP                 *****************************************************************************
     
      while( Button_States_Selectors_and_Detectors[8] == 1 )
      {
        Read_Buttons();                                           
                   
        GLCD.CursorToXY(48, 11);                 
        GLCD.print("*"); 
                    
        GLCD.CursorToXY(73, 11);
        GLCD.print("*"); 
                   
        GLCD.CursorToXY(54, 11);                 
        GLCD.print(Stage_Temperature_Setpoints[0]);                   
                                                   
        if ( Button_States_Selectors_and_Detectors[2] == LOW )
        {
          Stage_Temperature_Setpoints[0]++; 
          delay(150); 
                    
          if (  Stage_Temperature_Setpoints[0] >= 260 )
          {
            Stage_Temperature_Setpoints[0] = 260; 
          }  
        }
                  
        Button_States_Selectors_and_Detectors[2] = HIGH; 
                 
        if ( Button_States_Selectors_and_Detectors[4] == HIGH )
        {
          Stage_Temperature_Setpoints[0]--; 
          delay(150); 
     
          if (  Stage_Temperature_Setpoints[0] <= 100 )
          {
            Stage_Temperature_Setpoints[0] = 100; 
          }              
        }    
                      
        Button_States_Selectors_and_Detectors[4] = LOW;  
                                       
        if ( Button_States_Selectors_and_Detectors[6] != Button_States_Selectors_and_Detectors[7] && Button_States_Selectors_and_Detectors[6] == HIGH )               
        {
          Button_States_Selectors_and_Detectors[8]++;                  
          delay(150);               
        }    
                      
        Button_States_Selectors_and_Detectors[7] = Button_States_Selectors_and_Detectors[6];             
                     
        //*************************************                    EDIT PROGRAM HEAT TEMP                 ***************************************************************************
                     
        while( Button_States_Selectors_and_Detectors[8] == 2 )
        {
          Read_Buttons();
                
          GLCD.FillRect( 48, 11, 5, 8,        WHITE );                     
          GLCD.FillRect( 73, 11, 5, 8,        WHITE );
                           
          GLCD.CursorToXY(48, 19);                 
          GLCD.print("*"); 
                          
          GLCD.CursorToXY(73, 19);
          GLCD.print("*");   
     
          GLCD.CursorToXY(54, 19);                 
          GLCD.print(Stage_Temperature_Setpoints[1]);                          
                                                        
          if ( Button_States_Selectors_and_Detectors[2] == LOW )
          {
            Stage_Temperature_Setpoints[1]++; 
            delay(150); 
                              
            if (  Stage_Temperature_Setpoints[1] >= 260 )
            {
              Stage_Temperature_Setpoints[1] = 260; 
            } 
          }
                            
          Button_States_Selectors_and_Detectors[2] = HIGH;
                           
          if ( Button_States_Selectors_and_Detectors[4] == HIGH )
          {
            Stage_Temperature_Setpoints[1]--; 
            delay(150);  
                  
            if (  Stage_Temperature_Setpoints[1] <= 100 )
            {
              Stage_Temperature_Setpoints[1] = 100; 
            }                                               
          }    
                                
          Button_States_Selectors_and_Detectors[4] = LOW; 
                                                 
          if ( Button_States_Selectors_and_Detectors[6] != Button_States_Selectors_and_Detectors[7] && Button_States_Selectors_and_Detectors[6] == HIGH )               
          {
            Button_States_Selectors_and_Detectors[8]++;                 
            delay(150);               
          }    
                                
          Button_States_Selectors_and_Detectors[7] = Button_States_Selectors_and_Detectors[6]; 
                          
          //*************************************                    EDIT PROGRAM SLDR TEMP                 ***************************************************************************
                          
          while( Button_States_Selectors_and_Detectors[8] == 3 )
          {
            Read_Buttons();
                          
            GLCD.FillRect( 48, 19, 5, 8,        WHITE );                     
            GLCD.FillRect( 73, 19, 5, 8,        WHITE );
                                     
            GLCD.CursorToXY(48, 27);                 
            GLCD.print("*"); 
                                    
            GLCD.CursorToXY(73, 27);
            GLCD.print("*");   
               
            GLCD.CursorToXY(54, 27);                 
            GLCD.print(Stage_Temperature_Setpoints[2]);                          
                                                                  
            if ( Button_States_Selectors_and_Detectors[2] == LOW )
            {
              Stage_Temperature_Setpoints[2]++; 
              delay(150);  
                                         
              if (  Stage_Temperature_Setpoints[2] >= 260 )
              {
                Stage_Temperature_Setpoints[2] = 260; 
              }                                        
            }
                                      
            Button_States_Selectors_and_Detectors[2] = HIGH;
                                     
            if ( Button_States_Selectors_and_Detectors[4] == HIGH )
            {
              Stage_Temperature_Setpoints[2]--; 
              delay(150); 
                           
              if (  Stage_Temperature_Setpoints[2] <= 100 )
              {
                Stage_Temperature_Setpoints[2] = 100; 
              }                                         
            }    
                                          
            Button_States_Selectors_and_Detectors[4] = LOW; 
                                                           
            if ( Button_States_Selectors_and_Detectors[6] != Button_States_Selectors_and_Detectors[7] && Button_States_Selectors_and_Detectors[6] == HIGH )               
            {
              Button_States_Selectors_and_Detectors[8]++;                 
              delay(150);               
            }    
                                          
            Button_States_Selectors_and_Detectors[7] = Button_States_Selectors_and_Detectors[6]; 
                                     
            //*************************************                    EDIT PROGRAM KEEP TEMP                 ***************************************************************************
           
            while( Button_States_Selectors_and_Detectors[8] == 4 )
            {
              Read_Buttons();
                                  
              GLCD.FillRect( 48, 27, 5, 8,        WHITE );                     
              GLCD.FillRect( 73, 27, 5, 8,        WHITE );
                                             
              GLCD.CursorToXY(48, 35);                 
              GLCD.print("*"); 
                                            
              GLCD.CursorToXY(73, 35);
              GLCD.print("*");   
                       
              GLCD.CursorToXY(54, 35);                 
              GLCD.print(Stage_Temperature_Setpoints[3]);                          
                                                                          
              if ( Button_States_Selectors_and_Detectors[2] == LOW )
              {
                Stage_Temperature_Setpoints[3]++; 
                delay(150);  
                                                 
                if (  Stage_Temperature_Setpoints[3] >= 260 )
                {
                  Stage_Temperature_Setpoints[3] = 260; 
                }                                               
              }
                                              
              Button_States_Selectors_and_Detectors[2] = HIGH;
                                             
              if ( Button_States_Selectors_and_Detectors[4] == HIGH )
              {
                Stage_Temperature_Setpoints[3]--; 
                delay(150);  
                                    
                if (  Stage_Temperature_Setpoints[3] <= 100 )
                {
                  Stage_Temperature_Setpoints[3] = 100; 
                }                                                 
              }    
                                                  
              Button_States_Selectors_and_Detectors[4] = LOW; 
                                                                   
              if ( Button_States_Selectors_and_Detectors[6] != Button_States_Selectors_and_Detectors[7] && Button_States_Selectors_and_Detectors[6] == HIGH )               
              {
                Button_States_Selectors_and_Detectors[8]++;                  
                delay(150);               
              }    
                                                  
              Button_States_Selectors_and_Detectors[7] = Button_States_Selectors_and_Detectors[6]; 
                                          
              //*************************************                    EDIT PROGRAM COOL TEMP                 ***************************************************************************
                                          
              while( Button_States_Selectors_and_Detectors[8] == 5 )
              {
                Read_Buttons();
                                          
                GLCD.FillRect( 48, 35, 5, 8,        WHITE );                     
                GLCD.FillRect( 73, 35, 5, 8,        WHITE );
                                                     
                GLCD.CursorToXY(48, 43);                 
                GLCD.print("*"); 
                                                    
                GLCD.CursorToXY(73, 43);
                GLCD.print("*");   
                               
                GLCD.CursorToXY(54, 43);                 
                GLCD.print(Stage_Temperature_Setpoints[4]);                          
                                                                                  
                if ( Button_States_Selectors_and_Detectors[2] == LOW )
                {
                  Stage_Temperature_Setpoints[4]++; 
                  delay(150);  
                                                         
                  if (  Stage_Temperature_Setpoints[4] >= 260 )
                  {
                    Stage_Temperature_Setpoints[4] = 260; 
                  }                                                         
                }
                                                      
                Button_States_Selectors_and_Detectors[2] = HIGH;
                                                     
                if ( Button_States_Selectors_and_Detectors[4] == HIGH )
                {
                  Stage_Temperature_Setpoints[4]--; 
                  delay(150);  
                                            
                  if (  Stage_Temperature_Setpoints[4] <= 100 )
                  {
                    Stage_Temperature_Setpoints[4] = 100; 
                  }                                                         
                }    
                                                          
                Button_States_Selectors_and_Detectors[4] = LOW; 
                                                                           
                if ( Button_States_Selectors_and_Detectors[6] != Button_States_Selectors_and_Detectors[7] && Button_States_Selectors_and_Detectors[6] == HIGH )               
                {
                  Button_States_Selectors_and_Detectors[8]++;                 
                  delay(150);               
                }    
                                                          
                Button_States_Selectors_and_Detectors[7] = Button_States_Selectors_and_Detectors[6]; 
                                                  
                //*************************************                                                       EDIT PROGRAM HEAT KEEP TIME                                      ***************************************************************************
                                                  
                while( Button_States_Selectors_and_Detectors[8] == 6 )
                {
                  Read_Buttons();
                                                       
                  GLCD.FillRect( 48, 43, 5, 7,        WHITE );                     
                  GLCD.FillRect( 73, 43, 5, 7,        WHITE );
                                                                     
                  GLCD.CursorToXY(85, 11);                 
                  GLCD.print("*"); 
                                                                    
                  GLCD.CursorToXY(121, 11);
                  GLCD.print("*");                                                                      
                                                              
                  //*************************************                                           Testing up down buttons to increment up down counter.                            ***************************************************************************
                                                                                                  
                  if ( Button_States_Selectors_and_Detectors[2] == LOW )
                  {
                    Stage_Temperature_Setpoints[6]++; 
                    delay(150);
                  }
                                                                      
                  Button_States_Selectors_and_Detectors[2] = HIGH;
                                                                     
                  if ( Button_States_Selectors_and_Detectors[4] == HIGH )
                  {
                    Stage_Temperature_Setpoints[6]--; 
                    delay(150);
                  }    
                                                                          
                  Button_States_Selectors_and_Detectors[4] = LOW; 
                                                       
                  //************    Testing whether seconds > 60 to increment the minutes and testing whether seconds < 10 to print a zero before the value to align everything and lastly testing whether seconds < 0 to decrement the minutes  *****************
  
                  GLCD.CursorToXY(85, 11);  
                                                       
                  while (Stage_Temperature_Setpoints[6] >= 60)
                  {
                    Button_States_Selectors_and_Detectors[12]++;
                    Stage_Temperature_Setpoints[6] = Stage_Temperature_Setpoints[6] - 60;
  
                    if (Button_States_Selectors_and_Detectors[12] < 10)
                    {
                      GLCD.print("0");
                      GLCD.print(Button_States_Selectors_and_Detectors[12]);
                    }
                                                          
                    else
                    {
                      GLCD.print(Button_States_Selectors_and_Detectors[12]);
                    }                                                         
                  }
                                                       
                  while (Stage_Temperature_Setpoints[6] < 0)
                  {
                    Button_States_Selectors_and_Detectors[12]--;
                    Stage_Temperature_Setpoints[6] = Stage_Temperature_Setpoints[6] + 60;  
                  }                                                                                                             
  
                  GLCD.CursorToXY(91, 11);
  
                  if (Button_States_Selectors_and_Detectors[13] < 10)
                  {
                    GLCD.print("0");
                    GLCD.print(Button_States_Selectors_and_Detectors[12]);
                  }
                                                          
                  else
                  {
                    GLCD.print(Button_States_Selectors_and_Detectors[12]);
                  } 
                                                       
                  GLCD.CursorToXY(109, 11);    
                                                       
                  if (Stage_Temperature_Setpoints[6] < 10)
                  {
                    GLCD.print("0");
                    GLCD.print(Stage_Temperature_Setpoints[6]);
                  }
                  
                  else
                  {
                    GLCD.print(Stage_Temperature_Setpoints[6]);
                  }
                                                                                                                            
                  if( Button_States_Selectors_and_Detectors[12] == 0 )                                                      
                  {
                    while ( Stage_Temperature_Setpoints[6] == 0 )
                    {
                      Read_Buttons();
                                                           
                      Stage_Temperature_Setpoints[6] = 0; 
                                                           
                      if ( Button_States_Selectors_and_Detectors[2] == LOW )
                      {
                        Stage_Temperature_Setpoints[6]++; 
                        delay(150);
                      }
                                                                          
                      Button_States_Selectors_and_Detectors[2] = HIGH;
                    }
                  }
                  
                  if ( Button_States_Selectors_and_Detectors[6] != Button_States_Selectors_and_Detectors[7] && Button_States_Selectors_and_Detectors[6] == HIGH )               
                  {
                    Button_States_Selectors_and_Detectors[8]++;                 
                    delay(150);               
                  }    
                                                            
                  Button_States_Selectors_and_Detectors[7] = Button_States_Selectors_and_Detectors[6];                                         
                                                     
                  //*************************************                                                       EDIT PROGRAM HEAT KEEP TIME                                      ***************************************************************************
                                                  
                  while( Button_States_Selectors_and_Detectors[8] == 7 )
                  {
                    Read_Buttons();
                                                          
                    GLCD.FillRect(  85, 11, 5, 7,        WHITE );                     
                    GLCD.FillRect( 121, 11, 5, 7,        WHITE );
                                                                     
                    GLCD.CursorToXY(85, 19);                 
                    GLCD.print("*"); 
                                                                    
                    GLCD.CursorToXY(121, 19);
                    GLCD.print("*");                                                                      
                                                              
                    //*************************************                                           Testing up down buttons to increment up down counter.                            ***************************************************************************
                                                                                                  
                    if ( Button_States_Selectors_and_Detectors[2] == LOW )
                    {
                      Stage_Temperature_Setpoints[7]++; 
                      delay(150);
                    }
                                                                      
                    Button_States_Selectors_and_Detectors[2] = HIGH;
                                                                     
                    if ( Button_States_Selectors_and_Detectors[4] == HIGH )
                    {
                      Stage_Temperature_Setpoints[7]--; 
                      delay(150);
                    }    
                                                                          
                    Button_States_Selectors_and_Detectors[4] = LOW; 
                                                       
                    //************    Testing whether seconds > 60 to increment the minutes and testing whether seconds < 10 to print a zero before the value to align everything and lastly testing whether seconds < 0 to decrement the minutes  *******************
  
                    GLCD.CursorToXY(85, 19);  
                                                       
                    while (Stage_Temperature_Setpoints[7] >= 60)
                    {
                      Button_States_Selectors_and_Detectors[13]++;
                      Stage_Temperature_Setpoints[7] = Stage_Temperature_Setpoints[7] - 60;
  
                      if (Button_States_Selectors_and_Detectors[13] < 10)
                      {
                        GLCD.print("0");
                        GLCD.print(Button_States_Selectors_and_Detectors[13]);
                      }
                                                          
                      else
                      {
                        GLCD.print(Button_States_Selectors_and_Detectors[13]);
                      }                                                         
                    }
                                                       
                    while (Stage_Temperature_Setpoints[7] < 0)
                    {
                      Button_States_Selectors_and_Detectors[13]--;
                      Stage_Temperature_Setpoints[7] = Stage_Temperature_Setpoints[7] + 60;  
                    }                                                                                                             
  
                    GLCD.CursorToXY(91, 19);
  
                    if (Button_States_Selectors_and_Detectors[13] < 10)
                    {
                      GLCD.print("0");                    
                      GLCD.print(Button_States_Selectors_and_Detectors[13]);
                    }
                                                          
                    else
                    {
                      GLCD.print(Button_States_Selectors_and_Detectors[13]);
                    } 
                                                       
                    GLCD.CursorToXY(109, 19);    
                                                       
                    if (Stage_Temperature_Setpoints[7] < 10)
                    {
                      GLCD.print("0");
                      GLCD.print(Stage_Temperature_Setpoints[7]);
                    }
                    
                    else
                    {
                      GLCD.print(Stage_Temperature_Setpoints[7]);
                    }
                                                                                                                            
                    if( Button_States_Selectors_and_Detectors[13] == 0 )                                                      
                    {
                      while ( Stage_Temperature_Setpoints[7] == 0 )
                      {
                        Read_Buttons();
                                                           
                        Stage_Temperature_Setpoints[7] = 0; 
                                                           
                        if ( Button_States_Selectors_and_Detectors[2] == LOW)
                        {
                          Stage_Temperature_Setpoints[7]++; 
                          delay(150);
                        }
                                                                          
                        Button_States_Selectors_and_Detectors[2] = HIGH;
                      }
                    }
                    
                    if ( Button_States_Selectors_and_Detectors[6] != Button_States_Selectors_and_Detectors[7] && Button_States_Selectors_and_Detectors[6] == HIGH )               
                    {
                      Button_States_Selectors_and_Detectors[8]++;                 
                      delay(150);               
                     }    
                                                            
                     Button_States_Selectors_and_Detectors[7] = Button_States_Selectors_and_Detectors[6];                                      
                                                     
                                                   
                      //*************************************                                                       EDIT PROGRAM SLDR KEEP TIME                                      ***************************************************************************
                                                  
                      while( Button_States_Selectors_and_Detectors[8] == 8 )
                      {
                        Read_Buttons();
                                                          
                        GLCD.FillRect(  85, 19, 5, 7,        WHITE );                     
                        GLCD.FillRect( 121, 19, 5, 7,        WHITE );
                                                                     
                        GLCD.CursorToXY(85, 27);                 
                        GLCD.print("*"); 
                                                                    
                        GLCD.CursorToXY(121, 27);
                        GLCD.print("*");                                                                      
                                                              
                        //*************************************                                           Testing up down buttons to increment up down counter.                             ***************************************************************************
                                                                                                  
                        if ( Button_States_Selectors_and_Detectors[2] == LOW )
                        {
                          Stage_Temperature_Setpoints[8]++; 
                          delay(150);
                        }
                                                                      
                        Button_States_Selectors_and_Detectors[2] = HIGH;
                                                                     
                        if ( Button_States_Selectors_and_Detectors[4] == HIGH )
                        {
                          Stage_Temperature_Setpoints[8]--; 
                          delay(150);
                        }    
                                                                          
                        Button_States_Selectors_and_Detectors[4] = LOW; 
                                                       
                        //************    Testing whether seconds > 60 to increment the minutes and testing whether seconds < 10 to print a zero before the value to align everything and lastly testing whether seconds < 0 to decrement the minutes  *******************
  
                        GLCD.CursorToXY(85, 27);  
                                                       
                        while (Stage_Temperature_Setpoints[8] >= 60)
                        {
                          Button_States_Selectors_and_Detectors[14]++;
                          Stage_Temperature_Setpoints[8] = Stage_Temperature_Setpoints[8] - 60;
  
                          if (Button_States_Selectors_and_Detectors[14] < 10)
                          {
                            GLCD.print("0");
                            GLCD.print(Button_States_Selectors_and_Detectors[14]);
                          }
                                                          
                          else
                          {
                            GLCD.print(Button_States_Selectors_and_Detectors[14]);
                          }                                                         
                        }
                                                                                                                             
                        while (Stage_Temperature_Setpoints[8] < 0)
                        {
                          Button_States_Selectors_and_Detectors[14]--;
                          Stage_Temperature_Setpoints[8] = Stage_Temperature_Setpoints[8] + 60;  
                        }                                                                                                             
  
                        GLCD.CursorToXY(91, 27);
  
                        if (Button_States_Selectors_and_Detectors[14] < 10)
                        {
                          GLCD.print("0");
                          GLCD.print(Button_States_Selectors_and_Detectors[14]);
                        }
                                                          
                        else
                        {
                          GLCD.print(Button_States_Selectors_and_Detectors[14]);
                        } 
                                                       
                        GLCD.CursorToXY(109, 27);    
                                                       
                        if (Stage_Temperature_Setpoints[8] < 10)
                        {
                          GLCD.print("0");
                          GLCD.print(Stage_Temperature_Setpoints[8]);
                        }
                        
                        else
                        {
                          GLCD.print(Stage_Temperature_Setpoints[8]);
                        }
                                                                                                                            
                        if( Button_States_Selectors_and_Detectors[14] == 0 )                                                      
                        {
                          while ( Stage_Temperature_Setpoints[8] == 0 )
                          {
                            Read_Buttons();
                                                           
                            Stage_Temperature_Setpoints[8] = 0; 
                                                           
                            if ( Button_States_Selectors_and_Detectors[2] == LOW )
                            {
                              Stage_Temperature_Setpoints[8]++; 
                              delay(150);
                            }
                                                                          
                            Button_States_Selectors_and_Detectors[2] = HIGH;
                          }
                        }
                        
                        if ( Button_States_Selectors_and_Detectors[6] != Button_States_Selectors_and_Detectors[7] && Button_States_Selectors_and_Detectors[6] == HIGH )               
                        {
                          Button_States_Selectors_and_Detectors[8] = 0;    
                          Button_States_Selectors_and_Detectors[11]  = 2;
                          Button_States_Selectors_and_Detectors[10] = 1;      
                        }    
                                                            
                        Button_States_Selectors_and_Detectors[7] = Button_States_Selectors_and_Detectors[6]; 
                      }
                  }                    
                  }
                }                                                                                                                                                                                                         
              }                                            
            }                                  
          }                         
        }               
     } 
        //*********************************************************************************************************************************************************    
        //*******************************************            Convert Minutes to Seconds Function             **************************************************    
        //*********************************************************************************************************************************************************       
    
   
        void Convert_Minutes_to_Seconds()                                                 // This function is needed to convert the total time into a single variable i.e. the amount of seconds so that 
        {                                                                                 // only one variable is written to the EEPROM. This happens before storing the values to the EEPROM
          int Time_Stage_Selector = 0;
          
          while ( Time_Stage_Selector < 3 )
          { 
              
              switch (Time_Stage_Selector) 
              {
                case 0:           
                  Variable       =  Stage_Temperature_Setpoints[6];                                            // This command makes Stage_Temperature_Setpoints[6] the same value as that of Variable
                  Min_Variable   =  Button_States_Selectors_and_Detectors[12]; 
                  break;
                    
                case 1:    
                  Variable       =  Stage_Temperature_Setpoints[7];                                            
                  Min_Variable   =  Button_States_Selectors_and_Detectors[13]; 
                  break;
                
                case 2:           
                  Variable       =  Stage_Temperature_Setpoints[8];                                            
                  Min_Variable   =  Button_States_Selectors_and_Detectors[14];  
                  break;      
              }             
              
              while ( Min_Variable > 0 )
              {
                  Min_Variable--;
                  Variable = Variable + 60;
              }    
              
              switch (Time_Stage_Selector) 
              {
                 case 0:       
                  Stage_Temperature_Setpoints[6]   =  Variable;                                            // This command makes Stage_Temperature_Setpoints[6] the same value as that of Variable
                  Button_States_Selectors_and_Detectors[12]   =  Min_Variable; 
                  break;
                    
                case 1:    
                  Stage_Temperature_Setpoints[7]   =  Variable;                                            
                  Button_States_Selectors_and_Detectors[13]   =  Min_Variable; 
                  break;
                
                case 2:           
                  Stage_Temperature_Setpoints[8]   =  Variable;                                            
                  Button_States_Selectors_and_Detectors[14]   =  Min_Variable;  
                  break;      
              }
              
              Time_Stage_Selector++;
          }
        }


        //*********************************************************************************************************************************************************    
        //*******************************************            Convert Seconds to Minutes Function             **************************************************    
        //*********************************************************************************************************************************************************       
    

        void Convert_Seconds_to_Minutes()                                                 // This function is needed to convert the single variable stored in the EEPROM back into the the seperate variables 
        {                                                                                 // that is minutes and seconds as it was before storing it. This happens after the value have been read from the 
          int Time_Stage_Selector = 0;
    
          while ( Time_Stage_Selector < 3 )
          {         

              switch (Time_Stage_Selector) 
              {
                case 0:           
                  Variable       =  Stage_Temperature_Setpoints[6];                                            // This command makes Stage_Temperature_Setpoints[6] the same value as that of Variable
                  Min_Variable   =  Button_States_Selectors_and_Detectors[12]; 
                  break;
                    
                case 1:    
                  Variable       =  Stage_Temperature_Setpoints[7];                                            
                  Min_Variable   =  Button_States_Selectors_and_Detectors[13]; 
                  break;
                
                case 2:           
                  Variable       =  Stage_Temperature_Setpoints[8];                                            
                  Min_Variable   =  Button_States_Selectors_and_Detectors[14];  
                  break;      
              }                         
              
              while ( Variable >= 60 )
              {
                Min_Variable++;
                Variable = Variable - 60;
              }   
              
              switch (Time_Stage_Selector) 
              {
                 case 0:           
                  Stage_Temperature_Setpoints[6]   =  Variable;                                            // This command makes Stage_Temperature_Setpoints[6] the same value as that of Variable
                  Button_States_Selectors_and_Detectors[12]   =  Min_Variable; 
                  break;
                    
                case 1:    
                  Stage_Temperature_Setpoints[7]   =  Variable;                                            
                  Button_States_Selectors_and_Detectors[13]   =  Min_Variable; 
                  break;
                
                case 2:           
                  Stage_Temperature_Setpoints[8]   =  Variable;                                            
                  Button_States_Selectors_and_Detectors[14]   =  Min_Variable;  
                  break;      
              }
                  
              Time_Stage_Selector++;
          }     
        }    
    
    
    //*********************************************************************************************************************************************************    
    //************************************              Store values from variables into storage array               ******************************************    
    //*********************************************************************************************************************************************************       


    void Store_Variables_into_Storage_Array_Elements()                                       
    {
      int Array_element_Selector = 0;

      while (Array_element_Selector < 12 )
      {
          switch (Array_element_Selector) 
          {
            case  0:           
              Stored_Values [ 0 ]  =      Stage_Temperature_Setpoints[0];
              break;
                
            case  1:    
              Stored_Values [ 1 ]  =      Stage_Temperature_Setpoints[1];
              break;
            
            case  2:           
              Stored_Values [ 2 ]  =      Stage_Temperature_Setpoints[2];
              break; 
    
            case  3:           
              Stored_Values [ 3 ]  =      Stage_Temperature_Setpoints[3];
              break;
                
            case  4:    
              Stored_Values [ 4 ]  =      Stage_Temperature_Setpoints[4]; 
              break;
            
            case  5:           
              Stored_Values [ 5 ]  =      Stage_Temperature_Setpoints[6]; 
              break; 
    
            case  6:           
              Stored_Values [ 6 ]  =      Stage_Temperature_Setpoints[7];
              break;
                
            case  7:    
              Stored_Values [ 7 ]  =      Stage_Temperature_Setpoints[8];
              break;
                
            case  8:    
              Stored_Values [ 8 ]  =      PREH_Hysteresis; 
              break;
            
            case  9:           
              Stored_Values [ 9 ]  =      HEAT_Hysteresis; 
              break; 
    
            case 10:           
              Stored_Values [ 10 ]  =     SLDR_Hysteresis;
              break;
                
            case 11:    
              Stored_Values [ 11 ]  =     Fan_Status;
              break;                                      
          }
          
          Array_element_Selector++;          
      }
    }
    
    //*********************************************************************************************************************************************************    
    //************************************               Store values from storage array into variables           *********************************************    
    //*********************************************************************************************************************************************************       


    void Store_Storage_Array_Elements_into_Variables()                                       
    {
      int Array_element_Selector = 0;

      while (Array_element_Selector < 12 )
      {
          switch (Array_element_Selector) 
          {
            case  0:           
              Stage_Temperature_Setpoints[0]        =     Stored_Values [ 0 ];
              break;
                
            case  1:    
              Stage_Temperature_Setpoints[1]         =     Stored_Values [ 1 ];
              break;
            
            case  2:           
              Stage_Temperature_Setpoints[2]         =     Stored_Values [ 2 ];
              break; 
    
            case  3:           
              Stage_Temperature_Setpoints[3]         =     Stored_Values [ 3 ];
              break;
                
            case  4:    
              Stage_Temperature_Setpoints[4]         =     Stored_Values [ 4 ]; 
              break;
            
            case  5:           
              Stage_Temperature_Setpoints[6]     =     Stored_Values [ 5 ]; 
              break; 
    
            case  6:           
              Stage_Temperature_Setpoints[7]     =     Stored_Values [ 6 ];
              break;
                
            case  7:    
              Stage_Temperature_Setpoints[8]     =     Stored_Values [ 7 ];
              break;
                
            case  8:    
              PREH_Hysteresis   =     Stored_Values [ 8 ];      
              break;
            
            case  9:           
              HEAT_Hysteresis   =     Stored_Values [ 9 ]; 
              break; 
    
            case 10:           
              SLDR_Hysteresis   =    Stored_Values [ 10 ];
              break;
                
            case 11:    
              Fan_Status        =    Stored_Values [ 11 ];
              break;               
          }          
          Array_element_Selector++;          
      }
    } 
