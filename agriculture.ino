#include <LiquidCrystal.h>//import the library to control LCD display, this library has been added manually through ZIP file<br>#include
#include <DHT.h>//import library for DHT sensor
#include <DHT_U.h>
#include "DHT.h"
#define dht_apin A3 // Analog Pin sensor is connected to A3
#define DHTTYPE DHT11 //create DHT object
DHT dht(dht_apin, DHTTYPE);

LiquidCrystal lcd = LiquidCrystal(2, 3, 4, 5, 6, 7);// create LCD object
//declaration of variable for buttond
int ButtonOnePin = 8;  //pin for button 1
int ButtonOneState =LOW ; //one variable to store current button state
int ButtonOnePreviousState = LOW; //one variable to store previous button state
int ButtonTwoPin = 9; // same for button 2
int ButtonTwoState =LOW ;
int ButtonTwoPreviousState = LOW;
unsigned long timeBeginDisplay=0;
int timeDisplayPeriod = 6000; //period the 2nd and 3rd screen are displayed
int valToDisplay = 0; //variable to decide what information is to be displayed
int airTemp; //variable to store air temperature measured by DHT11 sensor
int airHumidity; //variable to store air humidity measured by DHT11 sensor
int LedPin =  13; 
const int buzzerPin = 11;
int waterPumpPin=10;// pin on which is connected the relay that command water pump
int count = 0;//counter 
int num = 0;// variable to store value of no. of sprays  
int rem,dom ;//variable to store value of remaining sprays 
int sprayNum=0; //variable to store number of spraying 
int pumpOn = false; //variable used for spraying, it stays ON during the spray timing
int MoistureHigh = 800; 
int MoistureGood = 710;
int MoistureLow = 630;//threshold that should launch water spray of the plant
long duration;//duration used to check depth
const int depth = 100;//assuming depth of water tank
float cm,level;//variable to store value of water level
int valSoil ;  //variable to store value measured by moisture sensor
const int pingPin = A4;//  pin on which ultrasonic sensor is connected 
int soilPin = A0; // pin on which moisture sensor is connected 
int soilPower = 12;
int timePeriod = 1000; //variable to store timing in ms, period between each display refresh, a clear is called each time display is called 
int humMeasurePeriod=0; 
unsigned long currentMillis; //variable to store current timing
unsigned long debutMillis=0; //variable used to store timing at the debut of a period
int timePumpPeriod = 3000; //timing for water spraying
unsigned long timePumpDebut = 0; //variable used to store timing value for the debut of a spraying period
int gazPin = A1; //pin on which is connected gaz sensor
int valGaz ; //variable on which is stored gaz sensor value
int lightPin = A2; //pin on which is connected the photoresistor
int valLight ; //variable on which light value is stored
//function to read moisture sensor value
int readSoil()
{    digitalWrite(soilPower, HIGH);//turn D7 "On"
    delay(250);//wait 10 milliseconds 
    valSoil = analogRead(soilPin);//Read the SIG value form sensor 
    digitalWrite(soilPower, LOW);//turn D7 "Off"
    return valSoil;//send current moisture value
}
//function to display output of different sensors on com port
int displaycom(){
  Serial.print("No.of Sprays:");
  Serial.print(num);
  Serial.print(" Humidity:");
  Serial.print(airHumidity);// to print humidity on com port
  Serial.print("%  Temperature:");
  Serial.print(airTemp);// to print temperature on com port
  Serial.print("C");
  Serial.print(" Moisture:");
  Serial.print(valSoil);// to print moisture on com port
  Serial.print(" C02:");
  Serial.print(valGaz);// to print CO2 on com port
  Serial.print("PPM  Light:");
  Serial.print(valLight);// to print light amount on com port   
  if ((depth - cm)<0){
    Serial.print(" 404 error");
    Serial.println();}
  else{
  Serial.print(" Water:");
  Serial.print(level);// to print water level on com port
  Serial.print("%");
  Serial.println();
   }
  delay(1000);//Wait 1 seconds before accessing sensor again, without timing, sensor measurement is faulty. we commented this delay because this function isn't called often
}
int waterlevel(){
   {
  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2000);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  //inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  level = ((depth - cm)/depth)*100;
  delay(2000);
}
}
long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}


//function called to refresh LCD screen, displaying wanted screen regarding value of variable "valToDisplay" 

int toDisplay(){  
  lcd.clear(); //clear the screen to remove character that might not be overwritten by new message
  
  if (valToDisplay == 0){
    lcd.setCursor(0,0); //to put cursor on 1rst character of 1rst line
    lcd.print("Temp:");
    lcd.setCursor(6,0);
    lcd.print("Hum:");
    lcd.setCursor(11,0);
    lcd.print("Mois:");
    lcd.setCursor(0,1); //to put cursor on 1rst character of 2nd line
    lcd.print(airTemp);
    lcd.setCursor(2,1);
    lcd.print((char)223); //special character for temperature
    lcd.print("C");
    lcd.setCursor(6,1);  
    lcd.print(airHumidity);
    lcd.setCursor(8,1); 
    lcd.print("%");
    lcd.setCursor(11,1); 
    lcd.print(valSoil); 
  }
  else if (valToDisplay == 1 && (currentMillis-timeBeginDisplay)) {
   
    lcd.setCursor(0,0); //to put cursor on 1rst character of 1rst line
    lcd.print("C02:");
    lcd.setCursor(7,0);
    lcd.print("Lig:");
    lcd.setCursor(12,0);
    lcd.print("Wat:"); 
    lcd.setCursor(0,1); //to put cursor on 1rst character of 2nd line
    lcd.print(valGaz);
    lcd.setCursor(3,1);
    lcd.print("PPM"); 
    lcd.setCursor(7,1);  
    lcd.print(valLight);
    lcd.setCursor(10,1); 
    lcd.print("%");
    lcd.setCursor(12,1);
    lcd.print(level);
    lcd.setCursor(14,1);
    lcd.print(" ");
    lcd.setCursor(15,1);
    lcd.print("%");
    delay(1000);
   
  }
   
   else if (valToDisplay==2 && (currentMillis-timeBeginDisplay)) {
    lcd.setCursor(0,0); //to put cursor on 1rst character of 1rst line
    lcd.print("Spray's:");
    lcd.setCursor(0,1); //to put cursor on 1rst character of 2nd line
    lcd.print(num);
    delay(1000);
   
     
  }  
  
  if((currentMillis-timeBeginDisplay)>timeDisplayPeriod){ //display original screen when display period is over
    
      valToDisplay=0;
    //we display in this IF condition the 1rst screen because before there was a blank screen after the timing
    lcd.setCursor(0,0); //to put cursor on 1rst character of 1rst line
    lcd.print("Temp:");
    lcd.setCursor(6,0);
    lcd.print("Hum:");
    lcd.setCursor(11,0);
    lcd.print("Mois:");
    lcd.setCursor(0,1); //to put cursor on 1rst character of 2nd line
    lcd.print(airTemp);
    lcd.setCursor(2,1);
    lcd.print((char)223); //special character for temperature
    lcd.print("C");
    lcd.setCursor(6,1);  
    lcd.print(airHumidity);
    lcd.setCursor(8,1); 
    lcd.print("%");
    lcd.setCursor(11,1); 
    lcd.print(valSoil); 
  }
  }
//Function to increase no. of sprays manually
int increase(){
       
     if (ButtonTwoState == HIGH){
      num = count++;
     }
  if(num>6){// max no of sprays used at a time 
        num = 0; 
 }}
 //Function to spray water to plants 
 int spray(){
 if(ButtonOneState == HIGH){
     while(num>0){
        pumpOn = HIGH;
        digitalWrite( waterPumpPin, HIGH);
        Serial.print("\n");
        Serial.print(" No. of Sprays Rem: ");
        delay(5000);
        digitalWrite( waterPumpPin, LOW);
        delay(1000);
        rem = num-1;
        num--;
        Serial.print(rem);
        if(rem == 0){
           num = 0;
           count=0;
           break;
        } } }
     }
int waterPump(){//the condition below to command or not the water pump
 if (valSoil<MoistureLow)
 { 
     num = 2;// no of sprays 
     sprayNum = sprayNum+2; 
     spray();
  }
}
int waterTankAlarm(){
  if (level<10.00){ //when number of spraying is above limit, turn on led to warn user
    digitalWrite(LedPin,HIGH);
    buzz();
    delay(20000);
    sprayNum = 0;
  }
  else {
    digitalWrite(LedPin,LOW);
  }
}
int buzz(){
  tone(buzzerPin, 500);
  delay(5000);
  noTone(buzzerPin);
  delay(100);
}
void setup(){
  // put your setup code here, to run once:
  Serial.begin(9600);
  //defining which pin are IN or OUT
  pinMode(LedPin, OUTPUT);      
  pinMode(ButtonOnePin, INPUT); 
  pinMode(ButtonTwoPin, INPUT); 
  pinMode(waterPumpPin, OUTPUT); 
  pinMode(pingPin, OUTPUT);
  pinMode(pingPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  // initialize the lcd
  lcd.begin(16,2);                       
  lcd.setCursor(0,0);
  //waterPumpPin = HIGH; 
  lcd.print("Hello Anmol"); //ready first temperature and humidity from DHT sensor to store value (since this function isn't called often)
  displaycom();
  dht.begin();
  readSoil();
  waterlevel();
  delay(1000);
  lcd.clear();
  
}
void loop() {  //reading of sensor value and performinf functions
  readSoil();
  waterlevel();
  valGaz = analogRead(gazPin);
  valLight = analogRead(lightPin);
 // Reading temperature or humidity takes about 250 milliseconds!
 // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
 airHumidity =  dht.readHumidity();
  airTemp =  dht.readTemperature();
  delay(250);
  currentMillis=millis(); //measurement of current time  //small function to avoid button rebound, when pressing once on the button, arduino reads only one pressing...
  ButtonOneState=digitalRead(ButtonOnePin); //reading of button value

  //Serial.print("Btn 1 val :");
  //Serial.print(ButtonOneState);
  
  if (ButtonOneState !=ButtonOnePreviousState){ //comparing with previous state
    if (ButtonOneState == HIGH){ //if previous state was 0 and current is 1, then take button pressing in account
      //lcd.setCursor(0,0);
      //lcd.print("Button 1 ");
      delay(250);
      valToDisplay=1;
      timeBeginDisplay=currentMillis;
    }  
    ButtonOnePreviousState=ButtonOneState;
  }
  
 ButtonTwoState=digitalRead(ButtonTwoPin);
  if (ButtonTwoState !=ButtonTwoPreviousState){//same function for button 2 as button 1
  
  
    if (ButtonTwoState == HIGH){
      //0lcd.setCursor(0,0);
      //lcd.print("Button 2 ");
      delay(250);
      valToDisplay=2; //particular value for "valToDisplay"
      timeBeginDisplay=currentMillis;
      
    }
      ButtonTwoPreviousState=ButtonTwoState;
  }
  if(valToDisplay ==2){
    //loop for spray input
    increase();
    //spray();
    //loop to exe sprays
    }
  spray();
  waterPump(); //calling for water pump function which launch spray cycle if needed  
  waterTankAlarm(); //calling function to warn user about water tank level
  displaycom();
   //small condition for screen refreshing and measurement of DHT sensor
if (currentMillis-debutMillis>timePeriod){ //end of a period
    debutMillis = currentMillis; //define the begining of a new period
    toDisplay(); //refresh of the screen regarding "valToDisplay" value
    humMeasurePeriod++; //DHT sensor values are measured every 20 times the screens is refreshed
  }
   }
