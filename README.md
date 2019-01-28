# DHT22 library for AVR microcontroller

### Overview
This library was tested on ATmega 16 @ 8 MHz.

### Example
```
Dht22Port port = {&DDRA, &PORTA, &PINA, PIN0};
Dht22Port *portPtr = &port;
Dht22Data data;
uint8_t result = dht22ReadData(&data, portPtr);

if (result == DHT22_OK) {
    // do something
}
```