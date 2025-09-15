void mic_INIT() {

}

void mic_collect_DATA(){
  if (Serial.available() > 0) {
    String daten = Serial.readStringUntil('\n');
    data = daten.substring(0,5);
    rms = rms + data.toFloat();
    data = daten.substring(6,11);
    peak = peak + data.toFloat();
    count = count + 1;
  }
}

void mic_DATA() {

  rms = rms / count;
  peak = peak / count;

  sensors_mems_val["value"] = rms;
  sensors_mems_max["value"] = peak;


  rms = 0;
  peak = 0;
  count = 0;
}