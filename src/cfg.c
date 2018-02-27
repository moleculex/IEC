#include "cfg.h"

void cfg_map(void)
{

}

void cfg_default(void)
{
	_cfg.alarm_reset_interval = 10; //minutes
	_cfg.alarm_status_interval = 54;//60; //minutes
	_cfg.solenoid_timeout = 5000; //ms
	_cfg.alarms_in_use.alarms = 0x7F; //bitmask
	_cfg.reboot_interval = 10080; //minutes
	/*_cfg.alarms_in_use.door_sensor = 1;
	_cfg.alarms_in_use.door_sensor_presence = 1;
	_cfg.alarms_in_use.vibration_sensor = 1;
	_cfg.alarms_in_use.vibration_sensor_presence = 1;
	_cfg.alarms_in_use.solenoid_presence = 1;
	_cfg.alarms_in_use.battery_status = 1;*/
}

uint16_t cfg_byteSwap(uint16_t val)
{
	return (val << 8) | (val >> 8 );
}
