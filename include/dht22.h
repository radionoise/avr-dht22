/*
 * DHT22 temperature and humidity sensor library for AVR.
 */

#ifndef AVR_DHT22_DHT22_H
#define AVR_DHT22_DHT22_H

#define DHT22_NOT_FOUND_ERROR 999
#define DHT22_CRC_ERROR 998

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
Dht22Data *dht22ReadData(Dht22Port *port);

#endif //AVR_DHT22_DHT22_H