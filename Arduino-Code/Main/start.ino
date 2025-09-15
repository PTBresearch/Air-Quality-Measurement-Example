void start_INIT() {
  delay(1000);

  Serial.println("Start");

  zeile1 = "Wlan auswählen!";
  zeile2 = "";
  zeile4 = "okay";

  if (oled_sensor){
    oled_DATA();
  }
  pinMode(bt, INPUT);

  while (start == 0) {
    druck = 0;
    //Wenn button gedrück dann Druck hochzählen
    //Serial.println(digitalRead(bt));
    while (digitalRead(bt) < 1) {
      //Serial.println(druck);
      druck++;
      delay(10);
      //Sofern druck über 1000 wieder auf 0
      if (druck > 1000) {
        druck = 0;
      }
    }

    //Wenn gedrück wurde Wlan wechseln oder Starten
    if (druck > 1) {

      //Wenn lange gedrückt wurde Starten
      if (druck > 70) {
        Serial.println("Start");
        zeile1 = "Starte Init: ";
        zeile2 = "";
        zeile4 = "okay";

        if (oled_sensor){
          oled_DATA();
        }
        start++;
      } else {
        //Wenn kurz gedrück wurde Wlan auswählen
        wlanNummer++;

        switch (wlanNummer) {
          case 1:
            STASSID = "c24059";
            zeile1 = "WLAN: ";
            zeile1 = zeile1 + STASSID;
            zeile2 = "";
            zeile4 = "okay";

            if (oled_sensor){
              oled_DATA();
            }
            break;
          case 2:
            STASSID = "c24148";
            zeile1 = "WLAN: ";
            zeile1 = zeile1 + STASSID;
            zeile2 = "";
            zeile4 = "okay";

            if (oled_sensor){
              oled_DATA();
            }
            break;
          case 3:
            STASSID = "c24062";
            zeile1 = "WLAN: ";
            zeile1 = zeile1 + STASSID;
            zeile2 = "";
            zeile4 = "okay";

            if (oled_sensor){
              oled_DATA();
            }
            break;
          case 4:
            STASSID = "c24150";
            zeile1 = "WLAN: ";
            zeile1 = zeile1 + STASSID;
            zeile2 = "";
            zeile4 = "okay";

            if (oled_sensor){
              oled_DATA();
            }
            break;
          case 5:
            STASSID = "c24061";
            zeile1 = "WLAN: ";
            zeile1 = zeile1 + STASSID;
            zeile2 = "";
            zeile4 = "okay";

            if (oled_sensor){
              oled_DATA();
            }
            break;
          case 6:
            STASSID = "c24153";
            zeile1 = "WLAN: ";
            zeile1 = zeile1 + STASSID;
            zeile2 = "";
            zeile4 = "okay";

            if (oled_sensor){
              oled_DATA();
            }
            break;
          case 7:
            STASSID = "c25875";
            zeile1 = "WLAN: ";
            zeile1 = zeile1 + STASSID;
            zeile2 = "";
            zeile4 = "okay";

            if (oled_sensor){
              oled_DATA();
            }
            break;
          case 8:
            STASSID = "c25876";
            zeile1 = "WLAN: ";
            zeile1 = zeile1 + STASSID;
            zeile2 = "";
            zeile4 = "okay";

            if (oled_sensor){
              oled_DATA();
            }
            break;
          case 9:
            STASSID = "c25877";
            zeile1 = "WLAN: ";
            zeile1 = zeile1 + STASSID;
            zeile2 = "";
            zeile4 = "okay";

            if (oled_sensor){
              oled_DATA();
            }
            break;
          case 10:
            STASSID = "n23767";
            zeile1 = "WLAN: ";
            zeile1 = zeile1 + STASSID;
            zeile2 = "";
            zeile4 = "okay";

            if (oled_sensor){
              oled_DATA();
            }
            break;
          default:
            STASSID = "";
            zeile1 = "WLAN: ";
            zeile1 = zeile1 + STASSID;
            zeile2 = "";
            zeile4 = "okay";

            if (oled_sensor){
              oled_DATA();
            }
            wlanNummer = 0;
            break;
        }
        Serial.println(STASSID);
        //Serial.println(wlanNummer);
      }
      //delay(1000);
    }
    yield();
  }
}