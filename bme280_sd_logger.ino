#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SD_CS 10 // change to your SD module's CS pin!

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

File dataFile;

unsigned long delayTime;

void setup() {
    Serial.begin(9600);
    while(!Serial);

    Serial.println(F("BME280 + SD Card Logger"));

    // Init SD card
    if (!SD.begin(SD_CS)) {
        Serial.println("SD card initialization failed!");
        while(1);
    }
    Serial.println("SD card initialized.");
    
    // Init BME280
    unsigned status;
    status = bme.begin(0x76);  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        while (1) delay(10);
    }
    
    Serial.println("-- Logging Data --");
    delayTime = 3000;
    
    // Optional: Write headers to the file
    dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
        dataFile.println("Temperature(C),Pressure(hPa),Altitude(m),Humidity(%)");
        dataFile.close();
    } else {
        Serial.println("Error opening datalog.txt for writing headers");
    }
}

void loop() { 
    float temperature = bme.readTemperature();
    float pressure = bme.readPressure() / 100.0F;
    float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    float humidity = bme.readHumidity();

    // Print to Serial Monitor
    Serial.print("Temperature = "); Serial.print(temperature); Serial.println(" °C");
    Serial.print("Pressure = "); Serial.print(pressure); Serial.println(" hPa");
    Serial.print("Approx. Altitude = "); Serial.print(altitude); Serial.println(" m");
    Serial.print("Humidity = "); Serial.print(humidity); Serial.println(" %");
    Serial.println();

    // Save to SD card
    dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
        dataFile.print(temperature); dataFile.print(",");
        dataFile.print(pressure);    dataFile.print(",");
        dataFile.print(altitude);    dataFile.print(",");
        dataFile.println(humidity);
        dataFile.close();
    } else {
        Serial.println("Error opening datalog.txt for writing data");
    }

    delay(delayTime);
}