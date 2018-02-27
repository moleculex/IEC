#ifndef __BATTERY_H
#define __BATTERY_H

#define BATTERY_PRESENCE 0x01
#define BATTERY_LEVEL 0x02
#define BATTERY_DEEP_CHARGE 0x03


typedef struct{
	char strLevel[4];
	int16_t intTmpLevel;
	int16_t intLevel;
	int16_t difference;
	int16_t chargeTime;
	char chargeTimeEn;
	char charge;
	char thread;
}t_battery;

t_battery _battery;

void battery_manager(void);


#endif
