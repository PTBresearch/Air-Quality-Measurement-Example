void generate_output() {
  Serial.println(daten.memoryUsage());

  serializeJson(daten, jsonBuffer);
  Serial.println(jsonBuffer);

}

void init_output() {
  // Top-Level-Informationen
  daten["arduino_board"] = ARDUINO_ID;
  daten["wifi"] = STASSID;
  sensors = daten.createNestedArray("sensors");

  // --- Sensor: bme280 ---
  sensors_bme280 = sensors.createNestedObject();
  sensors_bme280["name"] = "BME";
  sensors_bme280["sensor_model"] = "BME280";
  sensors_bme280_readings = sensors_bme280.createNestedArray("readings");

  sensors_bme_temp = sensors_bme280_readings.createNestedObject();
  sensors_bme_temp["type"] = "Temperature";
  sensors_bme_temp["value"] = "NULL";
  sensors_bme_temp["unit"] = "\\degreecelsius";

  sensors_bme_pres = sensors_bme280_readings.createNestedObject();
  sensors_bme_pres["type"] = "AmbientPressure";
  sensors_bme_pres["value"] = "NULL";
  sensors_bme_pres["unit"] = "\\pascal";

  sensors_bme_hum = sensors_bme280_readings.createNestedObject();
  sensors_bme_hum["type"] = "RelativeHumidity";
  sensors_bme_hum["value"] = "NULL";
  sensors_bme_hum["unit"] = "\\percent";

  // --- Sensor: ccs811 ---
  sensors_ccs811 = sensors.createNestedObject();
  sensors_ccs811["name"] = "CCS";
  sensors_ccs811["sensor_model"] = "CCS811";
  sensors_ccs811_readings = sensors_ccs811.createNestedArray("readings");

  sensors_ccs_co2 = sensors_ccs811_readings.createNestedObject();
  sensors_ccs_co2["type"] = "CO2";
  sensors_ccs_co2["value"] = "NULL";
  sensors_ccs_co2["unit"] = "\\partspermillion";

  sensors_ccs_tvoc = sensors_ccs811_readings.createNestedObject();
  sensors_ccs_tvoc["type"] = "TVOC";
  sensors_ccs_tvoc["value"] = "NULL";
  sensors_ccs_tvoc["unit"] = "\\partspermillion";

  // --- Sensor: dust ---
  sensors_dust = sensors.createNestedObject();
  sensors_dust["name"] = "Dust";
  sensors_dust["sensor_model"] = "Grove Dust";
  sensors_dust_readings = sensors_dust.createNestedArray("readings");

  sensors_dust_ratio = sensors_dust_readings.createNestedObject();
  sensors_dust_ratio["type"] = "Ratio";
  sensors_dust_ratio["value"] = "NULL";
  sensors_dust_ratio["unit"] = "\\percent";

  sensors_dust_conc = sensors_dust_readings.createNestedObject();
  sensors_dust_conc["type"] = "Concentration";
  sensors_dust_conc["value"] = "NULL";
  sensors_dust_conc["unit"] = "\\counts";

  sensors_dust_lpo = sensors_dust_readings.createNestedObject();
  sensors_dust_lpo["type"] = "LowPulseOccupancy";
  sensors_dust_lpo["value"] = "NULL";
  sensors_dust_lpo["unit"] = "\\percent";

  // --- Sensor: mems ---
  sensors_mems = sensors.createNestedObject();
  sensors_mems["name"] = "Mems";
  sensors_mems["sensor_model"] = "SPH0645";
  sensors_mems_readings = sensors_mems.createNestedArray("readings");

  sensors_mems_val = sensors_mems_readings.createNestedObject();
  sensors_mems_val["type"] = "Val";
  sensors_mems_val["value"] = "NULL";
  sensors_mems_val["unit"] = "\\decibel";

  sensors_mems_max = sensors_mems_readings.createNestedObject();
  sensors_mems_max["type"] = "Peak";
  sensors_mems_max["value"] = "NULL";
  sensors_mems_max["unit"] = "\\decibel";

  // --- Sensor: mhz19 ---
  sensors_mhz19 = sensors.createNestedObject();
  sensors_mhz19["name"] = "MHZ";
  sensors_mhz19["sensor_model"] = "MHZ19";
  sensors_mhz19_readings = sensors_mhz19.createNestedArray("readings");

  sensors_mhz19_co2 = sensors_mhz19_readings.createNestedObject();
  sensors_mhz19_co2["type"] = "CO2";
  sensors_mhz19_co2["value"] = "NULL";
  sensors_mhz19_co2["unit"] = "\\partspermillion";

  sensors_mhz19_temp = sensors_mhz19_readings.createNestedObject();
  sensors_mhz19_temp["type"] = "Temperature";
  sensors_mhz19_temp["value"] = "NULL";
  sensors_mhz19_temp["unit"] = "\\degreecelsius";

  // --- Sensor: sht35 ---
  sensors_sht35 = sensors.createNestedObject();
  sensors_sht35["name"] = "SHT";
  sensors_sht35["sensor_model"] = "SHT35";
  sensors_sht35_readings = sensors_sht35.createNestedArray("readings");

  sensors_sht35_temp = sensors_sht35_readings.createNestedObject();
  sensors_sht35_temp["type"] = "Temperature";
  sensors_sht35_temp["value"] = "NULL";
  sensors_sht35_temp["unit"] = "\\degreecelsius";

  sensors_sht35_hum = sensors_sht35_readings.createNestedObject();
  sensors_sht35_hum["type"] = "RelativeHumidity";
  sensors_sht35_hum["value"] = "NULL";
  sensors_sht35_hum["unit"] = "\\percent";

  // --- Sensor: tcs ---
  sensors_tcs = sensors.createNestedObject();
  sensors_tcs["name"] = "TCS";
  sensors_tcs["sensor_model"] = "TCS3472";
  sensors_tcs_readings = sensors_tcs.createNestedArray("readings");

  sensors_tcs_lux = sensors_tcs_readings.createNestedObject();
  sensors_tcs_lux["type"] = "Lux";
  sensors_tcs_lux["value"] = "NULL";
  sensors_tcs_lux["unit"] = "\\lx";

  sensors_tcs_colortemp = sensors_tcs_readings.createNestedObject();
  sensors_tcs_colortemp["type"] = "ColorTemperature";
  sensors_tcs_colortemp["value"] = "NULL";
  sensors_tcs_colortemp["unit"] = "\\kelvin";

  sensors_tcs_clear = sensors_tcs_readings.createNestedObject();
  sensors_tcs_clear["type"] = "ClearCol";
  sensors_tcs_clear["value"] = "NULL";
  sensors_tcs_clear["unit"] = "\\counts";

  sensors_tcs_red = sensors_tcs_readings.createNestedObject();
  sensors_tcs_red["type"] = "Red";
  sensors_tcs_red["value"] = "NULL";
  sensors_tcs_red["unit"] = "\\counts";

  sensors_tcs_green = sensors_tcs_readings.createNestedObject();
  sensors_tcs_green["type"] = "Green";
  sensors_tcs_green["value"] = "NULL";
  sensors_tcs_green["unit"] = "\\counts";

  sensors_tcs_blue = sensors_tcs_readings.createNestedObject();
  sensors_tcs_blue["type"] = "Blue";
  sensors_tcs_blue["value"] = "NULL";
  sensors_tcs_blue["unit"] = "\\counts";

  // --- Sensor: typk (1) ---
  sensors_typk1 = sensors.createNestedObject();
  sensors_typk1["name"] = "TYP_K";
  sensors_typk1["sensor_model"] = "MCP9600";
  sensors_typk1_readings = sensors_typk1.createNestedArray("readings");

  sensors_typk1_temp_hot = sensors_typk1_readings.createNestedObject();
  sensors_typk1_temp_hot["type"] = "Temperature";
  sensors_typk1_temp_hot["value"] = "NULL";
  sensors_typk1_temp_hot["unit"] = "\\degreecelsius";
  sensors_typk1_temp_hot["label"] = "hot junction";

  sensors_typk1_temp_cold = sensors_typk1_readings.createNestedObject();
  sensors_typk1_temp_cold["type"] = "Temperature";
  sensors_typk1_temp_cold["value"] = "NULL";
  sensors_typk1_temp_cold["unit"] = "\\degreecelsius";
  sensors_typk1_temp_cold["label"] = "cold junction";

  sensors_typk1_adc = sensors_typk1_readings.createNestedObject();
  sensors_typk1_adc["type"] = "ADC";
  sensors_typk1_adc["value"] = "NULL";
  sensors_typk1_adc["unit"] = "\\microvolts";

  // --- Sensor: typk (2) ---
  sensors_typk2 = sensors.createNestedObject();
  sensors_typk2["name"] = "TYP_K";
  sensors_typk2["sensor_model"] = "MAX31855";
  sensors_typk2_readings = sensors_typk2.createNestedArray("readings");

  sensors_typk2_temp = sensors_typk2_readings.createNestedObject();
  sensors_typk2_temp["type"] = "Temperature";
  sensors_typk2_temp["value"] = "NULL";
  sensors_typk2_temp["unit"] = "\\degreecelsius";

  // --- Sensor: uv ---
  sensors_uv = sensors.createNestedObject();
  sensors_uv["name"] = "UV";
  sensors_uv["sensor_model"] = "Grove - UV";
  sensors_uv_readings = sensors_uv.createNestedArray("readings");

  sensors_uv_uv = sensors_uv_readings.createNestedObject();
  sensors_uv_uv["type"] = "UV";
  sensors_uv_uv["value"] = "NULL";
  sensors_uv_uv["unit"] = "\\counts";
}