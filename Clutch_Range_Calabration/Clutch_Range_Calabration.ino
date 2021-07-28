/* Load this onto the board to find the potentiometer ranges for the master and slave clutch.
*  While the board is pluged in open the serial monitor to display the values.
*  Move the clutch levers through their full ranges while running the program.
*  Make sure that there are no big jumps in the current values which would likely indicate that
*  there is a bad conection between the potentiometer and the control board. 
*  After doing a few full sweeps of both clutch assemblies the board can then be disconected 
*  and the Max and Min range values can be copied into the steering wheel control code  
*  where the they are set (between the doted lines towards the top).
*/

#define MSTR_PIN 20
#define SLV_PIN 21

int mstrMax = 0;
int mstrMin = 9999;

int slvMax = 0;
int slvMin = 9999;

char mstrBuff[50];
char slvBuff[50];

void setup() 
{     
  pinMode(MSTR_PIN, INPUT);
  pinMode(SLV_PIN, INPUT);

  Serial.begin(9600);
}

void loop() 
{
  int mstrCurrent = analogRead(MSTR_PIN);
  int slvCurrent = analogRead(SLV_PIN);

  if(mstrCurrent > mstrMax){mstrMax = mstrCurrent;}
  if(mstrCurrent < mstrMin){mstrMin = mstrCurrent;}
  if(slvCurrent > slvMax){slvMax = slvCurrent;}
  if(slvCurrent < slvMin){slvMin = slvCurrent;}

  sprintf(mstrBuff, "Master Current: %d Max: %d Min: %d", mstrCurrent, mstrMax, mstrMin);
  sprintf(slvBuff, "Slave Current: %d Max: %d Min: %d", slvCurrent, slvMax, slvMin);

  Serial.println(mstrBuff);
  Serial.println(slvBuff);
  Serial.println("----------------------------------------");
    
}
