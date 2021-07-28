#include <Joystick.h> //  [https://github.com/MHeironimus/ArduinoJoystickLibrary]
#include <Keypad.h>
#include <EEPROM.h>

/*----------------------- These values should be customised to liking, current values my not work out of the box------------------
*
*Best way to select your own buttons is to use windows game controller settings input preview or a gamepad tester website to see 
*how the buttons are mapped. Load this code onto your board and just start pressing buttons. See what value comes up for the 
*button you want to use and input that value below.
*
*Potentiometer ranges should be checked after clutch assembly using the clutch range program and then inputted below.
*/ 

// buttons held together to arm clutch adjust mode, program doesn't block these from sending to pc when pushed so care for unintentional commands
#define CLUTCH_ADJ_BUTTON_1 18 
#define CLUTCH_ADJ_BUTTON_2 19

#define INCREASE_BUTTON 7 // changes the slv clutch multipler value (bite adjust) 
#define DECREASE_BUTTON 22
#define ADJ_INCRMT_INCREASE_BUTTON 16 // changes the adjustment increment 
#define ADJ_INCRMT_DECREASE_BUTTON 1

#define MSTR_POT_MAX 538 // master clutch potentiometer movement range 
#define MSTR_POT_MIN 470

#define SLV_POT_MAX 730 // slave clutch potentiometer movement range
#define SLV_POT_MIN 650
//--------------------------------------------------------------------------------------------------------------------------------


// swap these two numbers around below if you want to swap which clutch is which
#define SLV_PIN 21 // slave clutch signal pin
#define MSTR_PIN 20 // master clutch signal pin

#define BITE_ADJ_MAX 0.95 // range for the slv clutch multipler
#define BITE_ADJ_MIN 0.05

#define ENABLE_PULLUPS
#define NUMBUTTONS 30 // pcb allows 30 buttons but not all need to be used 
#define NUMROWS 6
#define NUMCOLS 5


int addr = 1;

float slvValue = 0;
float mstrValue = 0;

double slvRead = 0;
double mstrRead = 0;

float sendValue = 0;

float biteAdjust = 0.4;
float adjustIncrement = 0.01;

bool clutchCalabration = false;
char currentPressedChar;
int calaSeen = 0;

byte buttons[NUMROWS][NUMCOLS] = { // buttons map to chars
  {0, 1, 2, 3, 4},
  {5, 6, 7, 8, 9},
  {10, 11, 12, 13, 14},
  {15, 16, 17, 18, 19},
  {20, 21, 22, 23, 24},
  {25, 26, 27, 28, 29}
};


byte rowPins[NUMROWS] = {10, 16, 14, 15, 18, 19}; // data pins
byte colPins[NUMCOLS] = {2, 3, 4, 5, 6}; // common pins

Keypad butts = Keypad( makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK, NUMBUTTONS, 0, true, false, false, false, false, false, false, false, false, false, false);
                  // make joystick with buttons and X axis


void CheckClutchAdjust(int butNum)  // checks for button presses for adjusting the slave clutch
{
  char pressed = butts.key[butNum].kchar;
  if (pressed == (INCREASE_BUTTON - 1)) 
  {
    biteAdjust = biteAdjust + adjustIncrement;
    if(biteAdjust > BITE_ADJ_MAX) //increased past slave clutch max so set to max
    {
      biteAdjust = BITE_ADJ_MAX;
    }    
  }
  else if (pressed == (DECREASE_BUTTON - 1))
  {
    biteAdjust = biteAdjust - adjustIncrement;
    if(biteAdjust < BITE_ADJ_MIN) //decreased past slave clutch min so set to min
    {
      biteAdjust = BITE_ADJ_MIN;
    }    
  }
  else if (pressed == (ADJ_INCRMT_INCREASE_BUTTON - 1))//increase adjustment incremnet
  {
    adjustIncrement = adjustIncrement * 10;    
  }
  else if (butNum == (ADJ_INCRMT_DECREASE_BUTTON - 1))//decrease adjustment incremnet
  {
    adjustIncrement = adjustIncrement / 10;    
  }
  else // inputs made during clutch adjustment not for adjustment don't get blocked to the computer 
  {  
    Joystick.setButton(pressed, 1); 
  }
  
  
}

void CheckClutch(void) // handles reading, operations and setting clutchs
{
  mstrValue = map(analogRead(MSTR_PIN), MSTR_POT_MIN, MSTR_POT_MAX, 1023, 0); // read master clutch and map to 0 - 1023 range, pot max min range need to be set to get good readings
  
  slvRead = map(analogRead(SLV_PIN), SLV_POT_MIN, SLV_POT_MAX, 0, 1023); // read slave clutch and map to 0 - 1023 range, pot max min range need to be set to get good readings
  slvValue = slvRead * biteAdjust; // uses the clutch mutlipler to set clutch travel for slave clutch
  
  if (mstrValue > slvValue) // determines what clutch value to use (largest value is always used)
  {
    Joystick.setXAxis(mstrValue);
  }
  else
  {
    Joystick.setXAxis(slvValue);
  }  
}


void CheckAllButtons(void) 
{
  if (butts.getKeys()) 
  {
    for (int i = 0; i < LIST_MAX; i++)
    {
      if (butts.key[i].stateChanged )
      {
         switch (butts.key[i].kstate) 
         {
          case PRESSED:
            if (clutchCalabration)
            {              
              CheckClutchAdjust(i);
            }
            else
            {
              Joystick.setButton(butts.key[i].kchar, 1);              
            }
            
            break;
          case HOLD:
            currentPressedChar = butts.key[i].kchar;

            if (currentPressedChar == (CLUTCH_ADJ_BUTTON_1 - 1) || currentPressedChar == (CLUTCH_ADJ_BUTTON_2 - 1)) // Uses look left and look right together
            {
              calaSeen++; // when @ 2 we know we've seen both button holds or one button held for 2 cycles
              
            }
            Joystick.setButton(currentPressedChar, 1);
            
            break;
          case RELEASED:
          case IDLE:
            Joystick.setButton(butts.key[i].kchar, 0);
            break;
        }
      }
    }
    if (calaSeen == 2) // when @ 2 we know we've seen both button holds or one held for 2 cycles, the 2 could be increased if you want longer holds to engage/ disengage the calabration mode
    {
      //Joystick.setButton(29, 1); // to digitally show clutch adjust state
      if (clutchCalabration)
      {
        adjustIncrement = 0.1; // resets the adjustment increment when disarming clutch calabration
        EEPROM.write(addr,biteAdjust); // save the slave clutch multipler to eeprom 

        //Joystick.setButton(29, 0); // to digitally show clutch adjust state
      }
      clutchCalabration = !clutchCalabration;
    }
    calaSeen = 0;
  }
}




void setup() 
{
  pinMode(MSTR_PIN, INPUT); // setup clutch inputs
  pinMode(SLV_PIN, INPUT);

  biteAdjust = EEPROM.read(addr); // read the maximum slave clutch value

  if (biteAdjust > BITE_ADJ_MAX || biteAdjust < BITE_ADJ_MIN) // check that what is read from eeprom is what we want 
  {
    biteAdjust = 0.4; // set to default when biteAdjust is read as out of bounds on start up
  }

  Joystick.begin();  
}

void loop() 
{
  CheckAllButtons();
  CheckClutch();
}
