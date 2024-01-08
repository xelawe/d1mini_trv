void setup() {

  cy_serial::start(gc_file);

  motor_stop_all();
  delay(500);

  init_window();

  init_ina219();

  wifi_init(gc_hostname);
  delay(500);

  init_ota_local();

  init_mqtt_local();

  init_state();

  check_window();

  init_sched();

  //gv_state = 2; // calibration
  gv_dest_pos = 0;
  gv_state = 6;
}

void loop() {

  check_ota();

  check_mqtt_reset();

  check_state();

  check_sched();

  check_window();

  if (gv_pub_sens) {
    pub_sens();
    gv_pub_sens = false;
  }
}
