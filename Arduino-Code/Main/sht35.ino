void sht_INIT() {
  if (!sht3x.begin()) {
    Serial.println("SHT3x not found !");
    delay(1000);
  }
}

void sht_DATA() {

  if (sht3x.measure()) {
    float shttemp = sht3x.temperature();
    float shthum = sht3x.humidity();

    sensors_sht35_temp["value"] = shttemp;
    sensors_sht35_hum["value"] = shthum;

  }
}
