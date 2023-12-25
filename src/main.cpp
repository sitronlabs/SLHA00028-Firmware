/* Config */
#include "../cfg/config.h"

/* Arduino Libraries */
#include <Arduino.h>
#include <MySensors.h>

/* C/C++ libraries */
#include <ctype.h>
#include <stdlib.h>

/* */
static double volatile m_value;                 //!< In cubic meters
static bool volatile m_value_initial_received;  //!<

/* List of virtual sensors */
enum {
    SENSOR_0_MANAL,  // S_INFO (V_TEXT)
    SENSOR_1_GAS,    // S_GAS (V_VOLUME)
};

/**
 * Setup function.
 * Called once MySensors has successfully initialized.
 */
void setup(void) {

    /* Setup serial */
    Serial.begin(115200);
    Serial.println(" [i] Hello world.");

    /* Setup battery reading */
    analogReference(INTERNAL);
    analogRead(CONFIG_BATTERY_PIN);

    /* Setup pulse input */
    pinMode(CONFIG_GAZPAR_PULSE_PIN, INPUT);
}

/**
 * MySensors function called to describe this sensor and its capabilites.
 */
void presentation(void) {

    /* Because messages might be lost,
     * we're not doing the presentation in one block, but rather step by step,
     * making sure each step is sucessful before advancing to the next */
    for (int8_t step = -1;;) {

        /* Send out presentation information corresponding to the current step,
         * and advance one step if successful */
        switch (step) {
            case -1: {
                if (sendSketchInfo(F("SLHA00028 Gazpar"), F("0.1.0")) == true) {
                    step++;
                }
                break;
            }
            case SENSOR_0_MANAL: {
                if (present(SENSOR_0_MANAL, S_INFO, F("Relevé manuel")) == true) {
                    step++;
                }
                break;
            }
            case SENSOR_1_GAS: {
                if (present(SENSOR_1_GAS, S_GAS, F("Consommation")) == true) {
                    step++;
                }
                break;
            }
            default: {
                return;
            }
        }

        /* Sleep a little bit after each presentation, otherwise the next fails
         * @see https://forum.mysensors.org/topic/4450/sensor-presentation-failure */
        sleep(50);
    }
}

/**
 * MySensors function called when a message is received.
 */
void receive(const MyMessage &message) {

    /* Handle user manual input initial value */
    if (message.sensor == SENSOR_0_MANAL && message.getType() == V_TEXT) {
        m_value = strtod(message.getString(), NULL);
        m_value_initial_received = true;
        MyMessage message(SENSOR_0_MANAL, V_TEXT);
        send(message.set(""));
    }

    /* Handle value sent by controller */
    else if (message.sensor == SENSOR_1_GAS && message.getType() == V_VOLUME) {
        m_value = message.getFloat();
        m_value_initial_received = true;
    }
}

/**
 * Main loop.
 */
void loop(void) {
    int res;

    /* Sensor task */
    {
        static enum {
            STATE_0,
            STATE_1,
            STATE_2,
            STATE_3,
            STATE_4,
            STATE_5,
        } m_sm;
        switch (m_sm) {

            case STATE_0: {

                /* Present user with a text input to set the initial value */
                MyMessage message(SENSOR_0_MANAL, V_TEXT);
                if (send(message.set("")) != true) {
                    sleep(1000);
                    break;
                }

                /* Move on */
                m_sm = STATE_1;
                break;
            }

            case STATE_1: {

                /* Request last value
                 * this is done to keep the value incrementing across reboots
                 * the response will be processed in the receive() function */
                if (request(SENSOR_1_GAS, V_VOLUME) != true) {
                    sleep(1000);
                    break;
                }

                /* Move on */
                m_sm = STATE_2;
                break;
            }

            case STATE_2: {

                /* Wait for initial value to be retrieved */
                if (m_value_initial_received != true) {
                    sleep(1000, true);
                    m_sm = STATE_1;
                    break;
                }

                /* Move on */
                Serial.printf(" [w] Retrieved value from controller.\r\n");
                m_sm = STATE_3;
                break;
            }

            case STATE_3: {

                /* Compute battery level */
                double adc_voltage = (1.1 * analogRead(CONFIG_BATTERY_PIN)) / 1023.0;
                double bat_voltage = (adc_voltage * (1000 + 300)) / 300.0;
                double bat_ratio = (bat_voltage - 3.6) / (4.2 - 3.6);
                if (bat_ratio < 0) {
                    bat_ratio = 0;
                } else if (bat_ratio > 1) {
                    bat_ratio = 1;
                }
                uint8_t bat_pct = bat_ratio * 100;

                /* Send battery level if it has changed */
                static uint8_t bat_pct_last = UINT8_MAX;
                if (bat_pct_last != bat_pct) {
                    if (sendBatteryLevel(bat_pct, false) == true) {
                        bat_pct_last = bat_pct;
                    } else {
                        Serial.println(" [w] Failed to send battery level!");
                    }
                }

                /* Move on */
                m_sm = STATE_4;
                break;
            }

            case STATE_4: {

                /* Sleep until we get interrupted by either a pulse signal or a timeout */
                uint8_t interrupt = digitalPinToInterrupt(CONFIG_GAZPAR_PULSE_PIN);
                res = sleep(interrupt, FALLING, 3600000, true);

                /* If we have been interrupted by a pulse signal,
                 * increment the value and send the new one */
                if (res == interrupt) {
                    m_value += 0.010;
                    m_sm = STATE_5;
                }

                /* If we have been interrupted by a timeout,
                 * just go back to the battery reporting part */
                else if (res == MY_WAKE_UP_BY_TIMER) {
                    m_sm = STATE_3;
                }
                break;
            }

            case STATE_5: {

                /* Send the value,
                 * but if it doesn't work don't bother retrying */
                static uint32_t value_last = 0;
                if (m_value != value_last) {
                    MyMessage message(SENSOR_1_GAS, V_VOLUME);
                    if (send(message.set((float)m_value, 3)) == true) {
                        value_last = m_value;
                    } else {
                        Serial.println(" [w] Failed to send gas volume!");
                    }
                }

                /* Move on */
                m_sm = STATE_3;
                break;
            }
        }
    }
}
