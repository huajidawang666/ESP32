#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED
//brightness
void brightness_init(); void brightness_loop();
//time & date
void timeanddate_init(); void timeanddate_loop();

extern bool finishSettingMission;
#endif