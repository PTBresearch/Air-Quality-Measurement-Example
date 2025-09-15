void ccs_INIT() {
  Serial.println("CCS Beginn");
  zeile1 = "Starte Init: ";
  zeile2 = "CCS";
  zeile4 = "okay";

  if (oled_sensor){
    oled_DATA();
  }
  while (!ccs811Sensor.begin()) {
    Serial.print("CCS811 error. Please check wiring.");
    zeile1 = "Starte Init: ";
    zeile2 = "CCS Nicht gefunden";
    zeile4 = "error";
    if (oled_sensor){
      oled_DATA();
    }
    delay(1000);
  }
  zeile1 = "Starte Init: ";
  zeile2 = "CCS gefunden";
  zeile4 = "okay";

  if (oled_sensor){
    oled_DATA();
  }
  Serial.println("CCS Ready");
}

void ccs_DATA() {
  if (ccs811Sensor.dataAvailable()) {

    ccs811Sensor.readAlgorithmResults();

    ccs_co2 = ccs811Sensor.getCO2();
    ccs_tvoc = ccs811Sensor.getTVOC(); 

    sensors_ccs_co2["value"] = ccs_co2;
    sensors_ccs_tvoc["value"] = ccs_tvoc;
  }
}
