# DHT22 library for AVR microcontroller

### Overview
This library was tested on ATmega 16 @ 8 MHz.

### Example
```
Dht22Port port = {&DDRA, &PORTA, &PINA, PIN0};
Dht22Port *portPtr = &port;
Dht22Data *data = dht22ReadData(portPtr);

if (data->temperature != DHT22_NOT_FOUND_ERROR && data->temperature != DHT22_CRC_ERROR) {
    // do something
}

free(data);
```