#ifndef __BUZZER_H
#define __BUZZER_H

typedef struct{
	char buzz;
}t_buzzer;

t_buzzer _buzzer;

void buzzer_manager(void);
void buzzer_on(void);
void buzzer_off(void);

#endif
