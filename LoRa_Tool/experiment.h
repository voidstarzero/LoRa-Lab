// Band to use for transmission
// Use 433E6 for Asia, 866E6 for EUR, and 915E6 for NA/Aus
#define BAND 915E6

// Prefix for all transmitted messages, for disambiguation
#define COMM_ID "JAMES"

// Delay for transmission rate (ms)
#define DELAY 1000

// Define each parameter set
const LoRa_Params params[] = {
  //  Bandwidth,  coding rate,  frequency,  spreading factor, transmit power
  {   250E3,      5,            9233E5,     7,                10},
  {   250E3,      5,            9233E5,     7,                12},
  {   250E3,      5,            9233E5,     7,                14},
};

// Number of different test parameter sets
#define NUM_PARAMS (sizeof params / sizeof (LoRa_Params))

// Number of test packets sent with each set of parameters
#define PKTS_PER_PARAM 50

