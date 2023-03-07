# Acknowledgements

The LoRa Library this module has been based of was created by Sandeep Mistry at: https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md

This project uses code that is MIT licensed located at https://github.com/sandeepmistry/arduino-LoRa/blob/master/LICENSE.


# LoRa-Lab Set-Up Instructions

The aim of this lab is to explore the behaviour of wireless channels and LoRa physical parameters in an experimental setting using TTGO T-beams.

The software for this lab was developed as part of a UWA Master's of Professional Engineering (MPE) Thesis Project.

The software is a LoRa toolkit for creating and recreating experiments with simple configuration.

To setup an experimental LoRa parameter configuration to test, edit the `Lora_Tool/experiment.h` file as needed. 



Please see the API.md for values each variable can take.

## Lab Set-Up

The following software tools are be required for this lab.

### Arduino IDE
Download the Arduino IDE software via:

https://www.arduino.cc/en/software

### Jupyter Notebook
This will be used for analysis after experiments have been conducted.

If you have a local deployment of Jupyter Notebook, skip this step.

Navigate to: https://jupyter.org/try and select `Try JupyterLab`

**Creating a new Python 3 Notebook**

`File` -> `New Notebook` -> `Python 3`

**Alternatively** read the Log_Files [README.md](/Log_Files/README.md) for Google Colab use.

## Setting up Arduino Environment

1. Follow the "Installing ESP32 Add-on in Arduino IDE" steps.

https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/


2. Clone the following libraries to the Arduino `libraries` folder.
```
cd C:\Users\<YOUR_USERNAME>\Documents\Arduino\libraries

git clone https://github.com/sandeepmistry/arduino-LoRa.git

git clone https://github.com/mikalhart/TinyGPSPlus.git

git clone https://github.com/lewisxhe/AXP202X_Library.git
```

2. *(Optional)* If unable to do step 2, unzip files to directory.

   https://github.com/sandeepmistry/arduino-LoRa

   https://github.com/mikalhart/TinyGPSPlus

   https://github.com/lewisxhe/AXP202X_Library

   Choose green the `Code` dropdown -> `Download ZIP`

   Extract the root folder to the Arduino `libraries` folder.
```
C:\Users\<YOUR_USERNAME>\Documents\Arduino\libraries
```

## Installation of the LoRa Toolkit Repository

1. Clone the repository to your desired location
   **NOTE: For the purposes of this GitHub we will be installing to the `Arduino` folder.**
```
cd C:\Users\<YOUR_USERNAME>\Documents\Arduino

git clone https://github.com/voidstarzero/LoRa-Lab
```
2. Copy the `Lab_Utils` folder to the Arduino `libraries` folder.
```
C:\Users\<YOUR_USERNAME>\Documents\Arduino\libraries
```

## Uploading Code to TTGO T Beams

1. Open the location of `LoRa_Tool`.
```
C:\Users\<YOUR_USERNAME>\Documents\Arduino\libraries\LoRa_Tool
``` 
2. Open the `experiment.h` file in your editor of choice.

3. Edit the paramaters in the file to suit your experiment needs.

3.1. **Windows Users:** On Windows, the symbolic links may break from `LoRa_Tool/Sender/experiment.h` and `LoRa_Tool/Receiver/experiment.h` to `LoRa_Tool/experiment.h`. You can just copy the `experiment.h` file from `LoRa_Tool/experiment.h` into the `LoRa_Tool/Sender` and the `LoRa_Tool/Receiver` directories to compile the sender and receiver.

4. Save the file.

5. Navigate to the LoRa_Tool folder
```
~\LoRa-Lab\LoRa_Tool
```
6. Navigate to the `Receiver` folder and open `Receiver.ino` with the Arduino IDE.
```
~\LoRa-Lab\LoRa_Tool\Receiver\Receiver.ino
```
7. Select `Tools` from the toolbar the top.

   Choose `Board` -> `ESP32` -> `TTGO T1`

   Choose `Port` -> `<YOUR_DEVICE_COM_PORT>`

8. Click the `Verify` button.

9. Click the `Compile` button.

10. Repeat "Steps 2 to Steps 6 with `Sender` and `Sender.ino`

11. At the end of compilation, the console would exit with 
```
"Leaving...
Hard resetting via RTS pin...
```
This is not an error, the ESP has loaded the data and rebooting to run the code and is ready.

12. **PLEASE NOTE THE GPS MODULE REQUIRES ~ 5-10 MINUTES TO INITIALISE AFTER CODE HAS BEEN UPLOADED.** Run a dummy or preloaded experiment first until GPS values are being outputted in the console/serial monitor before conducting **YOUR** experiments. The GPS module will also **NOT WORK INDOORS**.

*It is recommended that if you are able to use two laptops, to set up a Receiver paired up with a Laptop and a Sender paired up with a Laptop to reduce time waiting and ability to change parameters quickly and simultaneously* 

## Lab Experiments

Now that you have the software environment running go to the LoRa [Lab_Tasks](/Lab_Tasks/README.md) instructions to complete some experiments.
```
~\LoRa-Lab\Lab_Tasks
```
