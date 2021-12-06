# Bluetooth_Low_Energy_Introduction

**Prerequisites:** Download Visual Studio Code, and nRF Connect for Desktop -> Toolchain manager and install the latest version of nRF Connect SDK (1.7.1 when this guide was written). Install nRF Connect for Visual studio (instructions from Toolchain Manager).
Start by adding the "hello_world" sample from *NCS\zephyr\samples\hello_world* as an application in nRF Connect for Visual Studio Code. </br></br>

# HW requirements
- nRF52840 Development Kit. 

# SW Requirements
As mentioned in the prerequisites, you'll need:
- nRF Connect for Desktop
- Visual Studio Code
- nRF Connect for Visual Studio Code plugin (If you want to, you can use *west* directly instead of nRF Connect for Visual Studio code, but we will use VS Code in this guide).



</br>

This tutorial will show you how to create a custom service with two custom value characteristics. One which the central can read and subscribe to (notifications) and one that the central can write to. We will be using the nRF Connect SDK (v1.7.1 or later). This tutorial can be seen as a practical implementations of the guides:
- [(Bluetooth low energy Advertising, a beginner's tutorial)](https://devzone.nordicsemi.com/guides/short-range-guides/b/bluetooth-low-energy/posts/ble-advertising-a-beginners-tutorial)
- [(Bluetooth low energy Services, a beginner's tutorial)](https://devzone.nordicsemi.com/guides/short-range-guides/b/bluetooth-low-energy/posts/ble-services-a-beginners-tutorial)
- [(Bluetooth low energy Characteristics, a beginner's tutorial)](https://devzone.nordicsemi.com/guides/short-range-guides/b/bluetooth-low-energy/posts/ble-characteristics-a-beginners-tutorial)
</br>

Although these tutorials were written a while ago, when the nRF5 SDK was still the main SDK for nRF devices, the theory is the same, but in this guide we will be using the nRF Connect SDK, and the Softdevice Controller instead of the nRF5 SDK and the Softdevice.</br>
If you are looking for the nRF5 SDK version of this guide, please see this [(repository)](https://github.com/edvinand/custom_ble_service_example).
</br>
The aim of this tutorial is to simply create one service with two characteristics without too much theory in between the steps. You don't need to download any .c or .h files, as we will start with the hello_world sample as a template.

# Tutorial Steps
### Step 1 - Getting started

If you haven't done it already, start by setting up nRF Connect for Visual Studio code by setting the environment parameters. Under the nRF Connect tab in Visual Studio Code (VSC) click "Open welcome page" and click "Quick Setup". 
Visual Studio Code settings | 
------------ |
<img src="https://github.com/edvinand/bluetooth_intro/blob/main/images/welcome_page.PNG" width="1000"> |

These are my settings, but the path may vary in your environment.
</br>
Next we need to add the hello_world sample as our application. The path to the sample is *NCS\zephyr\samples\hello_world*. Note that this sample is from the "zephyr" part of nRF Connect SDK (NCS), but there are plenty of samples found in *NCS\nrf\samples* as well. 
</br>
Start by selecting *Create a new application from sample* in the *nRF Connect* -> *Welcome* tab, and choose settings similar to this the screenshot below. I recommend that you create a folder outside the NCS root folder where you store your custom applications. In this case, that is the folder where the new application is stored is the *custom_ble_sample*, and the name of the application is *remote_controller*. The application that we copy to our custom folder with our custom name is the *hello_world* sample. 

Setup Application from Sample | 
------------ |
<img src="https://github.com/edvinand/bluetooth_intro/blob/main/images/application_from_sample.PNG" width="600"> |

</br>
Now we have copied the sample to our custom applications folder, but we need to create a build environment before we can compile and flash the sample to our board. 
Below the *nRF Connect* -> *APPLICATIONS* tab, expand the "remote_controller" application, and click the *No build configurations*, which will create a new build configuration.
The only thing you need to do here is to set the board that you are using. Depending on the board you are using, you need to enter the NCS name of the DK:
- nRF52832 DK: nrf52dk_nrf52832
- nRF52833 DK: nrf52833dk_nrf52833
- nRF52840 DK: nrf52840dk_nrf52840
If you are using another board than the ones listed above, you can probably find the name of it in the folder: *NCS\zephyr\boards\arm*. When you have entered the name of your board, click Build Configuration.
When you built the configuration, it should compile/build the sample as well. If everything went well, you should be able to connect your DK using the micro USB port on the short end of the DK, and flash using the Flash button from the *ACTIONS* tab. </br>
If everything goes well, you should have flashed the *hello_world* sample to your board. We can see from the main.c file that it is printing some data using printk(), but unless we connect the nRF Terminal in VSC or another UART terminal, we will not see what it prints. Therefore, in the *CONNECTED DEVICES* tab, you should see your DK. Click the arrow on the left hand side to expand the board and click the left icon you see when you hover the mouse over the line saying VCOM0

Connect to board's UART | 
------------ |
<img src="https://github.com/edvinand/bluetooth_intro/blob/main/images/connect_uart.PNG" width="300"> |
</br>
A popup will occur with some UART settings. Just hit the enter key to select *115200 8n1*, and open the *NRF_TERMINAL* in your bottom terminal. It should print something like:
*Hello World! nrf52840dk_nrf52840*

### Step 2 - Enabling some basic application features
So 
