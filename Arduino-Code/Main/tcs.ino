void tcs_INIT() {
  Serial.println("TCS Beginn");
  zeile1 = "Starte Init: ";
  zeile2 = "TCS";
  zeile4 = "okay";

  if (oled_sensor){
    oled_DATA();
  }
  while (!tcs.begin()) {
    Serial.println("ERROR: TCS34725 NOT FOUND !!!");
    zeile1 = "Starte Init: ";
    zeile2 = "TCS Nicht gefunden";
    zeile4 = "error";

    if (oled_sensor){
      oled_DATA();
    }
    delay(1000);
  }
  zeile1 = "Starte Init: ";
  zeile2 = "TCS Gefunden";
  zeile4 = "okay";

  if (oled_sensor){
    oled_DATA();
  }
  Serial.println("TCS Ready");
}

void tcs_DATA() {
  uint16_t clearcol, red, green, blue;
  tcs.getRawData(&red, &green, &blue, &clearcol);
  tcs_lux = tcs.calculateLux(red, green, blue);
  tcs_color_temp = tcs.calculateColorTemperature_dn40(red, green, blue, clearcol); 

  sensors_tcs_lux["value"] = tcs_lux;
  sensors_tcs_colortemp["value"] = tcs_color_temp;
  sensors_tcs_clear["value"] = clearcol;
  sensors_tcs_red["value"] = red;
  sensors_tcs_green["value"] = green;
  sensors_tcs_blue["value"] = blue;


}
