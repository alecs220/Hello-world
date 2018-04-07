#include <TFT.h>  // Arduino LCD library
#include <SPI.h>
#include <DHT.h>
#include <MD_KeySwitch.h>

#define DHTPIN 7     // what digital pin we're connected to

#define DHTTYPE DHT11   // DHT 11

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

// char array to print to the screen
char humidPrint[6];
char tempPrint[6];
char tmptempPrint[6];
float tmphumid = 0.00;
float tmptemp = 0.00;
float tmpsettemp =0.00;
float settemp = 22.00;

const uint8_t PIN_UP = 5;       // switch connected to this pin
const uint8_t PIN_DWN = 3;       // switch connected to this pin
const uint8_t SWITCH_ACTIVE = HIGH;  // digital signal when switch is pressed 'on'

MD_KeySwitch UP(PIN_UP, SWITCH_ACTIVE);
MD_KeySwitch DWN(PIN_DWN, SWITCH_ACTIVE);

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(9600);
  Serial.println("DHTxx test!");

  //begin reading the buttons
  UP.begin();
  UP.enableDoublePress(true);

  DWN.begin();
  DWN.enableDoublePress(true);

  //Begin a DHT11 instance
  dht.begin();
  
  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();
  // Rotate the screen to landscape
  TFTscreen.setRotation(3);
  
  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);

  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255, 255, 255);
  // set the font size
  TFTscreen.setTextSize(2);
  // write the text to the top left corner of the screen
  TFTscreen.text("Humidity: % \n ", 0, 0);

  // set the font size
  TFTscreen.setTextSize(2);
  // write the text to the top left corner of the screen
  TFTscreen.text("Temp: Celsius\n ", 0, 45);

  // set the font color to yellow
  TFTscreen.stroke(0, 255, 255);
  //Real temperature
  TFTscreen.text("Real:\n ", 0, 65);
  //Set temperature
  TFTscreen.text("Set:\n ", 90, 65);
    
  // ste the font size very large for the loop
  TFTscreen.setTextSize(2);
}

void loop() {

  // Wait a few seconds between measurements.
  //delay(250);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  // Read the value of the DHT sensor
  // Humidity
  String humidVal = String(h);

  //Temperature
  String tempVal = String(t);

  if (UP.read() == 1 && settemp <= 31)
  {
    settemp += 1;
  }
  else if (DWN.read() == 1 && settemp > 15)
  {
    settemp -= 1;
  }
  
  //set Temperature
  String tmptempVal = String(settemp);

  // convert the reading to a char array
  humidVal.toCharArray(humidPrint, 6);
  tempVal.toCharArray(tempPrint, 6);
  tmptempVal.toCharArray(tmptempPrint, 6);


  if (h != tmphumid)
  {
    tmphumid = h;
    
    // erase the text before the new one is wrote
    TFTscreen.fillRect(0, 20, 160,14,0);

    // set the font color
    TFTscreen.stroke(0, 255, 0);

    //write the new value
    TFTscreen.text(humidPrint, 0, 20);

  }

  else if (t != tmptemp)
  {
    tmptemp = t;
    // erase the text before the new one is wrote
    TFTscreen.fillRect(0, 89, 70,14,0);

    // set the font color
    TFTscreen.stroke(0, 255, 0);

    //write the new value
    TFTscreen.text(tempPrint, 0, 89); 

  }

  else if (settemp != tmpsettemp)
  {
    tmpsettemp = settemp; 
    // erase the text before the new one is wrote   
    TFTscreen.fillRect(90, 89, 70,14,0);

    // set the font color
    TFTscreen.stroke(0, 255, 0);

    //write the new value
    TFTscreen.text(tmptempPrint, 90, 89); 

  }

}

