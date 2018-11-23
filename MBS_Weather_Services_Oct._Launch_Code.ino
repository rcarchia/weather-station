//MBS Weather Services Code, Version 1
//Records Humidity and Temperature with a time stamp and writes to a txt file on an SD card

#include <SPI.h> //Libraries for the SD Card
#include <SD.h>

const int chipSelect = 10;  //chipSelect pin, 10 in this case
File myFile;

//SD card attached to SPI bus as follows:
//MOSI - pin 11
//MISO - pin 12
//CLK - pin 13
//CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)


#include <Adafruit_Sensor.h>  //needed for new version
#include "DHT.h"



#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

unsigned long time; //variable for time of each measurement

void setup() {
  Serial.begin(9600);

  dht.begin();

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  
  Serial.println("card initialized.");
  
  Serial.println("MBS Weather Station Running!");

////////
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("data.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.println("Writing...");
    myFile.println("Data:");
    // close the file:
    myFile.close();
    Serial.println("done!");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  
  /////////

}



void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  time = millis(); // establish variable
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

//record data

  myFile = SD.open("data.txt", FILE_WRITE); //open file

  if (myFile) {
    Serial.println("Writing...");

    myFile.print("Humidity: ");
    myFile.print(h);
    myFile.print(" %\t");
    myFile.print("Temperature: ");
    myFile.print(t);
    myFile.print(" *C ");
    myFile.print(f);
    myFile.print(" *F\t");
    myFile.print("Heat index: ");
    myFile.print(hic);
    myFile.print(" *C ");
    myFile.print(hif);
    myFile.print(" *F");
    myFile.print(" Time: ");
    myFile.println(time/1000);
    
    // close the file:
    myFile.close();
    Serial.println("done!");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}




  // if the file opened okay, write to it:

