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
Memory Settings Segger Embedded Studio | 
------------ |
<img src="https://github.com/edvinand/bluetooth_introe/blob/master/images/welcome_page.PNG" width="1000"> |

