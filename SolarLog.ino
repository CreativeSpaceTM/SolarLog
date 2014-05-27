#include <SD.h>
#include <stdlib.h>

const int CHIP_SELECT_PIN = 10;
const int STATUS_LED = 9;
const bool DEBUG = true;

int counter;

float CAPTURE_INTERVAL; //minutes

File dataFile;

bool error = false;

void setup()
{
    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, LOW);

    if (DEBUG)
    {
        CAPTURE_INTERVAL = 0.02;
        Serial.begin(9600);
    }
    else
        CAPTURE_INTERVAL = 10.0;
        

    debug("Initializing SD card");
    pinMode(CHIP_SELECT_PIN, OUTPUT);

    //init SD card
    if (!SD.begin(CHIP_SELECT_PIN))
    {
        debug("Card failed, or not present");
        
        while(1){ 
            //stop further execution
            blinkLED();
        };
    }

    char *filename = "data_0.csv";
    
    // ensure unique sequential name
    int nameIndex = 0;
    while(SD.exists(filename)){
        nameIndex++;
        String name = "data_" + String(nameIndex) + ".csv";
        name.toCharArray(filename, name.length()+1);
    }

    debug(filename);

    // open data file
    dataFile = SD.open(filename, FILE_WRITE);
    if (!dataFile)
    {
        debug("Error opening data file");

        while(1){
            //stop further execution
            blinkLED();
        }

    }
    
    debug("Card initialized.");

    log("Time,Voltage");

    // blink 3 times to see it started
    blinkLED();
    blinkLED();
    blinkLED();
}

void debug(String msg)
{
    if (DEBUG)
        Serial.println(msg);
}

void log(String msg)
{
    dataFile.println(msg);
    dataFile.flush();
    debug(msg); 
}

void blinkLED()
{
    digitalWrite(STATUS_LED, HIGH);
    delay(100);
    digitalWrite(STATUS_LED, LOW);
    delay(100);
}

void loop()
{   
    // read voltage and multiply by voltage divider factor
    float voltage= analogRead(A0) * (5.0 / 1023.0) * 1.22;

    // convert float voltage to a string
    char voltage_s[5];
    dtostrf(voltage, 4, 2, voltage_s);

    String entry = String(counter) + "," + voltage_s;
    log(entry);

    counter++;

    delay(CAPTURE_INTERVAL * 60000);        
}