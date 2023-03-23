
void init_ota_local() {

  init_ota(gv_clientname);

  ArduinoOTA.onStart([]() { // switch off all the PWMs during upgrade
    motor_stop_all();

  });

  ArduinoOTA.onEnd([]() {

  });

}
