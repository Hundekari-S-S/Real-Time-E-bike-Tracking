# Real-Time E-Bike Tracking with Geofencing & Route History

## About the Project

The **Real-Time E-Bike Tracking System** is an IoT-based project developed to monitor the location and movement of an electric bike using GPS technology. The main aim of this project is to improve vehicle safety by tracking its location, monitoring a predefined geographical area and maintaining route history.

The system uses an ESP32 microcontroller integrated with a NEO-6M GPS module to collect location data. Geofencing is used to identify when the vehicle moves outside its designated safe zone. When such movement is detected, an alert is sent through the Telegram Bot API.

The project also focuses on recording GPS coordinates and displaying previous travel routes on a map.

## Technologies & Tools

- **Microcontroller:** ESP32
- **GPS Module:** NEO-6M
- **Programming:** Embedded C++
- **Development Environment:** Arduino IDE
- **GPS Library:** TinyGPSPlus
- **Communication:** Wi-Fi, HTTPClient
- **Notification:** Telegram Bot API
- **Backend:** Python Flask
- **Map Visualization:** Google Maps

## Main Features

- **Live Location Tracking:** Collects GPS coordinates to monitor the current location of the e-bike.
- **Geofencing:** Uses a predefined safe geographical boundary to monitor vehicle movement.
- **Telegram Alerts:** Sends notifications when the vehicle crosses the configured geofence.
- **Route History:** Maintains GPS location points to track the vehicle's previous movement.
- **Map Integration:** Displays location information through Google Maps links.
- **IoT Connectivity:** Uses ESP32 and Wi-Fi to transmit location data.

## Hardware Components

| Component | Purpose |
| ESP32 | Main controller for processing GPS data |
| NEO-6M GPS Module | Receives latitude and longitude |
| LED | Provides visual status indication |
| Connecting Wires | Hardware connections |
| Power Supply | Provides power to the system |

## How the System Works

1. The NEO-6M GPS module receives the current geographical coordinates of the e-bike.
2. The ESP32 reads and processes the GPS data using the TinyGPSPlus library.
3. A reference location is established to define the geofence, based on the implemented configuration.
4. The ESP32 continuously checks the vehicle's location against the configured safe zone.
5. If the vehicle moves outside the permitted boundary, a Telegram notification is triggered.
6. GPS coordinates are collected to maintain the vehicle's route history.
7. Python Flask and google map are used for backend processing and map-based route visualization.

## Project Objective

The objective of this project is to develop a cost-effective IoT-based tracking solution for electric bikes. By combining GPS tracking, geofencing and route monitoring, the system aims to provide better vehicle monitoring, improve security and help users keep track of their vehicle's movement.

## Applications

- Electric vehicle tracking and monitoring
- Vehicle security and theft monitoring
- Fleet management systems
- GPS-based route tracking
- IoT-based transportation solutions

## Future Enhancements

- Development of a dedicated mobile application.
- Integration of a real-time web-based tracking dashboard.
- Cloud database integration for storing GPS records.
- Battery level monitoring for electric vehicles.
- Additional security features and advanced vehicle monitoring.

## Project Outcome

This project provided practical experience in working with ESP32, GPS modules, embedded programming and IoT communication. It also helped develop an understanding of geofencing, GPS data processing, vehicle monitoring and map-based route visualization.
