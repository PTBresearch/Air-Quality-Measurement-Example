int ID;
void configuration() {

  switch (ID) {
    case 0:
      ARDUINO_ID = "Arduino_0"; 
      
	  ads_sensor = 1;
      mhz_sensor = 1;
      ccs_sensor = 1;
      bme_sensor = 1;
      tcs_sensor = 1;
      dust_sensor = 1;
      uv_sensor = 1;
      sht_sensor = 1;
      typk_spi_sensor = 1; //SPI
      typk_sensor = 0;
      wifi_sensor = 1;
      oled_sensor = 1;
      mic_sensor = 0;
      break;
    case 1:
      ARDUINO_ID = "Arduino_1";
	  
	  ads_sensor = 1;
      mhz_sensor = 1;
      ccs_sensor = 1;
      bme_sensor = 1;
      tcs_sensor = 1;
      dust_sensor = 1;
      uv_sensor = 1;
      sht_sensor = 0;
      typk_spi_sensor = 0; //SPI
      typk_sensor = 1;
      wifi_sensor = 1;
      oled_sensor = 1;
      mic_sensor = 0;
      break; 

    case 2:
      ARDUINO_ID = "Arduino_2";

      ads_sensor = 1;
      mhz_sensor = 1;
      ccs_sensor = 1;
      bme_sensor = 1;
      tcs_sensor = 1;
      dust_sensor = 1;
      uv_sensor = 1;
      sht_sensor = 0;
      typk_spi_sensor = 0; //SPI
      typk_sensor = 1;
      wifi_sensor = 1;
      oled_sensor = 1;
      mic_sensor = 0;
      break;

    case 3:
      ARDUINO_ID = "Arduino_3";

      ads_sensor = 1;
      mhz_sensor = 1;
      ccs_sensor = 1;
      bme_sensor = 1;
      tcs_sensor = 1;
      dust_sensor = 1;
      uv_sensor = 1;
      sht_sensor = 1;
      typk_spi_sensor = 0; //SPI
      typk_sensor = 1;
      wifi_sensor = 1;
      oled_sensor = 1;
      mic_sensor = 0;
      break; 

    case 4:
      ARDUINO_ID = "Arduino_4";
      
      ads_sensor = 1;
      mhz_sensor = 1;
      ccs_sensor = 1;
      bme_sensor = 1;
      tcs_sensor = 1;
      dust_sensor = 1;
      uv_sensor = 1;
      sht_sensor = 0;
      typk_spi_sensor = 0; //SPI
      typk_sensor = 1;
      wifi_sensor = 1;
      oled_sensor = 1;
      mic_sensor = 0;
      break;

    case 5:
      ARDUINO_ID = "Arduino_5";
      
      ads_sensor = 1;
      mhz_sensor = 1;
      ccs_sensor = 1;
      bme_sensor = 1;
      tcs_sensor = 1;
      dust_sensor = 1;
      uv_sensor = 1;
      sht_sensor = 0;
      typk_spi_sensor = 0; //SPI
      typk_sensor = 1;
      wifi_sensor = 1;
      oled_sensor = 1;
      mic_sensor = 0;
      break;

    case 6:
      ARDUINO_ID = "Arduino_6";
      
      ads_sensor = 1;
      mhz_sensor = 1;
      ccs_sensor = 1;
      bme_sensor = 1;
      tcs_sensor = 1;
      dust_sensor = 1;
      uv_sensor = 1;
      sht_sensor = 0;
      typk_spi_sensor = 0; //SPI
      typk_sensor = 1;
      wifi_sensor = 1;
      oled_sensor = 1;
      mic_sensor = 0;
      break; 

    case 7:
      ARDUINO_ID = "Arduino_7";
      
  	  ads_sensor = 1;
      mhz_sensor = 1;
      ccs_sensor = 1;
      bme_sensor = 1;
      tcs_sensor = 1;
      dust_sensor = 1;
      uv_sensor = 1;
      sht_sensor = 0;
      typk_spi_sensor = 0; //SPI
      typk_sensor = 1;
      wifi_sensor = 1;
      oled_sensor = 1;
      mic_sensor = 0;
      break; 

    case 8:
      ARDUINO_ID = "Arduino_8";
      
      ads_sensor = 1;
      mhz_sensor = 1;
      ccs_sensor = 1;
      bme_sensor = 1;
      tcs_sensor = 1;
      dust_sensor = 1;
      uv_sensor = 1;
      sht_sensor = 0;
      typk_spi_sensor = 0; //SPI
      typk_sensor = 1;
      wifi_sensor = 1;
      oled_sensor = 1;
      mic_sensor = 0;
      break; 

    case 9:
      ARDUINO_ID = "Arduino_9";

      ads_sensor = 1;
      mhz_sensor = 1;
      ccs_sensor = 1;
      bme_sensor = 1;
      tcs_sensor = 1;
      dust_sensor = 1;
      uv_sensor = 1;
      sht_sensor = 0;
      typk_spi_sensor = 0; //SPI
      typk_sensor = 1;
      wifi_sensor = 1;
      oled_sensor = 1;
      mic_sensor = 0;
      break; 

    case 10:
      ARDUINO_ID = "Arduino_10";

      wifi_sensor = 1;
      mic_sensor = 1;
      ssid = "c24059";
      break;

    case 11:
      ARDUINO_ID = "Arduino_11";

      wifi_sensor = 1;
      mic_sensor = 1;
      ssid = "c24148";
      break;

    case 12:
      ARDUINO_ID = "Arduino_12";

      wifi_sensor = 1;
      mic_sensor = 1;
      ssid = "c24062";
      break;

    case 13:
      ARDUINO_ID = "Arduino_13";

      wifi_sensor = 1;
      mic_sensor = 1;
      ssid = "c24150";
      break;

    case 14:
      ARDUINO_ID = "Arduino_14";

      wifi_sensor = 1;
      mic_sensor = 1;
      ssid = "c24061";
      break;

    case 15:
      ARDUINO_ID = "Arduino_15";

      wifi_sensor = 1;
      mic_sensor = 1;
      ssid = "c24153";
      break;

    case 16:
      ARDUINO_ID = "Arduino_16";

      wifi_sensor = 1;
      mic_sensor = 1;
      ssid = "c25875";
      break;

    case 17:
      ARDUINO_ID = "Arduino_17";

      wifi_sensor = 1;
      mic_sensor = 1;
      ssid = "c25876";
      break;

    case 18:
      ARDUINO_ID = "Arduino_18";

      wifi_sensor = 1;
      mic_sensor = 1;
      ssid = "c25877";
      break;

    case 19:
      ARDUINO_ID = "Arduino_19";

      wifi_sensor = 1;
      mic_sensor = 1;
      ssid = "n23767";
      break;

    default:
      ARDUINO_ID = "Arduino_X";

      bme_sensor = 1;
      wifi_sensor = 1;
      ssid="c24059";
      start = 1;
      break; 
  }

  Serial.println(ARDUINO_ID);

  if (ssid != ""){
    start = 1;
    STASSID = ssid;
  }

}
