/*
 * DHT22 temperature and humidity sensor library for AVR.
 * Version: 2.0.0
 */

#include <stdint.h>

#ifndef AVR_DHT22_DHT22_H
#define AVR_DHT22_DHT22_H

#define DHT22_OK 0
#define DHT22_ERROR_NOT_FOUND 1
#define DHT22_ERROR_CRC 2

typedef struct Dht22Port {
    volatile uint8_t * avrDdrPort;
    volatile uint8_t * avrPort;
    volatile uint8_t * avrPinPort;
    uint8_t avrPin;
} Dht22Port;

typedef struct Dht22Data {
    int relativeHumidity;
    float temperature;
} Dht22Data;

/*
 * Reads data from sensor
 */
uint8_t dht22ReadData(Dht22Data *data, Dht22Port *port);

#endif //AVR_DHT22_DHT22_H