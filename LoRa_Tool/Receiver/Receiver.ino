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

  // Print header for serial CSV data
  delay(DELAY * 2);
  Serial.println(
    "R_Time,Comm_ID,Pkt_Set,Pkt_ID,S_Lat,S_Lng,Pkt_BW,Pkt_CR,"
    "Pkt_Freq,Pkt_SF,Pkt_Tx_Pwr,Pkt_RSSI,Now_RSSI,Pkt_SNR,R_Lat,R_Lng");
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

static int expect_pkt;

void parse_packet()
{
  int packet_sz = LoRa.parsePacket();
  if (packet_sz)
  {
    smart_delay(DELAY / 2);

    // Get packet contents
    String LoRa_data;
    while (LoRa.available())
    {
      LoRa_data += LoRa.readString();
    }

    // TODO: abort if packet does not start with COMM_ID + ','

    // Print my GPS time
    if (gps.time.hour() < 10) Serial.print("0");
    Serial.print(gps.time.hour());

    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print("0");
    Serial.print(gps.time.minute());

    Serial.print(":");
    if (gps.time.second() < 10) Serial.print("0");
    Serial.print(gps.time.second());

    // Print packet contents
    Serial.print(",");
    Serial.print(LoRa_data);

    // Print LoRa parameters
    Serial.print(",");
    Serial.print(cur_params.bandwidth);

    Serial.print(",");
    Serial.print(cur_params.codingRate);

    Serial.print(",");
    Serial.print(cur_params.frequency);

    Serial.print(",");
    Serial.print(cur_params.spreadingFactor);

    Serial.print(",");
    Serial.print(cur_params.txPower);
    
    // Print LoRa metrics
    Serial.print(",");
    Serial.print(LoRa.packetRssi());

    Serial.print(",");
    Serial.print(LoRa.rssi());

    Serial.print(",");
    Serial.print(LoRa.packetSnr());

    // Print my GPS location
    Serial.print(",");
    Serial.print(String(gps.location.lat(), 6));

    Serial.print(",");
    Serial.println(String(gps.location.lng(), 6));

    // Expect the next packet
    // TODO: calculate based on pkt_id + 1 to account for dropped packets
    expect_pkt++;
  }
}

void loop()
{
  for (int i = 0; i < NUM_PARAMS; i++)
  {
    // Set new parameters
    use_LoRa_params(params[i]);
    delay(DELAY);

    expect_pkt = 0;
    while (expect_pkt < PKTS_PER_PARAM)
    {
      parse_packet();

      // TODO: If no packet detected within 5-10 seconds, automatically advance to next params (assume missed transition)
    }
  }
}
