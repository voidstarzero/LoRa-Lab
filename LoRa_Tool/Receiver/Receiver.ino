//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>

//Libraries used for GPS Module
#include <TinyGPS++.h>
#include <axp20x.h>

TinyGPSPlus gps;
HardwareSerial GPS(1);
AXP20X_Class axp;

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 915E6

//packet counter
int counter = 0;
int test = 0;
int test2 = 0;

String LoRaData;

String logPrint; // variable to record logging data
int NUM_PARAMETERS = 3; // number of unique parameters passed
int NUM_PACKETS = 50; // number of packets to send
int dly = 1000; // delay for transmission rate (ms)

// Save current coding rate
int cr_cur;

int getCodingRate()
{
    return cr_cur;
}

/**
 * Storing of parameters to cycle through
 * NOTE
 * From my testing, Spreading Factor must remain the same across the experiment and cannot be cycled.
 * Please see https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md for values each parameter can take.
 */

// Storing parameters to test
//Supported values are 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, 250E3, and 500E3.
long int bandwidth[] = {250E3, 250E3, 250E3};

//Supported values are between 5 and 8, these correspond to coding rates of 4/5 and 4/8. The coding rate numerator is fixed at 4.
int codingRate[] = {5, 5, 5, 5, 5, 5, 5, 5, 5};

//frequency - frequency in Hz (433E6, 868E6, 915E6)
long int frequency[] = {9233E5, 9233E5, 9233E5};

//Supported values are between 6 and 12. If a spreading factor of 6 is set, implicit header mode must be used to transmit and receive packets.
int spreadingFactor[] = {7, 7, 7, 7, 7, 7, 7, 7, 7};

//Supported values are 2 to 20 for PA_OUTPUT_PA_BOOST_PIN, and 0 to 14 for PA_OUTPUT_RFO_PIN.
int txPower[] = {10, 12, 14, 8, 10, 12, 14, 10, 12, 14};

//Minimize bandwidth and maximize spreading factor to boost link budget. Maximize coding rate to boost reliability.

/**
 * Function to set parameters, taking an integer to loop through
 */
String setParameters(int i)
{

  LoRa.setSignalBandwidth(bandwidth[i]);
  LoRa.setCodingRate4(codingRate[i]);
  LoRa.setFrequency(frequency[i]);
  LoRa.setSpreadingFactor(spreadingFactor[i]);
  LoRa.setTxPower(txPower[i]);

  long int bw = bandwidth[i];
  int cr = codingRate[i];
  long int freq = frequency[i];
  int sf = spreadingFactor[i];
  int tx = txPower[i];

  cr_cur = codingRate[i];

  // Storing in csv to print to log file
  logPrint = "";
  logPrint = logPrint +
             bw + "," +
             cr + "," +
             freq + "," +
             sf + "," +
             tx;

  return logPrint;
}

void setup()
{
  //initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("LoRa Receiver Test");

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND))
  {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }
  //Serial.println("LoRa Initializing OK!");
  Wire.begin(21, 22);
  if (!axp.begin(Wire, AXP192_SLAVE_ADDRESS)) {
    //Serial.println("AXP192 Begin PASS");
  } else {
    Serial.println("AXP192 Begin FAIL");
  }
  axp.setPowerOutPut(AXP192_LDO2, AXP202_ON);
  axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
  axp.setPowerOutPut(AXP192_EXTEN, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC1, AXP202_ON);
  
  GPS.begin(9600, SERIAL_8N1, 34, 12);   //17-TX 18-RX
  
  Serial.println("Time,Number,S_Lat,S_Long,Bandwidth,CodingRate,Frequency,SpreadingFactor,TxPower,PacketRSSI,RSSI,SNR,R_Lat,R_Long");
}

void parsePacket()
{ 
  //try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    smartDelay(500);
      if(gps.time.hour()<10){
        Serial.print("0");
      }
      Serial.print(gps.time.hour());
      Serial.print(":");
      if(gps.time.minute()<10){
        Serial.print("0");
      }
      Serial.print(gps.time.minute());
      Serial.print(":");
      if(gps.time.second()<10){
        Serial.print("0");
      }
      Serial.print(gps.time.second());
      Serial.print(",");    
    //read packet
    while (LoRa.available())
    {
      LoRaData = LoRa.readString();

      Serial.print(LoRaData);
      Serial.print(",");
    }
    
    //print RSSI of packet
    int packetRssi = LoRa.packetRssi();
    int rssi = LoRa.rssi();
    float snr = LoRa.packetSnr();
    float t_lat = gps.location.lat();
    float t_lng = gps.location.lng();

    String lat = String(t_lat,6);
    String lng = String(t_lng,6);

    String temp = ",";
    temp = temp + packetRssi + "," + rssi + "," + snr + "," +lat+ ","+lng;

    //logPrint = logPrint + temp;

    Serial.println(logPrint + temp);
    test++;
    test2++;
  }
}

void loop()
{
  for (int i = 0; i < NUM_PARAMETERS; i++)
  {

    // set new parameters
    setParameters(i);
    delay(dly);

    //Serial.println(i);
    test = 0;

    while (test < NUM_PACKETS)
    {
      parsePacket();
    }

    if (test2 == NUM_PARAMETERS * NUM_PARAMETERS)
    {
      Serial.println("Lora End!");
      LoRa.end();
      return;
    }
  }
  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (GPS.available())
      gps.encode(GPS.read());
  } while (millis() - start < ms);
}
