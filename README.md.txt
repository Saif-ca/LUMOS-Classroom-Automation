# 🌟 LUMOS – A Smart Electronic System for Classroom Automation

![LUMOS Banner](screenshots/banner.png) <!-- Optional if you want a banner -->

## 📌 Overview

**LUMOS** is a B.Tech final year project aimed at transforming traditional classrooms into intelligent, automated environments using **IoT**, **AI**, and **Flutter-based mobile applications**. It enhances energy efficiency, automates attendance, and improves accessibility—especially for differently-abled students—by enabling remote control and monitoring of classroom devices.

---

## 🧠 Key Features

- 📸 **AI-based Attendance System** – Uses face recognition (VGGFace, OpenCV) for real-time, automated student attendance.
- 💡 **Smart Device Control** – Automates fans and lights based on occupancy (PIR sensor) and environment (LDR, DHT11).
- 📱 **Mobile Application** – Built using Flutter for cross-platform control of classroom appliances.
- 🌐 **Cloud Integration** – Real-time data sync and logging using Firebase.
- 🔋 **Energy Monitoring** – Tracks device-level energy consumption using ACS712.
- 🗣️ **Voice Command Support** – Accessible interface via voice commands for better inclusivity.

---

## 🛠️ Tech Stack

| Layer          | Technology                                      |
|----------------|--------------------------------------------------|
| Hardware       | NodeMCU (ESP8266), PIR, DHT11, LDR, ACS712, Relay |
| App Development | Flutter (Dart), Android Studio                  |
| Backend        | Firebase Realtime Database, Python, Postman      |
| AI Models      | Haar Cascade, VGGFace (Facial Recognition)       |
| Programming    | C++, Dart, Python                                |
| Tools          | Arduino IDE, Visual Studio Code, Google Colab    |

---

## 🧩 Modules

- **User Management**
- **Mobile App Interface**
- **Device Control**
- **Energy Monitoring**
- **Scheduling Automation**
- **Face Recognition Attendance**
- **Alerts & Notifications**
- **Voice Command Module**
- **Cloud Sync**

---
## 📥 Download Model File

Due to GitHub's file size restrictions, the face recognition model (`face_recognition_model.pkl`) is hosted externally.

👉 [Download the model from Google Drive]( https://drive.google.com/drive/folders/1Y8wSxaTN6NonsYc1kic5AKnf0Pd2Dv0p?usp=drive_link )

---
## 📷 Screenshots

![Login Page](screenshots/login_screen.png)
![Device Control](screenshots/device_control.png)
![Attendance View](screenshots/attendance.png)

---

## 📂 Project Structure

```bash
LUMOS-Classroom-Automation/
├── arduino-code/             # ESP8266 & sensor scripts
├── flutter-app/              # Flutter mobile app files
├── face-recognition/         # Python scripts for attendance
├── screenshots/              # UI and working system screenshots
├── README.md                 # Project overview
