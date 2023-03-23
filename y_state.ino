void state_motor_stop() {
  Ventil.setmotor(_STOP);
  gv_motor_run = false;
  gv_state = 1;
  gv_pub_sens = true;
}

void check_state( ) {

  if (gv_State_millis < millis()) {
    return;
  }

  gv_state_old = gv_state;

  if (gv_state != 1) {
    check_ina219( );
  }

  switch (gv_state) {
    case 0: //do stop
      state_motor_stop();
      break;
    case 1: // stopped
      if (gv_state == 1) {
        if ( gv_dest_pos_mqtt != -1 ) {
          gv_dest_pos = gv_dest_pos_mqtt;
          gv_state = 6;
          gv_dest_pos_mqtt = -1;
        }
      }

      if (gv_state == 1) {
        if ( gv_cal_mqtt == true ) {
          gv_state = 2; // calibration
          gv_cal_mqtt = false;
        }
      }
      break;
    case 2: // do calibration
      gv_act_pos = -1;
      Ventil.setmotor(_CW, 100);
      gv_motor_dir = true;
      gv_motor_run = true;
      gv_state = 3;
    case 3: // calibration opening
      if (current_mA > gv_max_curr) {
        Ventil.setmotor(_STOP);
        gv_motor_run = false;
        gv_state = 4;
      }
      break;
    case 4: // do calibration close
      Ventil.setmotor(_CCW, 100);
      gv_motor_dir = false;
      gv_motor_run = true;
      gv_max_time = 0;
      gv_state = 5;

      break;

    case 5: // calibration closing
      if (current_mA > gv_max_curr) {
        state_motor_stop();
        gv_act_pos = 0;
        Serial.print("Max time: ");
        Serial.println(gv_max_time);
      }
      break;
    case 6: // do positioning
      if (gv_act_pos == gv_dest_pos) {
        gv_state = 1;
        break;
      }

      gv_old_pos = gv_act_pos;

      if (gv_act_pos > gv_dest_pos) {
        // millis für neue Position errechnen
        int lv_diff_pos = gv_act_pos - gv_dest_pos;
        int lv_diff_millis = (( gv_max_time * 1000 ) / 100 ) * lv_diff_pos;
        gv_old_millis = millis();
        gv_dest_millis = gv_old_millis + lv_diff_millis;

        Ventil.setmotor(_CCW, 100);
        gv_motor_dir = false;
        gv_motor_run = true;
        gv_state = 7;
      } else {
        // millis für neue Position errechnen
        int lv_diff_pos = gv_dest_pos - gv_act_pos;
        int lv_diff_millis = (( gv_max_time * 1000 ) / 100 ) * lv_diff_pos;
        gv_old_millis = millis();
        gv_dest_millis = gv_old_millis + lv_diff_millis;

        Ventil.setmotor(_CW, 100);
        gv_motor_dir = true;
        gv_motor_run = true;
        gv_state = 8;
      }
      //gv_pos_time = gv_max_time - map(gv_dest_pos, 0, 100, gv_max_time, 0);
      break;
    case 7: // go to position closing
      if (current_mA > gv_max_curr) {
        state_motor_stop();
        gv_act_pos = 0;
        break;
      }

      // aktuelle Position berechnen
      gv_act_pos = gv_old_pos - (( 100 * ( millis() - gv_old_millis ) ) / ( gv_max_time * 1000 ) );

      if (gv_act_pos <= gv_dest_pos) {
        state_motor_stop();
        break;
      }
      break;
    case 8: // go to position opening
      if (current_mA > gv_max_curr) {
        state_motor_stop();
        gv_act_pos = 100;

        break;
      }

      // aktuelle Position berechnen
      gv_act_pos = gv_old_pos + (( 100 * ( millis() - gv_old_millis ) ) / ( gv_max_time * 1000 ) );

      if (gv_act_pos >= gv_dest_pos) {
        state_motor_stop();
        break;
      }
      break;
  }

  // Serial.println();

  if (gv_state_old != gv_state) {
    gv_pub_sens = true;
  }

  set_state_tact();

  return;

//  if (current_mA > 30) {
//    Ventil.setmotor(_STOP);
//    gv_motor_run = false;
//  } else {
//    if (!gv_motor_run) {
//      if ( gv_motor_dir ) {
//        Ventil.setmotor(_CCW, 100);
//        //Ventil.setmotor(_STOP);
//        gv_motor_dir = false;
//      } else {
//        Ventil.setmotor(_CW, 100);
//        gv_motor_dir = true;
//      }
//      gv_motor_run = true;
//    }
//  }
//
//  if (!gv_motor_run) {
//    Serial.println("Motor stop");
//  } else {
//    if ( gv_motor_dir ) {
//      Serial.println("Valve opening");
//    } else {
//      Serial.println("Valve closing");
//    }
//  }

}

void check_state_timer_1s() {
  print_ina219();
  Serial.print("aktuelle Position ");
  Serial.println(gv_act_pos);
  switch (gv_state) {
    case 5: // calibration closing
      gv_max_time = gv_max_time + 1;
      break;
    case 7:
      // gv_act_pos = gv_act_pos - ( 100 / gv_max_time );

      break;
    case 8:
      // gv_act_pos = gv_act_pos + ( 100 / gv_max_time );

      break;
  }

}

void init_state() {
  gv_state = 0;
  gv_max_time = 46;
  set_state_tact();
}


void set_state_tact() {
  gv_State_millis = millis() + 50; // Messung alle 50 ms
}
