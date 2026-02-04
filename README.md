[![Designed by Sitron Labs](https://img.shields.io/badge/Designed_by-Sitron_Labs-FCE477.svg)](https://www.sitronlabs.com/)
[![Join the Discord community](https://img.shields.io/discord/552242187665145866.svg?logo=discord&logoColor=white&label=Discord&color=%237289da)](https://discord.gg/btnVDeWhfW)
[![PayPal Donate](https://img.shields.io/badge/PayPal-Donate-00457C.svg?logo=paypal&logoColor=white)](https://www.paypal.com/donate/?hosted_button_id=QLX8VU9Q3PFFL)
![License](https://img.shields.io/github/license/sitronlabs/SLHA00028-Firmware.svg)

# MySensors RFM Gazpar Module

Firmware for the MySensors RFM Gazpar Module, an IoT energy sensor designed to monitor French Gazpar gas meters in real time.

![Product preview](doc/product.jpg)

[Buy Now](https://www.sitronlabs.com/store/mysensors-rfm-gazpar-module-255)

## Overview

This module connects to French Gazpar gas meters to track gas consumption in real time. Gazpar meters emit a pulse every 0.01 m³ (10 liters), which this module detects and transmits wirelessly to your home automation controller. The module reports gas volume every minute and battery status every hour.

This product is designed for the French market and integrates with Gazpar smart gas meters installed by GRDF (Gaz Réseau Distribution France).

## What is MySensors?

[MySensors](https://www.mysensors.org/) is an open source framework that simplifies building your own IoT sensors and actuators for home automation. It handles the low-level communication protocols so you can focus on your project's functionality. MySensors integrates with many popular home automation controllers including Home Assistant, Jeedom, Domoticz, OpenHAB, and more, making it easy to add custom devices to your existing setup.

## Features

* Designed for [MySensors](https://www.mysensors.org/) with a RFM69 or RFM95/RFM96 (LoRa) radio
* Status LEDs for radio communication feedback
* Battery powered (2x AA batteries)
* Battery voltage monitoring
* Screw terminal for connecting cable to Gazpar meter pulse output
* USB port for debugging and firmware updates
* Automatic volume tracking with persistence across reboots
* Configurable reporting intervals
* Open firmware and schematic

## Building the Firmware

This project uses [PlatformIO](https://platformio.org/). To build and upload the firmware, open the project in PlatformIO IDE (VS Code extension), select the appropriate build environment from the toolbar based on your radio module (RFM69 or RFM95/96, 433 MHz or 868 MHz), then click "Build" to compile and "Upload" to flash the firmware to the module.
