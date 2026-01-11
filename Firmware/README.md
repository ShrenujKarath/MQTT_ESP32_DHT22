## **ESP32 DHT22 Monitoring with MQTT \& Node-RED Dashboard**





#### 📌 Overview

This project demonstrates an end-to-end IoT data pipeline using an ESP32, a DHT22 temperature \& humidity sensor, MQTT communication, and a Node-RED dashboard.



The ESP32 reads sensor data and publishes it to an MQTT broker. Node-RED subscribes to these topics, visualizes the data on a web dashboard, and sends control commands back to the ESP32 (LED control), enabling \*\*bidirectional IoT communication\*\*.



This project was developed and tested using \*\*Wokwi simulation\*\* and \*\*Node-RED with FlowFuse Dashboard\*\*.







#### 🧱 System Architecture




MQTT\_ESP32\_DHT22/

├── Dashboard/

│   └── Dashboard.png

│

├── Firmware/

│   └── esp32\_mqtt\_dht22.ino

│

├── Node-Red/

│   └── MQTT\_flows

│

├── Wiring/

│   └── Wiring.png

│

├── README.md

└── .gitignore







#### 🔌 Hardware Components



\- ESP32 Dev Module  

\- DHT22 Temperature \& Humidity Sensor  

\- External pull-up resistor (4.7kΩ – 10kΩ) on DATA line  

\- Breadboard \& jumper wires  







#### 🛠️ Software \& Tools



\- Arduino IDE (ESP32 core)

\- DHTesp library

\- PubSubClient (MQTT)

\- HiveMQ public MQTT broker

\- Node-RED

\- FlowFuse Node-RED Dashboard

\- Wokwi (ESP32 simulation)







#### 📡MQTT Topics Used



| Purpose          | Topic                   |

|------------------|-------------------------|

| Temperature data | `ESP/DHT22/temperature` |

| Humidity data    | `ESP/DHT22/humidity`    |

| LED control      | `ESP/mqtt`              |



LED Control Payloads

\- `"1"` → LED ON  

\- `"0"` → LED OFF  







#### 🧠 Firmware Logic Summary



\- Connects ESP32 to Wi-Fi

\- Establishes MQTT connection with broker

\- Reads temperature and humidity from DHT22 every 2 seconds

\- Publishes sensor data via MQTT

\- Subscribes to control topic to toggle onboard LED

\- Uses a callback-based MQTT architecture



Firmware source is located in:
Firmware/esp32\_mqtt\_dht22






#### 📊 Node-RED Dashboard



\- Real-time temperature visualization (chart)

\- Real-time humidity visualization (gauge)

\- Built using \*\*@flowfuse/node-red-dashboard\*\*



Dashboard URL (local):
http://localhost:1880/dashboard


# Node-RED flow export is included for easy import:
node-red/flows.json




---



#### 🧪 Simulation Notes (Important)



During Wokwi simulation:



\- Stable DHT22 readings were achieved using \*\*GPIO 9 (D2)\*\*

\- GPIO 9 behaves reliably in simulation but \*\*is not usable on real ESP32 hardware\*\*

\- This pin choice is used \*\*only for simulation purposes\*\* to demonstrate system functionality





#### ⚠️ Real Hardware Considerations



This project exposed a well-known limitation:



\# DHT22 + ESP32 + Wi-Fi Issues

\- DHT22 uses a timing-critical single-wire protocol

\- ESP32 Wi-Fi and MQTT introduce interrupt jitter

\- This can cause intermittent `NaN` or failed sensor reads



\# Mitigation Strategies (for real hardware)

\- Use safe GPIOs (e.g. GPIO 4 / 16 / 17)

\- Add an external pull-up resistor on DATA line

\- Increase sensor read interval (≥ 4 seconds)

\- Ignore failed reads and cache last valid values



\# Design Recommendation

For production or long-term deployment, replace DHT22 with an I²C-based sensor such as:

\- BME280

\- SHT31



These sensors are immune to timing issues under Wi-Fi load.







#### 🚀 How to Run (Simulation)



1\. Open the firmware `.ino` file in Arduino IDE or Wokwi

2\. Start the ESP32 simulation

3\. Run Node-RED

4\. Import `node-red/flows.json`

5\. Open the dashboard:  http://localhost:1880/dashboard







#### 🔮 Future Improvements



\- Replace DHT22 with BME280

\- Send sensor data as JSON payload

\- Add data logging (file or database)

\- Deploy private MQTT broker

\- Move firmware to ESP-IDF





#### 📌 Key Learnings



\- End-to-end MQTT-based IoT integration

\- Real-world limitations of timing-sensitive sensors

\- Difference between simulation behavior and real hardware

\- Importance of documenting known issues and design trade-offs











