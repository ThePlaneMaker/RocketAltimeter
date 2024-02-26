#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

File myFile;
Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

unsigned long startTime;
int buzzerPin = 7;
int delayTime = 150;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
/////////////////////////////////////////////////////////

  Serial.print(F("Initializing SD card..."));

  if (!SD.begin(8)) {
    Serial.println(F("initialization failed!"));
    pinMode(buzzerPin, OUTPUT);
    tone(buzzerPin, 440); //A4
    delay(1000);
    pinMode(buzzerPin, INPUT);
    
    while (1);
  }
  Serial.println(F("initialization done."));

/////////////////////////////////////////////////////////

  unsigned status;
  //status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  status = bmp.begin();
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor"));

    while (1) delay(10);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  //bmp_temp->printSensorDetails();
/////////////////////////////////////////////////////////
  String latestDataFile = getLatestDataFileName();
  Serial.println("Latest data file: " + latestDataFile);

  int file_number = latestDataFile.substring(6, latestDataFile.indexOf(".")).toInt();
  String new_file = "FDATA_" + String(file_number+1) + ".TXT";
  
  Serial.println(new_file);

  startTime = millis();


  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(new_file, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    mario(); //play the mario theme for start

    // calibrating the altitude so it starts from 0
    float cal_altitude = 0.0;
    Serial.print("Calibrating altitude...");
    for (int i = 0; i < 50; i++) {

        float altitude = bmp.readAltitude();
        cal_altitude += altitude;

        delay(100);
      }
    cal_altitude /= 50.0;
    Serial.println("done: " + String(cal_altitude) + "m");
//////////////////////////////////////////////////////////////////////////

    Serial.print("Writing to flightData...");
    myFile.println("time (s),temperature (°C),pressure (hPa),altitude (m)");

    char elapsedTimeStr[10];
    char temperatureStr[10];
    char pressureStr[10];
    char altitudeStr[10];


    for (int i = 0; i < 30; i++) {
        // Your code to be executed inside the loop
        unsigned long currentTime = millis();
        float elapsedTime = (currentTime - startTime)/1000.0;

        dtostrf(elapsedTime, 1, 1, elapsedTimeStr);

        sensors_event_t temp_event, pressure_event;
        bmp_temp->getEvent(&temp_event);
        bmp_pressure->getEvent(&pressure_event);

        float temperature = temp_event.temperature;
        float pressure = pressure_event.pressure;
        float altitude = bmp.readAltitude()-cal_altitude;

        dtostrf(temperature, 1, 1, temperatureStr);
        dtostrf(pressure, 1, 1, pressureStr);
        dtostrf(altitude, 1, 1, altitudeStr);

        String obtained_data = String(elapsedTimeStr) + ","+
                              String(temperatureStr) + "," +
                              String(pressureStr) + "," + 
                              String(altitudeStr);
        myFile.println(obtained_data);

        delay(500);
      }

    myFile.close();
    Serial.println("done.");

    pinMode(buzzerPin, OUTPUT);
    for (int i = 0; i < 2; i++){
      tone(buzzerPin, 440); //A4
      delay(100);
      tone(buzzerPin, 880); //A4
      delay(100);
    }
    pinMode(buzzerPin, INPUT);

  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening text file");
  }

}

void loop() {
  // nothing happens after setup
}


String getLatestDataFileName() {
  int maxNumber = 0;
  String latestFileName = "";
  
  File dir = SD.open("/");

  while (true) {
    File entry = dir.openNextFile();

    if (!entry) {
      break;
    }

    String fileName = entry.name();
    
    if (fileName.startsWith("FDATA") && fileName.endsWith(".TXT")) {
      int number = fileName.substring(6, fileName.indexOf(".")).toInt();
      if (number > maxNumber) {
        maxNumber = number;
        latestFileName = fileName;
      }
    }

    entry.close();
  }

  dir.close();
  return latestFileName;
}


void mario(){
  pinMode(buzzerPin, OUTPUT);

  tone(buzzerPin, 330); //E4
  delay(delayTime*0.9);
  pinMode(buzzerPin, INPUT);
  delay(delayTime*0.1);

  tone(buzzerPin, 330); //E4
  delay(delayTime*0.9);
  pinMode(buzzerPin, INPUT);
  delay(delayTime*0.1);

  delay(delayTime);

  tone(buzzerPin, 330); //E4
  delay(delayTime*0.9);
  pinMode(buzzerPin, INPUT);
  delay(delayTime*0.1);

  delay(delayTime);

  tone(buzzerPin, 262); //C4
  delay(delayTime*0.9);
  pinMode(buzzerPin, INPUT);
  delay(delayTime*0.1);

  tone(buzzerPin, 330); //E4
  delay(delayTime*0.9);
  pinMode(buzzerPin, INPUT);
  delay(delayTime*0.1);

  delay(delayTime);

  tone(buzzerPin, 196); //G4
  delay(delayTime);
  pinMode(buzzerPin, INPUT);
}