void check_sched() {

  if (gv_1s) {
    check_state_timer_1s();

    check_window( );

    if (gv_pub_sens) {
      pub_sens();
      gv_pub_sens = false;
    }

    gv_1s = false;
  }

  if (gv_1m) {
    pub_sens();

    gv_1m = false;
  }

  if (gv_10m) {

    gv_10m = false;
  }
}


void do_sched_1s() {
  gv_1s = true;

  gv_1m_cnt += 1;
  if ( gv_1m_cnt >= 60 ) {
    gv_1m = true;
    gv_1m_cnt = 0;

    gv_10m_cnt += 1;
    if ( gv_10m_cnt >= 10 ) {
      gv_10m = true;
      gv_10m_cnt = 0;
    }
  }
}

void init_sched() {
  sched_1s.attach(1, do_sched_1s);
  gv_1m = true;
  gv_10m = true;
}
