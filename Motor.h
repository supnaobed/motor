#ifndef DF_ROBOT_MOTOR
#define DF_ROBOT_MOTOR

#define STEPPER_COUNT 1
#define MOTOR_COUNT 2

#define REG_SLAVE_ADDR 0x00
#define REG_PID 0x01
#define REG_PVD 0x02
#define REG_CTRL_MODE 0x03
#define REG_ENCODER1_EN 0x04
#define REG_ENCODER1_SPPED 0x05
#define REG_ENCODER1_REDUCTION_RATIO 0x07
#define REG_ENCODER2_EN 0x09
#define REG_ENCODER2_SPEED 0x0a
#define REG_ENCODER2_REDUCTION_RATIO 0x0c
#define REG_MOTOR_PWM 0x0e
#define REG_MOTOR1_ORIENTATION 0x0f
#define REG_MOTOR1_SPEED 0x10
#define REG_MOTOR2_ORIENTATION 0x12
#define REG_MOTOR2_SPEED 0x13

#define REG_DEF_PID 0xdf
#define REG_DEF_VID 0x10

// Enum motor ID
#define M1 0x01
#define M2 0x02

// Board status
#define STA_OK 0x00
#define STA_ERR 0x01
#define STA_ERR_DEVICE_NOT_DETECTED 0x02
#define STA_ERR_SOFT_VERSION 0x03
#define STA_ERR_PARAMETER 0x04

// last operate status, users can use this variable to determine the result of a function call.
#define last_operate_status STA_OK

// Board control mode 
#define control_mode_dc_motor 0x00
#define control_mode_stepper 0x01

// Orientation and global variables 
#define CW 0x01     # clockwise
#define CCW 0x02    # countclockwise
#define STOP 0x05
#define ALL 0xffffffff

typedef struct Motor
{
    int fd;
    uint8_t addr;
    unsigned long command;
} Motor;


int begin(Motor * motor);
int set_addr(Motor * motor, uint8_t addr);
int motor_stop(Motor * motor, int id);
int parse_id(int id, int * ret, int * size);
int set_encoder_enable(Motor * motor, int id);
int get_encoder_speed(Motor * motor, int id, uint8_t * result, int * size);
int motor_movement(Motor * motor, int id, int speed);
#endif	
