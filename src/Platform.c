#include "../include/Smbus.h"
#include "../include/Motor.h"
#include "../include/Platform.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int init(Platform * p, uint8_t addr_front, uint8_t addr_back){
    Motor motor_front, motor_back;

    int ret;
    ret = begin(&motor_front, addr_front);

    if (ret < 0){
        return ret;
    }

    ret = begin(&motor_back, addr_back);

    if (ret < 0){
        return ret;
    }
    
    p->motor_front = &motor_front;
    p->motor_back = &motor_back;
    return 0;
}

int moving_ahead(Platform * p, int speed){
    int ret;
    ret = motor_movement(p->motor_back, M1, speed, CW);
    if (ret < 0) {
        return ret;
    }
    ret = motor_movement(p->motor_back, M2, speed, CCW);
    if (ret < 0) {
        return ret;
    }
    ret = motor_movement(p->motor_front, M1, speed, CCW);
    if (ret < 0) {
        return ret;
    }
    ret = motor_movement(p->motor_front, M2, speed, CW);
    if (ret < 0) {
        return ret;
    }
    return 0;
}

int rotation(Platform * p, uint8_t orientation, int speed){
    int ret;
    int back, front;
    if (orientation == CW){
        back = orientation;
        front = orientation >> 0x01;
    } else {
        front = orientation;
        back = orientation >> 0x01;
    }
    
    ret = motor_movement(p->motor_back, M1, speed, back);
    if (ret < 0) {
        return ret;
    }
    ret = motor_movement(p->motor_back, M2, speed, back);
    if (ret < 0) {
        return ret;
    }
    ret = motor_movement(p->motor_front, M1, speed, front);
    if (ret < 0) {
        return ret;
    }
    ret = motor_movement(p->motor_front, M2, speed, front);
    if (ret < 0) {
        return ret;
    }
    return 0;
}