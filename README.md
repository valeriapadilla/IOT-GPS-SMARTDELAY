# LILYGO LoRa 32 End Device Sensor Development

This project involves the development of an end device sensor using the integrated LILYGO LoRa 32 microprocessor, provided by the lab. The sensor setup includes an HDC1008 sensor for temperature and humidity measurement, along with GPS positioning capabilities.

## Features
- **Temperature & Humidity Measurement**: Utilizing the HDC1008 sensor.
- **GPS Positioning**: Integrated GPS module for location tracking.
- **Data Transmission**: Implements chirping, pruning, and bundling processes to send information to a cloud server at `http://10.38.32.137/`.
  
## Hardware Communication

- **GPS Communication**: The GPS module communicates with the microcontroller via `Serial1`.
- **I2C Communication**: The HDC1008 sensor uses digital inputs 4 and 0 for I2C communication.
- **WiFi Connectivity**: Managed using the `WiFi.h` library for seamless data transmission to the cloud.
  

## Data Transmission Structure
The data is sent via a POST request with the following structure:
```json
POST http://10.38.32.137/update_data
{
    "id": "point11",
    "lat": 6.245259,
    "lon": -75.596510,
    "temperature": 22.4,
    "humidity": 68
}

