#ifndef DF_ROBOT_PLATFORM
#define DF_ROBOT_PLATFORM

typedef struct Platform
{
    Motor * motor_front;
    Motor * motor_back;
} Platform;


int init(Platform * p, uint8_t addr_front, uint8_t addr_back);
int moving_ahead(Platform * p, int speed);
int rotation(Platform * p, uint8_t orientation, int speed);

#endif	
