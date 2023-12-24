#ifndef CONFIG_H
#define CONFIG_H

/* Leds configuration */
#define CONFIG_LED_RED_PIN A1
#define CONFIG_LED_YELLOW_PIN A2
#define CONFIG_LED_GREEN_PIN A3
#define CONFIG_LED_PULSE_PIN 4

/* MySensors configuration */
#define MY_DEBUG
#define MY_DEFAULT_ERR_LED_PIN CONFIG_LED_RED_PIN
#define MY_DEFAULT_TX_LED_PIN CONFIG_LED_GREEN_PIN

/* Battery monitoring configuration */
#define CONFIG_BATTERY_PIN A0

/* Input configuration */
#define CONFIG_GAZPAR_PULSE_PIN 3

#endif
