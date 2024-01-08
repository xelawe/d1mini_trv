void ICACHE_RAM_ATTR window_isr() {
  gv_window_changed = true;
}

void check_window() {
  go_window1.update();
  go_window2.update();

  if (go_window1.changed() || go_window2.changed()) {
    gv_window_changed = true;
  }

  if (!gv_window_changed) {
    return;
  }

  //bool lv_win1 = digitalRead(D5);
  //bool lv_win2 = digitalRead(D6);

  bool lv_win1 = go_window1.read();
  bool lv_win2 = go_window2.read();

  if ((lv_win1 != gv_win1) || (lv_win2 != gv_win2)) {
    gv_pub_sens = true;
  }

  gv_win1 = lv_win1;
  gv_win2 = lv_win2;

  if (gv_log_mqtt == true) {
    Serial.print("Window 1: ");
    Serial.print(gv_win1);
    Serial.print(", Window 2: ");
    Serial.println(gv_win2);
  }

  gv_window_changed = false;
}

void init_window() {

  // pinMode(D5, INPUT_PULLUP);
  // pinMode(D6, INPUT_PULLUP);

  //attachInterrupt(D5, window_isr, CHANGE);
  //attachInterrupt(D6, window_isr, CHANGE);

  go_window1.attach(D5, INPUT_PULLUP);  // USE INTERNAL PULL-UP
    // DEBOUNCE INTERVAL IN MILLISECONDS
  go_window1.interval(10);               // interval in ms
  go_window2.attach(D6, INPUT_PULLUP);  // USE INTERNAL PULL-UP
    // DEBOUNCE INTERVAL IN MILLISECONDS
  go_window2.interval(10);  // interval in ms

  gv_window_changed = true;
}
