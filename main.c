#include "../include/Smbus.h"
#include "../include/Motor.h"
#include <stdio.h>
#include <unistd.h>
#include <linux/i2c-dev.h>

void print_speed(Motor motor){
    uint8_t data[MOTOR_COUNT];
    int size;
    int ret = get_encoder_speed(&motor, ALL, data, &size);
    for (int i = 0; i < size; ++i){
        printf("result_speed %d of motor %d\n", data[i], i);
    }
}

int main(){
    uint8_t addr;
    Motor motor;
    int ret;

    char * filename = "/dev/i2c-1";
    int file_i2c = 0;
     if((file_i2c = open(filename, O_RDWR))<0){
       printf("can't open file_i2c\n");
     }
  
    motor.fd = file_i2c;
    //fixit
    motor.command = 1;
    addr = 0x10;
    ret = set_addr(&motor, addr);
    printf("set_addr status %d\n", ret);
    ret = motor_stop(&motor, ALL);
    printf("motor_stop status %d\n", ret);
    ret = motor_movement(&motor, ALL, 1);
    printf("motor_movement status %d\n", ret);
    return ret;
}

int _write_bytes(Motor motor, uint8_t reg, uint8_t *data, int size);
int _read_bytes(Motor motor, uint8_t reg, uint8_t *data);
int _set_value(Motor * motor, int id, uint8_t reg, uint8_t value);

void _set_control_mode(Motor motor, int mode){
    // _write_bytes(self._REG_CTRL_MODE, [mode])
}

int set_addr(Motor * motor, uint8_t addr){
    Devcm devcm;
    uint8_t reg;
    uint8_t	data[1];																				

    devcm.fd = motor->fd;
    devcm.comand = 0;
    int res = checkAddress(devcm, addr);
    printf("check_addr status %d\n", res);
    if (res < 0) {
        return res;
    }
    motor->addr = addr;
	data[0] = addr;
    reg = REG_SLAVE_ADDR;									
    return _write_bytes(*motor, reg, data, 1);
}

int begin(Motor * motor){
    								
    return -1;
}

int get_encoder_speed(Motor * motor, int id, uint8_t * result, int * size) {
    int arr[MOTOR_COUNT];
    int ret = parse_id(id, arr, size);
    if (ret < 0){
        return ret;
    }
    uint8_t	data[2];																				
    for (int i = 0; i < *size; ++i){
        uint8_t value = REG_ENCODER1_SPPED + 5 * (arr[i] - 1);
        ret = _read_bytes(*motor, value, data);
        if (ret < 0){
            return ret;
        }
        uint8_t s = (data[0] << 8) | data[1];
        if ((s & 0x8000) > 0){
            s = - (0x10000 - s);
        }
        result[i] = s;
    }					
    return 1;
}

int set_encoder_enable(Motor * motor, int id){
    return _set_value(motor, id, REG_ENCODER1_EN + 5, 0x01);
}

int motor_stop(Motor * motor, int id){
    return _set_value(motor, id, REG_MOTOR1_ORIENTATION + 3, STOP);
}

int motor_movement(Motor * motor, int id, int speed){
    if (speed > 100.0 || speed < 0.0) {
        return -1;
    } else {
        uint8_t	data[2];																				
        int arr[MOTOR_COUNT];
        int size;
        if (parse_id(id, arr, &size) < 0) return -1;
        for (int i = 0; i < size; ++i){
            int reg = REG_MOTOR1_SPEED + (i - 1) * 3;
            data[0] = speed;
            data[1] = (speed * 10) % 10;
            if (_write_bytes(*motor, reg, data, 2) < 0) {
                return -1;
            }					
        }						
    }
    return 1;
}


int _set_value(Motor * motor, int id, uint8_t reg, uint8_t value){
    uint8_t	data[1];																				
    int arr[MOTOR_COUNT];
    int size;
    if (parse_id(id, arr, &size) < 0) return -1;
    for (int i = 0; i < size; ++i){
        data[0] = value;
        if (_write_bytes(*motor, reg * (arr[i] - 1), data, 1) < 0) {
            return -1;
        }					
    }
    return 0;
}
    


int parse_id(int id, int * arr, int * size){
    int ret;
    ret = 0;
    if (id == ALL){
        for(int i = 1; i < MOTOR_COUNT + 1; ++i) {
            arr[i-1] = i;
        } 
        *size = MOTOR_COUNT;
    }else {
        if (id < 1 || id > MOTOR_COUNT) {
            ret = -1;
        } else {
            arr[0] = id;
            *size = 1;
        }
    }
    return ret;
}
    

int _write_bytes(Motor motor, uint8_t reg, uint8_t *data, int size) {
    int ret; 
    Devcm devcm;
    devcm.fd = motor.fd;
    devcm.comand = motor.command;
    ret = writeBytes(devcm, motor.addr, reg, data, size);
    return ret;
}

int _read_bytes(Motor motor, uint8_t reg, uint8_t *data) {
    int ret; 
    int sumtry;
    Devcm devcm;
    devcm.fd = motor.fd;
    devcm.comand = motor.command;
    
    ret = -1;
    sumtry = 10;
    do {
        ret = readBytes(devcm, motor.addr, reg, data, sumtry);
        sumtry --;
    } while (sumtry > 0 && ret<0);
    return ret;
}
