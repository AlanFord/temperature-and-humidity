// The code for the temperature & humidity arduino learning station:
//  This is a simple code for use with an Arduino Uno, SG90 servo motor, a DHT temperature and humidity sensor, and a 16x2 LCD Shield
//  It goes along with a 3D printed sensor play stand from Build, RVA.  

// Include libraries of code for use.
#include <Servo.h>
#include <dht.h>
#include <LiquidCrystal.h>

// Define variables (memory space) to store values of types in the libraries.
dht sensorDHT;
Servo servoMotor;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//  Sometimes, to save space, we don't need a variable; we just need to remember one value up until the last moment when we compile the program.
//  In these cases we can use #define.  It's value cannot change when your program is running, it also doesn't take up memory.
#define sensorDHT_Pin 2
#define servoMotor_Pin 3

//  These particular numbers are used later for various things.
int numSamples = 0;
int maxSamples = 10;
double humidityCurrent, humiditySum;
double temperatureCurrent, temperatureSum;

//  The first code the microcontroller (arduino) starts reading when it wakes up is the setup:
void setup(){
  //  We tell the microchip which pin should have the motor signal attached to it, which we defined above.
  servoMotor.attach(servoMotor_Pin);
  
  // Then we boot up the lcd screen and send a clear signal for good measure.
  // Each of these functions is a function we are calling to use from a library we included.
  lcd.begin(16, 2);
  lcd.clear();

 // Once the screen's turned on and cleared, we want to draw some text on it as a title.
  lcd.setCursor(3,0); //3 steps out to center it, in row 0
  lcd.print("Build, RVA");
  lcd.setCursor(0,1); //1st box, 1 row down
  lcd.print("Temp & Humidity");

  Serial.begin(115200);

 //  Once the text is drawn, delay(milliseconds) is used to extend the text display.
  delay(5000);
}

//  The microcontrollers' main code goes inside a loop to repeat indefinitely
void loop()
{
  //  First we read the DHT11 Temperature & Humidity Sensor from the pin defined above.
  //  This stores both values for temperature and humidity into our variable.
  int chk = sensorDHT.read11(sensorDHT_Pin);

  //  Each time through the loop we add the current value in our variable to a running sum variable.
  humiditySum += sensorDHT.humidity;
  temperatureSum += sensorDHT.temperature;
  Serial.print("Sample OK: ");
  Serial.print((int)sensorDHT.temperature); Serial.print(" *C, "); 
  Serial.print((int)sensorDHT.humidity); Serial.println(" %");
  
  //  We keep track of how many times through the loop we've been by adding 1 to our count.
  numSamples += 1;

  //  Then, if the number of times through the loop is greater than some number we defined above, maxSamples,
  //  we devide the total running sum by the number of times through the loop to get the average.
  if(numSamples >= maxSamples){
    humiditySum /= maxSamples;
    temperatureSum /= maxSamples;

   //  We use a separate variable to display to the lcd screen so we don't show all the math
    humidityCurrent = humiditySum;

   //For temperature we convert celsius to fahrenheit by T(F) = T(C)*1.8+32
    temperatureCurrent = temperatureSum*1.8+32;
    
    //  We have to remember to reset the count of how many times through the loop when this happens
    //  On the first pass through, 10 values will be summed.  On subsequent passes 9 additional
    //  values will be read and summed, in addition to the average value from the previous pass.
    numSamples = 1;
    // humiditySum = 0;
    // temperatureSum = 0;  

  //  Each time , we display the data back on the LCD screen
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Temperature: ");
   lcd.setCursor(12,0);
   lcd.print(temperatureCurrent);
   lcd.setCursor(0,1);
   lcd.print("Humidity: ");
   lcd.setCursor(10,1);
   lcd.print(humidityCurrent);
  }

 //  Then we scale the humidity measurement for the servo.
  //  This one takes some math skills, so just ask for help, or google it :confused:
   double scaledServoHumidity = 180-180*(humidityCurrent / 100);

  //  Once we have converted the humidity measurement for scale from 0-180, then we send it to the servo.
   servoMotor.write(scaledServoHumidity);

  //  A delay is used each cycle since there is no good reason to run the processor at full bore.  2 samples a second is plenty fast.
   delay(500);
}
