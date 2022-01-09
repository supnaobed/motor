#ifndef SMBUS
#define SMBUS

#include <unistd.h>
#include <sys/ioctl.h> 
#include <stdint.h> 
#include <sys/ioctl.h>

typedef struct Devcm
{
    int fd;
    unsigned long comand;
} Devcm;


// Функция пакетной записи нескольких байт данных в регистр модуля:
// аргументы: адрес_модуля, адрес_первого_регистра, указатель_на_массив, количество_байт
int writeBytes(Devcm devcm, uint8_t addr, uint8_t reg, uint8_t *data, uint8_t sum){
    uint8_t length = sum+1;
    uint8_t data_with_reg[length];
    for (int i=1; i < length; i++) {
        data_with_reg[i] = data[i-1];
    }
    data_with_reg[0] = reg;
    if (ioctl(devcm.fd, devcm.comand, addr) < 0){
        return -1;
    } else if (write(devcm.fd, data_with_reg, length) != length) {
        return -1;
    } else  {
        return 0;
    }
}

// Функция пакетной записи нескольких байт данных в модуль:
// аргументы: адрес_модуля, указатель_на_массив, количество_байт
int writeBytesMod(Devcm devcm, uint8_t addr, uint8_t *data, uint8_t sum){
    if (ioctl(devcm.fd, devcm.comand, addr) < 0) return -1;
    else if (write(devcm.fd, data, sum) !=sum) return -1;
    else return 0;
}

// Функция пакетного чтения нескольких байт данных из регистров модуля:
// аргументы: адрес_модуля, адрес_первого_регистра, указатель_на_массив, количество_байт
int readBytes(Devcm devcm, uint8_t addr, uint8_t reg, uint8_t *data, uint8_t sum){
    if (ioctl(devcm.fd, devcm.comand, addr) < 0) return -1;
    else if ((write(devcm.fd, &reg, 1) != 1)) return -1;
    else if (read(devcm.fd, data, sum) != sum) return -1;
    else return 0;
}

// Функция пакетного чтения нескольких байт данных из модуля:
// аргументы: адрес_модуля, указатель_на_массив, количество_байт
int readBytesMod(Devcm devcm, uint8_t addr, uint8_t *data, uint8_t sum){
    if (ioctl(devcm.fd, devcm.comand, addr) < 0) return -1;
    else if (read(devcm.fd, data, sum) != sum) return -1;
    else return 0;
}


//  Функция чтения одного байта из регистра модуля:
//  аргументы: адрес_модуля, адрес_регистра
uint8_t readByteReg(Devcm devcm, uint8_t adr, uint8_t reg){
    uint8_t i=0; readBytes(devcm, adr, reg, &i, 1);
    return i;
}

//  Функция чтения одного байта из модуля:
uint8_t readByte(Devcm devcm, uint8_t adr){
    uint8_t i=0; readBytesMod(devcm, adr, &i, 1);
    return i;
}

// Функция записи одного байта в регистр модуля:
int writeByteReg(Devcm devcm, uint8_t adr, uint8_t reg, uint8_t data){
    return writeBytes(devcm, adr, reg, &data, 1);
}

// Функция записи одного байта в модуль:
int writeByte(Devcm devcm, uint8_t adr, uint8_t data){
    return writeBytesMod(devcm, adr, &data, 1);
}

// Функция проверки наличия ведомого по его адресу:
// аргументы: адрес_регистра, байт_данных
int checkAddress(Devcm devcm, uint8_t addr){
    if (ioctl(devcm.fd, devcm.comand, addr) < 0) return -1;
    else return 0;
}

#endif