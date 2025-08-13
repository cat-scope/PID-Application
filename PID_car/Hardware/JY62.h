#ifndef __JY62_H
#define __JY62_H

void JY62_Init(void);
void JY62_Configure(void);
extern int16_t jy62_yaw;
int16_t NormalizeAngle(int16_t angle);


#endif
