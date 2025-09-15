//Allgemein
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>

#include <ArduinoJson.h>
StaticJsonDocument<4096> daten;

JsonArray sensors;
JsonObject sensors_bme280;
JsonArray sensors_bme280_readings;
JsonObject sensors_bme_temp;
JsonObject sensors_bme_pres;
JsonObject sensors_bme_hum;
JsonObject sensors_ccs811;
JsonArray sensors_ccs811_readings;
JsonObject sensors_ccs_co2;
JsonObject sensors_ccs_tvoc;
JsonObject sensors_dust;
JsonArray sensors_dust_readings;
JsonObject sensors_dust_ratio;
JsonObject sensors_dust_conc;
JsonObject sensors_dust_lpo;
JsonObject sensors_mems;
JsonArray sensors_mems_readings;
JsonObject sensors_mems_val;
JsonObject sensors_mems_max;
JsonObject sensors_mhz19;
JsonArray sensors_mhz19_readings;
JsonObject sensors_mhz19_co2;
JsonObject sensors_mhz19_temp;
JsonObject sensors_sht35;
JsonArray sensors_sht35_readings;
JsonObject sensors_sht35_temp;
JsonObject sensors_sht35_hum;
JsonObject sensors_tcs;
JsonArray sensors_tcs_readings;
JsonObject sensors_tcs_lux;
JsonObject sensors_tcs_colortemp;
JsonObject sensors_tcs_clear;
JsonObject sensors_tcs_red;
JsonObject sensors_tcs_green;
JsonObject sensors_tcs_blue;
JsonObject sensors_typk1;
JsonArray sensors_typk1_readings;
JsonObject sensors_typk1_temp_hot;
JsonObject sensors_typk1_temp_cold;
JsonObject sensors_typk1_adc;
JsonObject sensors_typk2;
JsonArray sensors_typk2_readings;
JsonObject sensors_typk2_temp;
JsonObject sensors_uv;
JsonArray sensors_uv_readings;
JsonObject sensors_uv_uv;
char jsonBuffer[4096];

#define BAUDRATE 9600  //Terminal Baudrate

//Configuration
int ads_sensor = 0;
int mhz_sensor = 0;
int ccs_sensor = 0;
int bme_sensor = 0;
int tcs_sensor = 0;
int dust_sensor = 0;
int uv_sensor = 0;
int sht_sensor = 0;
int wifi_sensor = 0;
int typk_sensor = 0;
int typk_spi_sensor = 0;
int mic_sensor = 0;
int oled_sensor = 0;
String ssid = "";
String ARDUINO_ID = "";

//https://forum.arduino.cc/t/what-is-the-esp8266-wemos-d1-mini-max-voltage-in/640646/4

//int bt = A0;
int bt = 12;
int start = 0;
int druck = 0;
int wlanNummer = 0;

unsigned long lastMillis;

//WIFI
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define SERVER_IP "192.168.1.1"

String STASSID = "";
String STAPSK = "luftqualitaet";

WiFiClient client;

HTTPClient http;

int httpCode = 0;
//MHZ
#include <MHZ19.h>
#include <SoftwareSerial.h>
#define RX_PIN 0  //MH-Z19 RX-PIN
#define TX_PIN 2  //MH-Z19 TX-PIN

MHZ19 myMHZ19;
SoftwareSerial mySerial(RX_PIN, TX_PIN);

String mhz_temp = "";
String mhz_co2 = "";

//CCS811
String ccs_co2 = "";
String ccs_tvoc = ""; 

#include <CCS811-SOLDERED.h>
CCS_811 ccs811Sensor;

//BME280
#include <BME280I2C.h>

BME280I2C::Settings settings(
  BME280::OSR_X1,
  BME280::OSR_X1,
  BME280::OSR_X1,
  BME280::Mode_Forced,
  BME280::StandbyTime_1000ms,
  BME280::Filter_Off,
  BME280::SpiEnable_False,
  BME280I2C::I2CAddr_0x76  // I2C address. I2C specific.
  //BME280I2C::I2CAddr_0x77  // I2C address. I2C specific.
);
BME280I2C bme(settings);

//TCS
String tcs_lux = "";
String tcs_color_temp = ""; 
#include <Adafruit_TCS34725.h>
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

//Dust
int Grove_Dust_DigitalPin = 14;
unsigned long Grove_Dust_lowpulseoccupancy = 0;
float Grove_Dust_ratio = 0;
float Grove_Dust_concentration = 0;
unsigned long sampletime_ms = 10000;
unsigned long duration;

//ads
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads;
int16_t adc0, adc1, adc2, adc3;

//uv
int sensorValue;
long sum = 0;
int uv_val = 0;
String uv_out = "";

//SHT35
#include <ArtronShop_SHT3x.h>
ArtronShop_SHT3x sht3x(0x45, &Wire); // ADDR: 0 => 0x44, ADDR: 1 => 0x45


//Typk

#include <SPI.h>
#include <Adafruit_MAX31855.h>
#define MAXDO   16 //D0 = Miso
#define MAXCS   15 //D8 = SS
#define MAXCLK  13 //D7 = SCK
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_MCP9600.h>
Adafruit_MCP9600 mcp;

Ambient_Resolution ambientRes = RES_ZERO_POINT_0625;


//Mic
float rms = 0;
float peak = 0;
int count = 0;
String data = "";

//OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#define i2c_Address 0x3c

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String zeile1 = "";
String zeile2 = "";
String zeile3 = "";
String zeile4 = "";

#include <EEPROM.h>
const int RESET_PIN = A0; 
// Schwellenwert für das "HIGH"-Signal. Ein Wert > 900 ist bei 1024-Bit-Auflösung ein guter Wert für HIGH.
const int RESET_THRESHOLD = 900; 
// Größe des EEPROM-Speichers in Bytes. 512 ist ein üblicher Wert.
const int EEPROM_SIZE = 512;

// Adresse im EEPROM für das Flag, das den ersten Start markiert.
const int firstStartFlagAddress = 0;
// Startadresse im EEPROM, an der die ID gespeichert wird.
const int idStorageAddress = 1;


void setup() {
  Serial.begin(BAUDRATE);
  mySerial.begin(BAUDRATE);  //Inialize

  Wire.begin();              //Inialize I2C Hardware
  EEPROM.begin(512);

  storage();

  configuration();

  if (oled_sensor){
    oled_INIT();
  }
  zeile1 = "SETUP Startet";
  zeile2 = "Bitte Warten";
  zeile4 = "okay";
  if (oled_sensor){
    oled_DATA();
  }
  if (ads_sensor){
    ads_INIT();
  }
  if (mhz_sensor){
    mhz_INIT();
  }
  if (ccs_sensor){
    ccs_INIT();
  }
  if (bme_sensor){
    bme_INIT();
  }
  if (tcs_sensor){
    tcs_INIT();
  }
  if (dust_sensor){
    dust_INIT();
  }
  if (sht_sensor){
    sht_INIT();
  }
  if (typk_sensor){
    typk_INIT();
  }
  if (typk_spi_sensor){
    typk_spi_INIT();
  }
  if (mic_sensor){
    mic_INIT();
  }
  start_INIT();

  if (wifi_sensor){
    wifi_INIT();
  }
  init_output();

  zeile1 = "LOOP Start";
  zeile2 = ARDUINO_ID;
  zeile4 = "okay";
  if (oled_sensor){
    oled_DATA();
  }
}

void loop() {
  zeile1 = "LOOP Laeuft";
  zeile2 = ARDUINO_ID;
  zeile4 = "okay";
  if (oled_sensor){
    oled_DATA();
  }
  
  if (dust_sensor){
    dust_collect_DATA();
  }

  if (mic_sensor){
    mic_collect_DATA();
  }
  
  //if (millis() - lastMillis >= 1*60*1000UL){
  if (millis() - lastMillis >= sampletime_ms) {
    lastMillis = millis();  //get ready for the next iteration

    Serial.println(ARDUINO_ID);
    Serial.println("--------------------------------------------------------------");
    generate_output();

    if (wifi_sensor){
      wifi_DATA();
    }
    
    if (mhz_sensor){
      mhz_DATA();
    }
    if (ccs_sensor){
      ccs_DATA();
    }
    if (bme_sensor){
      bme_DATA();
    }
    if (tcs_sensor){
      tcs_DATA();
    }
    if (dust_sensor){
      dust_DATA();
    }
    if (uv_sensor){
      uv_DATA();
    }
    if (sht_sensor){
      sht_DATA();
    }
    if (typk_sensor){
      typk_DATA();
    }
    if (typk_spi_sensor){
      typk_spi_DATA();
    }
    if (mic_sensor){
      mic_DATA();
    }

    Serial.println("--------------------------------------------------------------");
  }
  yield();
}
