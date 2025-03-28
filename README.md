# 🌊 Solar-Powered IoT Salinity Monitoring System

## 📖 Project Overview

This project focuses on developing a **real-time salinity monitoring system** for remote water bodies. It is designed to operate **24/7**, powered by a **solar panel and battery system**, and uses the **Simcom A7680 LTE module** to upload data to **ThingSpeak** or a cloud database. The system enables **unattended, continuous monitoring** in environments where wired infrastructure is unavailable.

## 🌟 Key Features

- **Real-time salinity data monitoring** using custom-built IoT hardware  
- **LTE data transmission** via Simcom A7680 module  
- **Solar-powered with battery backup** for off-grid deployment  
- **Low-power design** for long-term operation in remote locations  
- **Data integration** with platforms like ThingSpeak or cloud databases  

## 🛠️ What I Worked On

- 🔧 **Ensured 24/7 hardware reliability** through robust power design and testing  
- 📡 **Guaranteed consistent data transmission** to the cloud/database using LTE module  
- 🧩 **Designed custom PCBs** in KiCad to support outdoor deployment and energy efficiency  
- 🧠 **Developed embedded firmware** on ESP32 (C/C++) for sensor interfacing and LTE communication  
- 📚 **Created custom libraries** to read data from salinity sensors  
- 🔋 **Calculated energy needs** and selected appropriate solar panel and battery sizes for reliable 24/7 performance  
- 💤 **Optimized power consumption** through sleep modes and efficient code  
- 🧪 **Assisted deployment and testing** with the research team in real-world environments  

## 📦 System Components

- **ESP32-C3** microcontroller  
- **Simcom A7680** LTE module  
- **Salinity sensor** (custom interfacing)  
- **15W Solar Panel** with 18650 battery storage  
- Custom-designed **PCB** for field deployment  

## 🌐 Data Upload

Sensor data is transmitted hourly via LTE to:
- **ThingSpeak** (for visualization and alerts), or  
- A **custom cloud database** for storage and further analysis

## 📂 Project Structure

```
/.vscode         → VSCode configuration files  
/code            → Embedded firmware and source code  
/imgs            → Images or diagrams for documentation  
README.md        → Project summary and overview (this file)  
schematic.pdf    → Hardware circuit schematic  
.gitignore       → Git ignore rules  
```

## 🚀 Deployment

This system is currently being tested and deployed in **remote aquatic environments** as part of a research initiative focused on environmental monitoring and smart agriculture.

## 📸 Device Preview
Here is a photo of the deployed salinity monitoring device:
- **PCB of device**  
  <img src="./imgs/2_1.jpg" alt="PCB of device" width="400"/>

- **Prototype**  
  <img src="./imgs/prototype_pcb_0.jpg" alt="Prototype" width="400"/>

- **Streamlit app**  
  <img src="./imgs/streamlit.png" alt="Streamlit app" width="400"/>

- **Test in real environment**  
  <img src="./imgs/demo.jpg" alt="Test in real environment" width="400"/>

### 🔗 Important Links

- 📦 **[EC Sensor Library](https://github.com/tuchau1404/ECTDS10_Modbus)**  
  Custom Modbus library for salinity (EC) sensor communication using ESP32.

- 📄 **[EC Sensor Documentation](https://drive.google.com/drive/folders/1hncmjJqfUVRawqIBNy7eqLtmfCup9ZS6?usp=sharing)**  
  Detailed technical documents, datasheets, and usage guides.

- 📊 **[ThingSpeak – Device 1](https://thingspeak.mathworks.com/channels/2652379)**  
  Live salinity data feed from first deployed device.

- 📊 **[ThingSpeak – Device 2](https://thingspeak.mathworks.com/channels/2859624)**  
  Real-time data from second deployed unit.

- 🖥️ **[Streamlit GUI App](https://baswap-app.streamlit.app/)**  
  Web-based dashboard for visualizing and interacting with salinity data.

- 📋 **[Device Inventory List](https://docs.google.com/spreadsheets/d/1m3qKdZznI4jfmvPm_3H1yemph4VKm6-8AGkJWUl8WHI/edit?usp=sharing)**  
  Google Sheet containing device IDs, locations, and deployment status.

