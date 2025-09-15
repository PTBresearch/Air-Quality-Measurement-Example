
void uv_INIT() {

}

void uv_collect_DATA(){
  //if (uv_val < 512) {
  //  uv_val++;
  //  adc0 = ads.readADC_SingleEnded(0);
  //  sensorValue = adc0 / 16;
  //  if (sensorValue < 0) {
  //    sensorValue = 1;
  //  }
  //  sum = sensorValue + sum;
  //}
}

void uv_DATA() {


  sum = 0; // Setze die Summe vor der Schleife zurück

  for (int i = 0; i < 1024; i++) // 1024 Messungen für einen stabileren Wert
  {
    adc0 = ads.readADC_SingleEnded(0);
    sum = adc0 + sum;
    delay(2);
  }

  long meanVal = sum / 1024; // Mittelwert der ADC-Messungen
  float voltage_mV = (meanVal * 4096.0) / 32767.0;
  uv_out = (voltage_mV / 1000.0) / 0.1;

  sensors_uv_uv["value"] = uv_out;

  sum = 0;
  uv_val = 0;
}

