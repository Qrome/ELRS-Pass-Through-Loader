# RP2040‑Zero ELRS Serial Passthrough Tool
A lightweight USB‑to‑UART passthrough utility built on the **Waveshare RP2040‑Zero**, designed specifically for flashing **ExpressLRS (ELRS)** receivers without a flight controller.  
This tool provides a clean, reliable serial bridge between the **ExpressLRS Configurator** and any ELRS receiver using standard UART pins.

---

## ✨ Features

- **Direct Serial Passthrough** (USB ↔ UART1)
- Fully compatible with **ExpressLRS Configurator**
- Select Flashing Method **Betaflight Passthrough** and the com port
- Uses **GPIO0 (TX)** and **GPIO1 (RX)** for ELRS communication
- Automatic LED status feedback using the onboard WS2812 RGB LED:
  - **Blue (solid):** Device powered and idle/connected  
  - **Green (flashing):** Active data transfer (firmware flashing or bootloader traffic)
- No Betaflight passthrough emulation required  
- No MSP, CRSF, or protocol parsing — pure transparent UART bridge  
- Clean, minimal, reliable firmware

---

## 🛠 Hardware Requirements

- **Waveshare RP2040‑Zero** https://amzn.to/47EOQJG
- **ExpressLRS Receiver** (any UART‑based model) https://amzn.to/3NrMl6L
- USB‑C cable https://amzn.to/3PcbGlL
- 5V power supplied from the RP2040‑Zero to the receiver

---

## 🔌 Wiring Guide

Connect the RP2040‑Zero to the ELRS receiver as follows:

| RP2040‑Zero Pin | ELRS Receiver Pin |
|-----------------|-------------------|
| **GPIO0 (TX)**  | RX                |
| **GPIO1 (RX)**  | TX                |
| **5V**          | 5V                |
| **GND**         | GND               |

No additional components are required.

---

## 💡 LED Status Indicators

The RP2040‑Zero includes a built‑in WS2812 RGB LED on **GPIO16**.  
This firmware uses it to show device state:

| LED Color | Meaning |
|-----------|---------|
| **Blue (solid)** | Device is powered, USB connected, and idle |
| **Green (flashing)** | Data is actively passing through (firmware flashing or bootloader communication) |

The LED automatically returns to blue after ~150 ms of inactivity.

---

## 📦 Firmware Overview

The firmware implements:

- USB CDC serial at **420000 baud**
- UART1 passthrough at **420000 baud**
- Bidirectional byte‑for‑byte forwarding
- Activity detection for LED feedback
- No buffering, no protocol interpretation, no latency‑adding logic

This makes the RP2040‑Zero behave like a high‑speed USB‑to‑UART adapter optimized for ELRS flashing.

---

## 🚀 Usage

1. Connect the RP2040‑Zero to your PC via USB‑C.
2. Wire the ELRS receiver to GPIO0/1, 5V, and GND.
3. Open **ExpressLRS Configurator**.
4. Select **Serial Passthrough** as the flashing method.
5. Choose the COM port corresponding to the RP2040‑Zero.
6. Flash as normal.

The LED will flash **green** during data transfer and return to **blue** when idle.

---

## 📁 Arduino Sketch

The full Arduino sketch used for this project is included in this repository under:
