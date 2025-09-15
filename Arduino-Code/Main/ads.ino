void ads_INIT() {
  Serial.println("ADS Beginn");
  zeile1 = "Starte Init: ";
  zeile2 = "ADS";
  zeile4 = "okay";

  if (oled_sensor){
    oled_DATA();
  }
  
  while (!ads.begin()) {
    Serial.println("ADS Nicht gefunden");
    zeile1 = "Starte Init: ";
    zeile2 = "ADS Nicht gefunden";
    zeile4 = "error";

    if (oled_sensor){
      oled_DATA();
    }
    delay(1000);
  }
  ads.setGain(GAIN_ONE);

  Serial.println("ADS Ready");
  zeile1 = "Starte Init: ";
  zeile2 = "ADS gefunden";
  zeile4 = "okay";

  if (oled_sensor){
    oled_DATA();
  }
}

void ads_DATA() {
  //adc0 = ads.readADC_SingleEnded(0);
}

