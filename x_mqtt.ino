
#include "cy_mqtt_v1.h"


const char gc_stopic_pos[] PROGMEM = "pos"; // set new position
const char gc_stopic_cal[] PROGMEM = "cal"; // calibrate
const char* const gt_stopic[] PROGMEM = { gc_stopic_pos, gc_stopic_cal };

char gv_stopic[sizeof(gt_stopic)][MQTT_TOPSZ];

char gv_sbuffer[MQTT_TOPSZ];// buffer for reading the string to (needs to be large enough to take the longest string

char gv_ptopic[MQTT_TOPSZ];

int gv_dest_pos_mqtt;
boolean gv_cal_mqtt;

boolean gv_pub_sens;
