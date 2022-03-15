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

String logPrint; // variable to record logging data
int NUM_PARAMETERS = 3; // number of unique parameters passed
extern int NUM_PACKETS = 50; // number of packets to send
extern int dly = 1000; // delay for transmission rate (ms)

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
struct LoRa_Params
{
  //Supported values are 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, 250E3, and 500E3.
  long int bandwidth;

  //Supported values are between 5 and 8, these correspond to coding rates of 4/5 and 4/8. The coding rate numerator is fixed at 4.
  int codingRate;

  //frequency - frequency in Hz (433E6, 868E6, 915E6)
  long int frequency;

  //Supported values are between 6 and 12. If a spreading factor of 6 is set, implicit header mode must be used to transmit and receive packets.
  int spreadingFactor;

  //Supported values are 2 to 20 for PA_OUTPUT_PA_BOOST_PIN, and 0 to 14 for PA_OUTPUT_RFO_PIN.
  int txPower;
};

LoRa_Params params[] = {
  {250E3, 5, 9233E5, 7, 10},
  {250E3, 5, 9233E5, 7, 12},
  {250E3, 5, 9233E5, 7, 14},
};

/**
 * Function to set parameters, taking an integer to loop through
 */
String setParameters(int i)
{
  LoRa.setSignalBandwidth(params[i].bandwidth);
  LoRa.setCodingRate4(params[i].codingRate);
  LoRa.setFrequency(params[i].frequency);
  LoRa.setSpreadingFactor(params[i].spreadingFactor);
  LoRa.setTxPower(params[i].txPower);

  long int bw = params[i].bandwidth;
  int cr = params[i].codingRate;
  long int freq = params[i].frequency;
  int sf = params[i].spreadingFactor;
  int tx = params[i].txPower;

  cr_cur = params[i].codingRate;

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

  Serial.println("LoRa Sender Test");

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
  Serial.println("LoRa Initializing OK!");
  Wire.begin(21, 22);
  if (!axp.begin(Wire, AXP192_SLAVE_ADDRESS)) {
    Serial.println("AXP192 Begin PASS");
  } else {
    Serial.println("AXP192 Begin FAIL");
  }
  axp.setPowerOutPut(AXP192_LDO2, AXP202_ON);
  axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
  axp.setPowerOutPut(AXP192_EXTEN, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC1, AXP202_ON);
  GPS.begin(9600, SERIAL_8N1, 34, 12);   //17-TX 18-RX
  delay(dly*2);
}

void sendPacket()
{
  smartDelay(500);
  Serial.print("Sending packet: ");
  Serial.println(counter);

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  float t_lat = gps.location.lat();
  float t_lng = gps.location.lng();

  String lat = String(t_lat,6);
  String lng = String(t_lng,6);
  
  Serial.println(lat + "," + lng);
  // Added by James
  LoRa.print("JAMES,");
  LoRa.print(counter);
  LoRa.print(",");
  LoRa.print(lat);
  LoRa.print(",");
  LoRa.print(lng);
  LoRa.print(",");
  LoRa.print(getCodingRate());

  LoRa.endPacket();
  counter++;
  delay(dly);
}

void loop()
{
  for (int i = 0; i < NUM_PARAMETERS; i++)
  {
    // set new parameters
    setParameters(i);
    delay(dly*2);
    Serial.println(logPrint);
    Serial.println(i);

    for (int j = 0; j < NUM_PACKETS; j++)
    {
      sendPacket();
    }
  }
  Serial.println("Run end!");
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
