
//Code for reading the temperature

float temp;                //where the final temperature data is stored
unsigned long time;        //current time

void setup() {             //main loop
Serial.begin(38400);       //set up the hardware serial port to run at 38400
pinMode(32, OUTPUT);       //set pin 32 as an output
Serial.print ("Time Temp");

//Set pins sending signal to the H&C Control Board as output pins
pinMode(PeltierPin, OUTPUT);
pinMode(PolarityPin, OUTPUT);  
pinMode(HCPumpPin, OUTPUT);
pinMode(HCFanPin, OUTPUT);
}

void loop() {

time = millis()/1000;    //converts milliseconds to seconds   
temp = read_temp();      //call the function “read_temp” and return the temperature in C°
Serial.println();
Serial.print(time);      //prints time since program started
Serial.print(" ");
Serial.print(temp);      //print the temperature data
delay(1000);             //wait 1000ms before we do it again
}

float read_temp(void){   //the read temperature function
float v_out;             //voltage output from temp sensor 
float temp;              //the final temperature is stored here
digitalWrite(A0, LOW);   //set pull-up on analog pin
digitalWrite(32, HIGH);   //set pin 32 high, this will turn on temp sensor
delay(2);                //wait 2 ms for temp to stabilize
v_out = analogRead(0);   //read the input pin
digitalWrite(32, LOW);    //set pin 32 low, this will turn off temp sensor
v_out*=.0048;            //convert ADC points to volts (we are using .0048 because this device is running at 5 volts)
v_out*=1000;             //convert volts to millivolts
temp= 0.0512 * v_out -20.5128; //the equation from millivolts to temperature
return temp;             //send back the temp
}

//Create a set point - TSetpoint = user input from GUI// How to optain user input from the GUI for now use the following
double TSetpoint = 20;     //User defined setpoint in degrees celcius defined as a number

//Board Info From Jasper Sikken - http://www.instructables.com/id/BioMONSTAAAR-HC-Control-Board/
//0V on PELTIER turns peltier OFF , and +5V turns peltier ON
//0V on POLARITY is HEATING , and +5V is COOLING
//0V on PUMP is OFF, and +5V is ON
//0V on FAN is OFF, and +5V is ON

int PeltierPin = 1;   // The digital output pin that turns the Peltier On/Off (5V,OV aka HIGH/LOW)
int PolarityPin = 2;  // The digital output pin that swithes the polarity of the Peltier for heating (OV or LOW) and cooling (5V or HIGH)
int HCPumpPin = 3;      // The digital output pin that swithes the water pump On/Off (5V,OV aka HIGH/LOW)
int HCFanPin = 4;       // The digital output pin that swithes the fan On/Off (5V,OV aka HIGH/LOW)

//If the temperature is 0.5C below setpoint then turn on heat
if (temp - TSetpoint < 0.5) {
  Serial.println("Cooling");  
  digitalWrite(PeltierPin, HIGH); // Turns on Peltier
  digitalWrite(Polarity,LOW);     // Negative polarity to turns on heating
  digitalWrite(HCPumpPin, HIGH);    // Turns on H&C system's pump
  digitalWrite(HCFanPin, HIGH);     // Turns on H&c system's fan
  }
// If the temperature is 0.5C above setpoint turn on cooling
else if (temp - TSetpoint > 0.5){
  digitalWrite(PeltierPin, HIGH); // Turns on Peltier
  digitalWrite(Polarity, HIGH);   // Positive polarity turns on cooling
  digitalWrite(HCPumpPin, HIGH);    // Turns on H&C system's pump
  digitalWrite(HCFanPin, HIGH);     // Turns on H&c system's fan
  }
else { // If the temperature is within 1 degree of setpoint do nothing or turn off H&C system
  digitalWrite(PeltierPin,LOW);   // Turn off Peltier 
  digitalWrite(HCPumpPin, LOW);     // Turns off H&C system's pump
  digitalWrite(HCFanPin, LOW);      // Turns off H&c system's fan
  }

// ANOTHER METHOD OF CODING THE CONTROL 
//void loop() {
// if(TSetpoint > 0) 
// switch (temp - TSetpoint) {
// case '< 0.5':    
//   digitalWrite(PeltierPin, HIGH); // Turns on Peltier
//   digitalWrite(Polarity,LOW);     // Negative polarity to turns on heating
//   digitalWrite(HCPumpPin, HIGH);    // Turns on H&C system's pump
//   digitalWrite(HCFanPin, HIGH);     // Turns on H&c system's fan
//   break;
// case '> 0.5':    
//   digitalWrite(PeltierPin, HIGH); // Turns on Peltier
//   digitalWrite(Polarity, HIGH);   // Positive polarity turns on cooling
//   digitalWrite(HCPumpPin, HIGH);    // Turns on H&C system's pump
//   digitalWrite(HCFanPin, HIGH);     // Turns on H&c system's fan
//   break;
// default:
      // If the temperature is within 1 degree of setpoint do nothing or turn off H&C system
//   digitalWrite(PeltierPin,LOW);   // Turn off Peltier 
//   digitalWrite(HCPumpPin, LOW);     // Turns off H&C system's pump
//   digitalWrite(HCFanPin, LOW);      // Turns off H&c system's fan
//   }
//}  
