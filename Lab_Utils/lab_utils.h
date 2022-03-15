#ifndef LAB_UTILS_INC
#define LAB_UTILS_INC

//// Module setup functions
bool setup_LoRa(long int band);
bool setup_AXP192();
void setup_GPS();

// Helper function to do all setup items in order
void do_lab_setup(int con_baud, long int band);

// Export GPSSerial for use in sketches
extern HardwareSerial GPSSerial;

/**
 * Storing of parameters to cycle through
 * NOTE
 * From my testing, Spreading Factor must remain the same across the experiment and cannot be cycled.
 * Please see https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md for values each parameter can take.
 */

// Used to store a set of parameters to test
struct LoRa_Params {
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

// The current LoRa parameters in use
extern LoRa_Params cur_params;

// Set the LoRa module to use a given set of parameters
void use_LoRa_params(LoRa_Params params);

#endif
