#include <CMT2300A.h>

CMT2300A::CMT2300A(uint8_t csPin) : _csPin(csPin) {
    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, HIGH);
}

void CMT2300A::begin() {
    SPI.begin();
    // Установка начальных значений регистров
    setMode(MODE_SLEEP);
    delay(100);
    setMode(MODE_RX);  // Установить режим приема по умолчанию
}

void CMT2300A::setFrequency(uint32_t freq) {
    // Установка частоты: 32 бит (MSB, MID, LSB)
    writeRegister(REG_FREQ_MSB, (freq >> 16) & 0xFF);
    writeRegister(REG_FREQ_MID, (freq >> 8) & 0xFF);
    writeRegister(REG_FREQ_LSB, freq & 0xFF);
}

void CMT2300A::setBandwidth(uint8_t bandwidth) {
    // Настройка ширины полосы пропускания
    writeRegister(REG_BW, bandwidth);
}

void CMT2300A::setPower(uint8_t power) {
    // Настройка мощности передачи
    writeRegister(REG_POWER, power);
}

void CMT2300A::setSpreadingFactor(uint8_t sf) {
    // Установка спрединг фактора от 6 до 12
    writeRegister(REG_SF, sf);
}

void CMT2300A::setCodingRate(uint8_t rate) {
    // Установка коэффициента ошибки
    writeRegister(REG_CR, rate);
}

void CMT2300A::setPacketLength(uint8_t length) {
    // Установка длины пакета
    writeRegister(REG_PAC, length);
}

void CMT2300A::setLNA(uint8_t gain) {
    // Настройка усиления LNA
    writeRegister(REG_LNA, gain);
}

void CMT2300A::setTXFilterBandwidth(uint8_t bandwidth) {
    // Настройка фильтра передатчика
    writeRegister(REG_TX_FILTER_BW, bandwidth);
}

void CMT2300A::setRXFilterBandwidth(uint8_t bandwidth) {
    // Настройка фильтра приемника
    writeRegister(REG_RX_FILTER_BW, bandwidth);
}

void CMT2300A::setAddress(uint64_t address) {
    // Установка 64-битного адреса устройства (MSB, MID, LSB)
    writeRegister(REG_ADDRESS_MSB, (address >> 40) & 0xFF);
    writeRegister(REG_ADDRESS_MSB + 1, (address >> 32) & 0xFF);
    writeRegister(REG_ADDRESS_MSB + 2, (address >> 24) & 0xFF);
    writeRegister(REG_ADDRESS_MSB + 3, (address >> 16) & 0xFF);
    writeRegister(REG_ADDRESS_MSB + 4, (address >> 8) & 0xFF);
    writeRegister(REG_ADDRESS_LSB, address & 0xFF);
}

void CMT2300A::setMode(uint8_t mode) {
    // Установка режима работы
    writeRegister(REG_OP_MODE, mode);
}

void CMT2300A::setPipe(uint8_t pipe, uint32_t freq, uint8_t bandwidth) {
    // Настройка частоты и ширины полосы пропускания для указанного пайпа
    if (pipe == 0) {
        writeRegister(REG_PIPE0, (freq >> 16) & 0xFF);  // MSB частоты
        writeRegister(REG_PIPE0 + 1, (freq >> 8) & 0xFF);  // MID частоты
        writeRegister(REG_PIPE0 + 2, freq & 0xFF);  // LSB частоты
        writeRegister(REG_PIPE0 + 3, bandwidth);  // Ширина полосы пропускания
    } else if (pipe == 1) {
        writeRegister(REG_PIPE1, (freq >> 16) & 0xFF);  // MSB частоты
        writeRegister(REG_PIPE1 + 1, (freq >> 8) & 0xFF);  // MID частоты
        writeRegister(REG_PIPE1 + 2, freq & 0xFF);  // LSB частоты
        writeRegister(REG_PIPE1 + 3, bandwidth);  // Ширина полосы пропускания
    }
}

void CMT2300A::setPipeMode(uint8_t pipe, uint8_t mode) {
    // Установка режима для пайпа
    if (pipe == 0) {
        writeRegister(REG_PIPE0_MODE, mode);
    } else if (pipe == 1) {
        writeRegister(REG_PIPE1_MODE, mode);
    }
}

void CMT2300A::sendMessage(uint8_t* data, uint8_t length) {
    setMode(MODE_TX);  // Установить режим передачи

    // Запись данных в FIFO
    for (uint8_t i = 0; i < length; i++) {
        writeRegister(REG_FIFO, data[i]);
    }

    // Установка флага на передачу сообщения
    delay(100);  // Задержка для передачи сообщения

    // Вернуться в режим приема после передачи
    setMode(MODE_RX);
}

void CMT2300A::receiveMessage(uint8_t* data, uint8_t* length) {
    setMode(MODE_RX);  // Установить режим приема
    uint8_t i = 0;
    while (i < *length) {
        data[i] = readRegister(REG_FIFO);
        i++;
    }
    *length = i;
}

void CMT2300A::writeRegister(uint8_t reg, uint8_t value) {
    digitalWrite(_csPin, LOW);
    SPI.transfer(reg | 0x80);  // Запись в регистр
    SPI.transfer(value);
    digitalWrite(_csPin, HIGH);
}

uint8_t CMT2300A::readRegister(uint8_t reg) {
    digitalWrite(_csPin, LOW);
    SPI.transfer(reg & 0x7F);  // Чтение из регистра
    uint8_t value = SPI.transfer(0x00);  // Запрос значения
    digitalWrite(_csPin, HIGH);
    return value;
}

void CMT2300A::spiTransfer(uint8_t* txBuf, uint8_t* rxBuf, size_t len) {
    digitalWrite(_csPin, LOW);
    for (size_t i = 0; i < len; i++) {
        rxBuf[i] = SPI.transfer(txBuf[i]);
    }
    digitalWrite(_csPin, HIGH);
}
