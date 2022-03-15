#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>
#include <axp20x.h>

#include "lab_utils.h"

// Define the pins used by the LoRa transceiver module
#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_SS 18
#define LORA_RST 14
#define LORA_DIO0 26

bool setup_LoRa(long int band)
{
    // Init communications
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
    LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

    if (!LoRa.begin(band)) {
        return false;
    }
    return true;
}

// Define pins used by the AXP192 module
#define AXP_SDA 21
#define AXP_SCL 22

AXP20X_Class axp;

bool setup_AXP192()
{
    // Init communications
    Wire.begin(AXP_SDA, AXP_SCL);
    if (axp.begin(Wire, AXP192_SLAVE_ADDRESS) != 0) {
        // Non-zero code indicates failure
        return false;
    }

    axp.setPowerOutPut(AXP192_LDO2, AXP202_ON);
    axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
    axp.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
    axp.setPowerOutPut(AXP192_EXTEN, AXP202_ON);
    axp.setPowerOutPut(AXP192_DCDC1, AXP202_ON);
    return true;
}

// Define baud and pins used by the GPS serial connection
#define GPS_BAUD 9600
#define GPS_RX 34
#define GPS_TX 12

HardwareSerial GPSSerial(1);

void setup_GPS()
{
    GPSSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
}

// Helper function, does all setup items correctly in order
void do_lab_setup(int con_baud, long int band)
{
    // Initialize serial monitor
    Serial.begin(con_baud); // Doesn't fail
    Serial.println("info: Sender initializing...");

    // Initialize LoRa module
    if (!setup_LoRa(band)) {
        Serial.println("error: Starting LoRa failed!");
        while (true);
    }
    Serial.println("info: LoRa initialized OK.");

    // Initialize AXP module
    if (!setup_AXP192()) {
        Serial.println("error: Starting AXP192 failed!");
        while (true);
    }
    Serial.println("info: AXP192 initialized OK.");

    // Initialize GPS module
    setup_GPS(); // Doesn't fail
    Serial.println("info: GPS initialized OK.");
}

LoRa_Params cur_params;

void use_LoRa_params(LoRa_Params params)
{
  LoRa.setSignalBandwidth(params.bandwidth);
  LoRa.setCodingRate4(params.codingRate);
  LoRa.setFrequency(params.frequency);
  LoRa.setSpreadingFactor(params.spreadingFactor);
  LoRa.setTxPower(params.txPower);

  cur_params = params;
}
