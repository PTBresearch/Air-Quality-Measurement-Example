void wifi_INIT() {
  Serial.println("WIFI Beginn");
  Serial.println(STASSID);
  Serial.println(STAPSK);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  zeile1 = "Starte Init: ";
  zeile2 = "Wifi: ";
  zeile2 = zeile2 + STASSID;
  zeile4 = "okay";

  if (oled_sensor){
    oled_DATA();
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    zeile1 = "Starte Init: ";
    zeile2 = "Wifi verbinde.";
    zeile4 = "okay";

    if (oled_sensor){
      oled_DATA();
    }
  }
  Serial.println("");
  Serial.println(WiFi.localIP());
  Serial.println("WIFI Beginn Ende");
  zeile1 = "Starte Init: ";
  zeile2 = "Wifi verbunden.";
  zeile4 = "okay";

  if (oled_sensor){
    oled_DATA();
  }
}

void wifi_DATA() {
  if ((WiFi.status() != WL_CONNECTED)) {
    wifi_INIT();
  }
  http.begin(client, "http://" SERVER_IP ":30001/sensor");  // HTTP



  httpCode = http.POST(jsonBuffer);

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      const String& payload = http.getString();
    }
  } else {
      zeile1 = "Wifi Error: HTTP";
      zeile2 = "Post Failed";
      zeile4 = "error";

      if (oled_sensor){
        oled_DATA();
      }
  }
  httpCode = 0;
  http.end();
  client.stop();
}