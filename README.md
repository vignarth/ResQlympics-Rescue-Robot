# 🤖 ResQlympics Rescue Robot (ROBORASHTRA)

## 🏆 Achievement
Winner of ResQlympics Robotics Competition

---

## 📌 Project Overview
This robot was designed for the **ResQlympics challenge**, where the objective was to:

- Pick up rescue blocks
- Transport and store them inside a container
- Operate the robot remotely using FPV camera feed (operator cannot directly see the robot)

---

## 🎯 Problem Statement
In the competition, operators were not allowed to directly view the robot. Instead, they had to control it using live camera feed, simulating real-world rescue operations in hazardous or inaccessible environments.

---

## ⚙️ Features

✅ Remote controlled navigation  
✅ Servo-based block pickup mechanism  
✅ Real-time FPV camera feed  
✅ High torque drive motors  
✅ Long range RC control  
✅ High capacity Li-Po battery power system  

---

## 🧰 Components Used (BOM)

### 🔹 Control & Processing
- ESP32-WROOM-32 Development Board

### 🔹 Motion & Actuation
- Pro-Range DS3218 Pro 20kg Metal Gear Servo Motor
- 25GA-370 12V 60RPM DC Gear Motor
- BTS7960 43A H-Bridge Motor Driver Module
- 85mm Smart Robot Car Wheels

### 🔹 Mechanical Components
- 25mm DC Gear Motor I Shape Bracket
- 4mm Short Coupling (18mm Length)

### 🔹 Communication & Control
- Radiolink AT9S Pro 2.4GHz 12CH RC Transmitter
- Radiolink R9DS Receiver

### 🔹 Vision System
- 1/3" CMOS 1500TVL Mini FPV Camera
- 2.1mm Lens with OSD

### 🔹 Power System
- Pro-Range 11.1V 3300mAh 35C 3S Lithium Polymer Battery

---

## 🧠 Working Principle

### 🚗 Robot Movement
- The DC gear motors drive the wheels.
- BTS7960 motor driver controls motor direction and speed.
- RC transmitter sends commands wirelessly to the receiver.

### 🦾 Block Pickup Mechanism
- High torque servo motor controls gripping arm.
- Servo enables precise block pickup and release into storage container.

### 🎥 Vision System
- FPV camera streams live video feed to operator.
- Allows navigation without direct line-of-sight.

---

## 📂 Repository Structure
