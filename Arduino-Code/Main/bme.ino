void bme_INIT() {
  Serial.println("BME Beginn");
  zeile1 = "Starte Init: ";
  zeile2 = "BME";
  zeile4 = "okay";

  if (oled_sensor){
    oled_DATA();
  }

  while (!bme.begin()) {
    Serial.println("Could not find BME280I2C sensor!");
    zeile1 = "Starte Init: ";
    zeile2 = "BME nicht gefunden";
    zeile4 = "error";

    if (oled_sensor){
      oled_DATA();
    }
    delay(1000);
  }

  switch (bme.chipModel()) {
    case BME280::ChipModel_BME280:
      Serial.println("Found BME280 sensor! Success.");
      break;
    case BME280::ChipModel_BMP280:
      Serial.println("Found BMP280 sensor! No Humidity available.");
      break;
    default:
      Serial.println("Found UNKNOWN sensor! Error!");
  }

  zeile1 = "Starte Init: ";
  zeile2 = "BME gefunden";
  zeile4 = "okay";

  if (oled_sensor){
    oled_DATA();
  }
  settings.tempOSR = BME280::OSR_X4;

  bme.setSettings(settings);

  Serial.println("BME Ready");
}

void bme_DATA() {
  float temp(NAN), hum(NAN), pres(NAN);
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);

  bme.read(pres, temp, hum, tempUnit, presUnit);

  sensors_bme_temp["value"] = temp;
  sensors_bme_pres["value"] = pres * 100;
  sensors_bme_hum["value"] = hum;

}