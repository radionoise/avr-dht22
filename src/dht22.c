#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dht22.h"

#define sbi(port,bit) (port) |= (1 << (bit))
#define cbi(port,bit) (port) &= ~(1 << (bit))
#define sbi64Bit(port,bit) (port) |= ((uint64_t) 1 << (bit))
#define cbi64Bit(port,bit) (port) &= ~((uint64_t) 1 << (bit))

void dht22SetInput(Dht22Port *port) {
    cbi(*port->avrDdrPort, port->avrPin);
}

void dht22SetOutput(Dht22Port *port) {
    sbi(*port->avrDdrPort, port->avrPin);
}

void dht22SetBit(Dht22Port *port) {
    sbi(*port->avrPort, port->avrPin);
}

void dht22ClearBit(Dht22Port *port) {
    cbi(*port->avrPort, port->avrPin);
}

uint8_t dht22ReadBit(Dht22Port *port) {
    uint8_t bits = *port->avrPinPort & (1 << port->avrPin);
    return bits >> port->avrPin;
}

bool dht22Init(Dht22Port *port) {
    dht22SetBit(port);
    dht22SetOutput(port);
    _delay_ms(1);

    cli();
    dht22ClearBit(port);
    _delay_ms(18);
    dht22SetInput(port);

    _delay_us(40 + 40);
    uint8_t bit0 = dht22ReadBit(port);
    _delay_us(40 + 40);
    uint8_t bit1 = dht22ReadBit(port);
    _delay_us(40);
    sei();

    if (!bit0 && bit1) {
        return true;
    } else {
        return false;
    }
}

bool dht22CheckCrc(uint64_t data, Dht22Port *port) {
    uint8_t crc = (uint8_t) data;
    uint8_t crcCalculated = 0;

    for (int bit = 1; bit <= 4; bit++) {
        uint8_t byte = data >> 8 * bit;
        crcCalculated += byte;
    }

    return crcCalculated == crc;
}

uint64_t dht22ReadDataBits(Dht22Port *port) {
    uint64_t data = 0;

    cli();
    for (int bit = 39; bit >= 0; bit--) {
        while (!dht22ReadBit(port)) {
            _delay_us(1);
        }

        _delay_us(28);
        uint8_t result = dht22ReadBit(port);
        if (result) {
            sbi64Bit(data, bit);
            _delay_us(42);
        } else {
            cbi64Bit(data, bit);
        }
    }
    sei();

    return data;
}

Dht22Data *dht22ReadData(Dht22Port *port) {
    Dht22Data *data = malloc(sizeof(Dht22Data));

    bool sensorFound = dht22Init(port);

    if (sensorFound) {
        uint64_t bits = dht22ReadDataBits(port);

        if (dht22CheckCrc(bits, port)) {
            int relativeHumidity = (bits >> 24) / 10;
            uint16_t temperature = (bits >> 8);

            bool negative = false;
            if (temperature & 0x8000) {
                temperature &= 0x7FFF;
                negative = true;
            }

            data->relativeHumidity = relativeHumidity;
            data->temperature = (float) temperature / 10;

            if (negative) {
                data->temperature *= -1;
            }
        } else {
            data->relativeHumidity = DHT22_CRC_ERROR;
            data->temperature = DHT22_CRC_ERROR;
        }
    } else {
        data->relativeHumidity = DHT22_NOT_FOUND_ERROR;
        data->temperature = DHT22_NOT_FOUND_ERROR;
    }

    return data;
}