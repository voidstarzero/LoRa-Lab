// Include GPS parser
#include <TinyGPSPlus.h>
TinyGPSPlus gps;

// Include LoRa PHY library
#include <LoRa.h>

// Include lab utility library
#include <lab_utils.h>

// Common experiment definitions
#include "experiment.h"

void setup()
{
  do_lab_setup(115200, BAND);

  // Allow time to attach a console
  delay(DELAY * 2);
}

static void smart_delay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (GPSSerial.available())
    {
      gps.encode(GPSSerial.read());
    }
  } while (millis() - start < ms);
}

void send_packet(int i, int j)
{
  String msg;
  smart_delay(DELAY / 2);

  // Add the comm ID (includes trailing comma itself)
  msg += COMM_ID;

  // Add the param number and packet number
  msg += i;
  msg += ',';
  msg += j;

  // Add the GPS coordinates of the sender
  msg += ',';
  msg += String(gps.location.lat(), 6);
  msg += ',';
  msg += String(gps.location.lng(), 6);

  // Print the data to the console
  Serial.println(msg);

  // Send data in a LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(msg);
  LoRa.endPacket();

  delay(DELAY / 2);
}

void loop()
{
  for (int i = 0; i < NUM_PARAMS; i++)
  {
    // Set new parameters
    use_LoRa_params(params[i]);
    delay(DELAY * 2);

    for (int j = 0; j < PKTS_PER_PARAM; j++)
    {
      send_packet(i, j);
    }
  }
}
