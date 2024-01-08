void ICACHE_RAM_ATTR window_isr() {
  gv_window_changed = true;
}

void check_window( ) {
  if (!gv_window_changed) {
    return;
  }

  gv_win1 = digitalRead(D5);
  gv_win2 = digitalRead(D6);


  if ( gv_log_mqtt ==  true) {
    Serial.print("Window 1: ");
    Serial.print(gv_win1);
    Serial.print(", Window 2: ");
    Serial.println(gv_win2);
  }

  gv_pub_sens = true;
  gv_window_changed = false;
}

void init_window() {

  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);

  attachInterrupt(D5, window_isr, CHANGE);
  attachInterrupt(D6, window_isr, CHANGE);

  gv_window_changed = true;
}
