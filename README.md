# ESP32 Atom Echo project


## Contents

- [Purpose and goals](#purpose-and-goals)
- [Hardware and software Requirements](#hardware-and-software-requirements)
- [Used libraries](#used-libraries)
- [Programming Challenges](#programming-challenges)
- [Future Considerations](#future-considerations)




### Purpose and goals
The purpose of this project is to develop a IoT solution using the ESP32 microcontroller and the Atom Echo device to create a seamless and user-friendly process for connecting to a home Wi-Fi network. The project device isn't required to have I/O capabilities in order to achieve its purpose and goals. 

The main goals of this project include simplifying the Wi-Fi configuration, enabling persistent storage of network credentials, establishing reliable connections, providing a user-friendly experience, and considering future expansion possibilities.

Working principle:

   1.   Access point (AP) mode: The ESP32 is programmed to operate in AP mode, creating its own Wi-Fi network. This mode allows other devices to connect to the ESP32 as if it were a wireless router.

   2.   When a user wants to connect to the ESP32 they select the right available network and connect to it by following instructions which give user the correct SSID and password for the ESP32's AP.

   3.   After the connection is established user enters the ESP32's website by following the given IP address. On the website the user is prompted to enter their home Wi-Fi network's SSID and password. The user can now reset the device.

   4.   The entered SSID and password are securely stored in the ESP32's EEPROM. This allows the ESP32 to retain the network credentials even after power cycles.

   5.   If the entered SSID and password are correct and the user's home Wi-Fi network is available the Atom Echo tries to establish a connection with the user's home network.

   6.   Once successfully connected to the home Wi-Fi network, the ESP32 receives confirmation and establishes a stable connection. It can now communicate with other devices on the network and perform various tasks as required by the project. If the connection to the user's home network fails the ESP32. In case the connection to the user's home network fails, the ESP32 reverts back to the access point (AP) mode. This allows the user to have another opportunity to enter their SSID and password, providing a chance to correct any errors or address any connectivity issues. 

### Hardware and software Requirements

To achieve the project's intended purpose, certain HW and SW requirements must be fulfilled:
- Atom Echo is used as a device which has all the needed capabilities such as ESP32, EEPROM, USB-C etc. 
- Atom Echo needs a reliable power supply to operate. 
- Arduino IDE for programming the Atom Echo
- Relevant libraries

### Used libraries
- M5Atom.h: This library is specific to the M5Atom series of devices. It is also used for future upgrades which may include some of the capabilities of Atom Echo (microphone, speaker, LED etc.)
- WiFi.h: This library is essential for establishing Wi-Fi connectivity with the ESP32 
- WiFiAP.h: This library is used because the ESP32 has to function as an access point
- EEPROM.h: This library is required for storing and retrieving data in EEPROM of the device.


### Programming Challenges


The first problem I encountered was the trying to upload any code to the device. To resolve this, I installed all the necessary libraries and boards to the Arduino IDE and attempted to upload an example code provided within the IDE.

I proceeded by outlining a step-by-step guide detailing how I would achieve the required functionalities, following each step. The initial task was to configure the ESP32 to function as an Access Point (AP). While attempting to find the optimal method for presenting the user with input boxes for SSID and password, I encountered some challenges. However, after referring to examples available on the internet, I successfully modified them to save the user-inputted string values.

Next, I tried saving the string values to the EEPROM. To accomplish this, I utilized and adapted an Arduino IDE example. Once I was able to successfully save values to the EEPROM, I proceeded to save the user-inputted SSID and password, which proved to be a successfully.

Towards the end of the project, I encountered a few difficulties while attempting to write code that would first attempt to connect to the Wi-Fi network and, if unsuccessful, transition to acting as an Access Point. I realized that I needed to include the appropriate logic to handle the activation of the desired mode of the ESP32, ensuring the switch between AP and STA modes when needed.



### Future Considerations

There are few aspects that can be explored to further enhance its functionality. We should ensure the code's compatibility using different types of special characters etc. when user is entering values to the input boxes. 

It would be better to implement an automatic reconnection to Wi-Fi network in cases where the connection is lost due to factors such as being too far from the network or encountering interference. 

There should be a signal strength monitoring where if the signal strength falls below a certain threshold or if the connection quality deteriorates significantly, the device can proactively take appropriate actions.

The current HTML code for the website lacks visual appeal and may not provide an optimal user experience in terms of aesthetics. There should also be some kind of visual feedback for the user, for example using LED or speaker provided on the Atom Echo.



