# Weather Hub

Welcome to the **Weather Hub** project, a collaborative initiative for schools within the CIJE network. This project aims to empower students and educators to build and deploy their own weather stations using various wireless technologies, such as the ESP32, to collect and share weather data.

## Project Overview

The Weather Hub project provides all the necessary documents, code, and supporting files to build weather stations capable of transmitting real-time weather data to a centralized MySQL database. The weather stations will connect using wireless technologies such as the **ESP32** WiFi to connect to a cloud server utilizing **JSON** and **AJAX** to store the data in the database.

## Features

- **Wireless Communication**: Utilize the ESP32 or similar microcontrollers to collect and transmit weather data.
- **Real-Time Data**: Transmit data through the Weather Hub using provided code offering real time data capture.
- **Data Visualization**: Example code is provided to display weather data locally on devices or web interfaces.
- **Centralized Data Hub**: All weather stations will send their data in real-time to a cloud server with a MySQL database using JSON and AJAX accessible at [CIJE Weather Hub](https://cijeweatherhub.site).
- **Easy Registration**: After building a weather station, students and educators must register their station at [CIJE Weather Hub Registration]([https://cijeweatherhub.site](https://cijeweatherhub.site/weather-station-registration/)) with a unique name and location (either by zip code or latitude/longitude).
- **Public Data Access**: View and analyze weather data collected from all registered stations across the CIJE network at [CIJE Weather Hub](https://cijeweatherhub.site).

## Getting Started

### Prerequisites

- Basic knowledge of electronics and programming.
- Arduino IDE
- Access to an ESP32 or a similar microcontroller.
- An active Wi-Fi network.

### Installation

1. **Clone this Repository**:
   ```bash
   git clone https://github.com/yourusername/weather-hub.git
2. **Build a Weather Station**

- Follow the provided documentation in the `/docs` folder to assemble your weather station.
- This weather station utilizes the ESP32 to connect with an online database. However, other wireless technologies that use WiFi may be utilized but may require coding adjustments. Also GSM technologies (see `/examples` folder to or to the CIJE Weather Hub database.
- Additionally, Bluetooth (like the HM-10) and the nRF24 can be used transmit data to local display or app instead of or in addition to uploading data to the online database. 

3.  **Register Your Weather Station**

- After assembly and programming, register your weather station at [CIJE Weather Hub](https://cijeweatherhub.site) with a unique name and location.
  
4.
- **Flash code**

- Use the example code provided in the `/examples` folder to program your ESP32, HM-10, SiM7000 or nRF24.
- Be sure to include your unique **station_id** number provided after registration.

5. **Monitor Data**

- Once registered, your weather station will begin transmitting data to the CIJE Weather Hub. You can monitor the data in real-time at [CIJE Weather Hub](https://cijeweatherhub.site).

### Contributing

We welcome contributions from educators, students, and developers to improve and expand the Weather Hub project. Please refer to the `CONTRIBUTING.md` file for more details.

### License

This project is licensed under the MIT License - see the `LICENSE` file for details.

### Acknowledgments

This project is supported by the CIJE network, dedicated to enhancing STEM education across schools.
