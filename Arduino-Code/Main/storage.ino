void delEeprom(){
  for (int i = 0 ; i < EEPROM_SIZE ; i++) {
        EEPROM.write(i, 0);
      }
      
      // Änderungen im EEPROM speichern
      if (EEPROM.commit()) {
        Serial.println("EEPROM wurde erfolgreich geloescht.");
      } else {
        Serial.println("FEHLER! Loeschen des EEPROM fehlgeschlagen.");
      }
      
      Serial.println("Bitte entfernen Sie jetzt die Verbindung an Pin A0 und starten Sie den Controller neu.");
}

void readEeprom(){
      Serial.println("Geraet wurde bereits konfiguriert.");
    
      int storedId;
      // KORREKTUR: EEPROM.get() verwenden, um den ganzen Integer zu lesen.
      storedId = EEPROM.read(idStorageAddress);
      
      Serial.print("Die gespeicherte ID lautet: ");
      Serial.println(storedId);

      ID = storedId;
}

void storage(){
  while (!Serial) {
    ; 
  }
  
  if (analogRead(RESET_PIN) > RESET_THRESHOLD) {
    Serial.println("Reset-Signal erkannt. Loesche EEPROM...");
    delEeprom();
    while (true);
  }

  // ---- 2. PRÜFUNG: IST DAS GERÄT BEREITS KONFIGURIERT? ----
  byte firstStartFlag = EEPROM.read(firstStartFlagAddress);

  if (firstStartFlag != 'Y') { // 'Y' für "Yes, configured"
    // ---- ERSTER START ----
    Serial.println("Erster Start des Geraets (oder nach Reset).");
    Serial.println("Bitte geben Sie eine ID (ganze Zahl) ein und senden Sie sie ab:");

    while (Serial.available() == 0) {
      // Warten auf Eingabe
    }

    int idToStore = Serial.parseInt();

    // KORREKTUR: EEPROM.put() verwenden, um einen ganzen Integer zu speichern.
    EEPROM.put(idStorageAddress, idToStore);

    Serial.print("ID '");
    Serial.print(idToStore);
    Serial.println("' wird im EEPROM gespeichert.");
    
    // Setze das Flag für den ersten Start
    EEPROM.write(firstStartFlagAddress, 'Y');

    // WICHTIG: Änderungen im EEPROM committen (speichern)
    if (EEPROM.commit()) {
      Serial.println("EEPROM erfolgreich geschrieben.");
    } else {
      Serial.println("FEHLER! EEPROM konnte nicht geschrieben werden.");
    }

    readEeprom();

  } else {
    readEeprom();
  }
}