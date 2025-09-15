void typk_spi_INIT() {
  Serial.println("Typk Beginn");

  Serial.print("Initializing sensor...");
  if (!thermocouple.begin()) {
    Serial.println("ERROR.");
    while (1) delay(10);
  }

  Serial.println("Typk Ready");
}

void typk_spi_DATA() {
  double c = thermocouple.readCelsius();
  if (isnan(c)) {
    //Serial.println("Thermocouple fault(s) detected!");
    //uint8_t e = thermocouple.readError();
    //if (e & MAX31855_FAULT_OPEN) Serial.println("FAULT: Thermocouple is open - no connections.");
    //if (e & MAX31855_FAULT_SHORT_GND) Serial.println("FAULT: Thermocouple is short-circuited to GND.");
    //if (e & MAX31855_FAULT_SHORT_VCC) Serial.println("FAULT: Thermocouple is short-circuited to VCC.");
  } else {
    sensors_typk2_temp["value"] = c;
  }

  delay(1000);
}

