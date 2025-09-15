
void dust_INIT() {
  Serial.println("Dust Beginn");

  pinMode(Grove_Dust_DigitalPin, INPUT);
  zeile1 = "Starte Init: ";
  zeile2 = "Staub";
  zeile4 = "okay";

  if (oled_sensor){
    oled_DATA();
  }
  Serial.println("Dust Beginn");
}

void dust_collect_DATA(){
  duration = pulseIn(Grove_Dust_DigitalPin, LOW);
  Grove_Dust_lowpulseoccupancy = Grove_Dust_lowpulseoccupancy + duration;
}

void dust_DATA() {
  Grove_Dust_ratio = Grove_Dust_lowpulseoccupancy / (sampletime_ms * 10.0);  // Integer percentage 0=>100
  Grove_Dust_concentration = 1.1 * pow(Grove_Dust_ratio, 3) - 3.8 * pow(Grove_Dust_ratio, 2) + 520 * Grove_Dust_ratio + 0.62;

  sensors_dust_ratio["value"] = Grove_Dust_ratio;
  sensors_dust_conc["value"] = Grove_Dust_concentration;
  sensors_dust_lpo["value"] = Grove_Dust_lowpulseoccupancy;


  Grove_Dust_lowpulseoccupancy = 0;
}
