
void oled_INIT() {
  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
  //display.setContrast (0); // dim display
  display.display();
  display.clearDisplay();

}

void oled_DATA() {
  display.clearDisplay();
  // text display tests
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println(zeile1);
  display.println(zeile2);
  display.println(zeile3);
  display.println(zeile4);

  zeile1 = "";
  zeile2 = "";
  zeile3 = "error = reboot!!!";
  zeile4 = "";

  display.display();
}