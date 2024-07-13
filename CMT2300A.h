#ifndef CMT2300A_H
#define CMT2300A_H

#include <Arduino.h>
#include <SPI.h>

class CMT2300A {
public:
    // Конструктор класса
    CMT2300A(uint8_t csPin);

    // Инициализация устройства
    void begin();

    // Настройка частоты
    void setFrequency(uint32_t freq);

    // Настройка ширины полосы пропускания
    void setBandwidth(uint8_t bandwidth);

    // Настройка мощности передачи
    void setPower(uint8_t power);

    // Настройка спрединг фактора
    void setSpreadingFactor(uint8_t sf);

    // Настройка коэффициента ошибки
    void setCodingRate(uint8_t rate);

    // Настройка длины пакета
    void setPacketLength(uint8_t length);

    // Настройка усиления LNA
    void setLNA(uint8_t gain);

    // Настройка фильтра передатчика
    void setTXFilterBandwidth(uint8_t bandwidth);

    // Настройка фильтра приемника
    void setRXFilterBandwidth(uint8_t bandwidth);

    // Установка 64-битного адреса устройства
    void setAddress(uint64_t address);

    // Установка режима работы
    void setMode(uint8_t mode);

    // Настройка пайпа
    void setPipe(uint8_t pipe, uint32_t freq, uint8_t bandwidth);

    // Установка режима пайпа
    void setPipeMode(uint8_t pipe, uint8_t mode);

    // Отправка сообщения
    void sendMessage(uint8_t* data, uint8_t length);

    // Получение сообщения
    void receiveMessage(uint8_t* data, uint8_t* length);

private:
    uint8_t _csPin;

    // Запись значения в регистр
    void writeRegister(uint8_t reg, uint8_t value);

    // Чтение значения из регистра
    uint8_t readRegister(uint8_t reg);

    // Передача данных по SPI
    void spiTransfer(uint8_t* txBuf, uint8_t* rxBuf, size_t len);

    // Регистры CMT2300A
    const uint8_t REG_OP_MODE = 0x01;          // Режим работы
    const uint8_t REG_FREQ_MSB = 0x06;         // MSB частоты
    const uint8_t REG_FREQ_MID = 0x07;         // MID частоты
    const uint8_t REG_FREQ_LSB = 0x08;         // LSB частоты
    const uint8_t REG_BW = 0x09;               // Ширина полосы пропускания
    const uint8_t REG_POWER = 0x0C;            // Мощность передачи
    const uint8_t REG_SF = 0x0D;               // Спрединг фактор
    const uint8_t REG_CR = 0x0E;               // Коэффициент ошибки
    const uint8_t REG_PAC = 0x0F;              // Пакетная длина
    const uint8_t REG_LNA = 0x13;              // Усиление LNA
    const uint8_t REG_TX_FILTER_BW = 0x14;    // Фильтр передатчика
    const uint8_t REG_RX_FILTER_BW = 0x15;    // Фильтр приемника
    const uint8_t REG_ADDRESS_MSB = 0x1A;     // MSB 64-битного адреса
    const uint8_t REG_ADDRESS_LSB = 0x1B;     // LSB 64-битного адреса
    const uint8_t REG_MODE = 0x1C;            // Режим работы
    const uint8_t REG_PIPE0 = 0x1E;           // Пайп 0
    const uint8_t REG_PIPE1 = 0x1F;           // Пайп 1
    const uint8_t REG_PIPE0_MODE = 0x20;     // Режим пайпа 0
    const uint8_t REG_PIPE1_MODE = 0x21;     // Режим пайпа 1
    const uint8_t REG_FIFO = 0x22;           // FIFO регистр

    // Режимы работы
    static const uint8_t MODE_SLEEP = 0x00;
    static const uint8_t MODE_TX = 0x01;
    static const uint8_t MODE_RX = 0x02;

    // Режимы пайпов
    static const uint8_t PIPE_MODE_TX = 0x01;
    static const uint8_t PIPE_MODE_RX = 0x02;

    // Ширина полосы пропускания
    static const uint8_t BW_7_8KHz = 0x00;
    static const uint8_t BW_10_4KHz = 0x01;
    static const uint8_t BW_15_6KHz = 0x02;
    static const uint8_t BW_20_8KHz = 0x03;
    static const uint8_t BW_31_2KHz = 0x04;
    static const uint8_t BW_41_7KHz = 0x05;
    static const uint8_t BW_62_5KHz = 0x06;
    static const uint8_t BW_125KHz = 0x07;

    // Коэффициент ошибки
    static const uint8_t CR_4_5 = 0x01;
    static const uint8_t CR_4_6 = 0x02;
    static const uint8_t CR_4_7 = 0x03;
    static const uint8_t CR_4_8 = 0x04;
};

#endif
