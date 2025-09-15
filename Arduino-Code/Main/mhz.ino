void mhz_INIT() {
  Serial.println("MHZ Beginn");
  zeile1 = "Starte Init: ";
  zeile2 = "MHZ";
  zeile4 = "okay";

  if (oled_sensor){
    oled_DATA();
  }

  myMHZ19.begin(mySerial);
  myMHZ19.autoCalibration();    

  zeile1 = "Starte Init: ";
  zeile2 = "MHZ gefunden";
  zeile4 = "okay";

  if (oled_sensor){
    oled_DATA();
  }
  Serial.println("MHZ Ready");
}

void mhz_DATA() {
  mhz_co2 = myMHZ19.getCO2();
  mhz_temp = myMHZ19.getTemperature();
  
  sensors_mhz19_co2["value"] = mhz_co2;
  sensors_mhz19_temp["value"] = mhz_temp;

}