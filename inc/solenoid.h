#ifndef __SOLENOID_H
#define __SOLENOID_H


typedef struct{
	char solenoid;
	char solenoid2;
}t_solenoid;

t_solenoid _solenoid;


void solenoid_manager(void);

#endif
