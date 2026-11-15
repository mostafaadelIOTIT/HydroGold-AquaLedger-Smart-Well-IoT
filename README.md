# 💧 HydroGold Dynamics & AquaLedger™ – Smart Groundwater Filter & Zero-Waste Upcycling IoT

**Lead Architect:** Mostafa Adel Muhammed (B.Sc. in Information Technology, GPA: 3.09 | IT Systems, Odoo ERP & Cybersecurity Specialist)  
**LinkedIn:** [mostafa-adel-iotit](https://www.linkedin.com/in/mostafa-adel-iotit/)

## 📌 Executive Summary
**HydroGold Dynamics** and the **AquaLedger™** platform transform deep groundwater filter backwash sludge (rich in pure Iron Oxide $\text{Fe}_2\text{O}_3$, Manganese Dioxide $\text{MnO}_2$, and Colloidal Silica) from an irrigation-clogging liability into a zero-waste industrial stream of micronutrient fertilizers, construction pigments, and EU CBAM carbon credits.

This repository hosts the embedded **ESP32 C++ SCADA firmware** and **Odoo ERP / Blockchain telemetry dashboard** that automate dual-filter backwashing, UV-C iron-bacteria sterilization, bluetooth/ADC sludge weighing, and **Smart iLock OTP security**.

## 🚀 Live Interactive Simulations (Click Below to Test in Browser)
* 🔗 **[Launch AquaLedger ERP & Carbon Credit Console (Streamlit Cloud)](https://hydrogold-aqualedger-smart-well-iot-d9cvsh33hpjnwetsw8vz2g.streamlit.app/)**
* 🔗 **[Run Real-Time ESP32 Smart Filter & iLock Simulation (Wokwi)](https://wokwi.com/projects/480762495601059841)**

## 🏗️ System Architecture & Key Modules
1. **Automated Dual-Filter & Harvester (`firmware_hydrogold_esp32.ino`):** Monitors differential pressure clogging and ultrasonic well levels, automatically triggering backwash flushing into solar drying harvesters while running UV-C LEDs to eliminate iron biofilms.
2. **Smart iLock OTP & SHA-256 Security:** Prevents unauthorized sludge tampering using servo-actuated smart locks that unlock via 6-digit encrypted OTPs once target harvest weights ($\ge 100\text{ Kg}$) are verified, signing all packets with hardware **SHA-256 hashes**.
3. **AquaLedger & Odoo ERP Integration (`app.py`):** Calculates real-time $3.01\times$ stoichiometric mass multiplication (soluble iron/manganese fertilizers, industrial oxides, and $\text{CO}_2$ offsets) and manages farmer barter wallets.
