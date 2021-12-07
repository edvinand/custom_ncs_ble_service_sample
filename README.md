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
- [Bluetooth low energy Advertising, a beginner's tutorial](https://devzone.nordicsemi.com/guides/short-range-guides/b/bluetooth-low-energy/posts/ble-advertising-a-beginners-tutorial)
- [Bluetooth low energy Services, a beginner's tutorial](https://devzone.nordicsemi.com/guides/short-range-guides/b/bluetooth-low-energy/posts/ble-services-a-beginners-tutorial)
- [Bluetooth low energy Characteristics, a beginner's tutorial](https://devzone.nordicsemi.com/guides/short-range-guides/b/bluetooth-low-energy/posts/ble-characteristics-a-beginners-tutorial)
</br>

Although these tutorials were written a while ago, when the nRF5 SDK was still the main SDK for nRF devices, the theory is the same, but in this guide we will be using the nRF Connect SDK, and the Softdevice Controller instead of the nRF5 SDK and the Softdevice.</br>
If you are looking for the nRF5 SDK version of this guide, please see [this repository](https://github.com/edvinand/custom_ble_service_example).
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
<img src="https://github.com/edvinand/bluetooth_intro/blob/main/images/application_from_sample.PNG"> |

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
<img src="https://github.com/edvinand/bluetooth_intro/blob/main/images/connect_uart.PNG"> |
</br>
A popup will occur with some UART settings. Just hit the enter key to select *115200 8n1*, and open the *NRF_TERMINAL* in your bottom terminal. It should print something like:
*Hello World! nrf52840dk_nrf52840*

### Step 2 - Enabling some basic application features
Congratulations! You have built and flashed our first application. Let's move on by doing some minor modifications. If you explore some of the samples from the *nrf* folder in NCS, you'll see that most of them use our logging module, which is what we will use as well. In order to do so, please replace the line '#include <sys/printk.h>' with '#include <logging/log.h>. In order to use the log module, we need to add a few things in the prj.conf file. You will find it from the application tab (called remote_controller if you didn't change it) -> Input files -> prj.conf. At this point, it should just say `#nothing here`.
</br>
Add the following:
```
# Configure logger
CONFIG_LOG=y
CONFIG_USE_SEGGER_RTT=n
CONFIG_LOG_BACKEND_UART=y
CONFIG_LOG_DEFAULT_LEVEL=3
```
They are quite self explaining, but what we are doing here is enabling the log module, deselecting the default RTT backend, selecting the UART backend, and setting the log level to 3 (INFO). <br>
Back in main.c, try replacing the `printk()` with `LOG_INF();` and add the following snippet before `void main(void)`
```C
#define LOG_MODULE_NAME app
LOG_MODULE_REGISTER(LOG_MODULE_NAME);
```
Compile and flash the application again, and you should see that it still prints over UART, but now we are using the log module

</br>

#### Configure buttons and LEDs
Before we start adding Bluetooth, we want to set up some LEDs that we can use to indicate that our application is still running, and hasn't crashed, and some buttons that we can use later to trigger certain BLE calls.
Start by including <dk_buttons_and_leds.h> in your main.c file.
Next, create a function to initiate the LEDs and buttons. I will call mine `static void configure_dk_buttons_leds(void)`.
The first thing we need to do in this function is to enable the LEDs. Looking in dk_buttons_and_leds.h, we can look for a function that does about that. Try adding `dk_leds_init()` to your configure_dk_buttons_leds() function. Since this function returns and int, we would like to check the return value. 

```C
    int err;
    err = dk_leds_init();
    if (err) {
        LOG_ERR("Couldn't init LEDS (err %d)", err);
    }
```
Let us add a specific LED and a blinking interval near the top of main.c
```C
#define RUN_STATUS_LED DK_LED1
#define RUN_LED_BLINK_INTERVAL 1000
```
Open dk_buttons_and_leds.h to see if there is any ways you can turn on and off this LED from your main function. Our goal is to toggle the LED in a `for(;;)` loop (equivalent to a while(true) loop). There are several ways to do this. Try to find one that works. </br>
*Hint: You can use k_sleep() to wait a given amount of time, and there is a macro called K_MSEC() that takes an input of ms, and converts it to ticks.*

Now, let us look for a function that can enable the buttons in the dk_and_leds_init.h file. Remember to check the return value of the button init function. </br>
*Hint: As this function initializes our buttons, it has an input parameter which is a handler.* 
</br> 
In your button handler try using the log module to print something whenever it is called. We will tweak it later.
</br>
If you try to build your application at this point, you will see that it fails because it can't find any references to your LED or buttons init function, even though you included dk_buttons_and_leds.h. The reason for this is that we didn't include the dk_buttons_and_leds.c file. We need to tell our application to do so. There are two ways of doing this. If you create your own files, you can add them manually, which we will do later for some custom files. But for now we want to add a file that belongs to NCS, and therefore we include it using configuration switches. 
</br>
In prj.conf, add the following:
```
# Configure buttons and LEDs.
CONFIG_GPIO=y
CONFIG_DK_LIBRARY=y
```
This snippet will enable the GPIOs and include the DK library. The way this is done in NCS/Zephyr is a bit complex. If you are interrested in how this works, you can look into the CMakeLists.txt file found in NCS\nrf\lib\CMakeLists.txt, and see how it includes stuff based on the configurations. For now we will accept that this is just how it works.
After adding the configurations in prj.conf your project should compile, and something should be printed in the log whenever you press or release a button. Remember to call `configure_dk_buttons_leds()` in your main() function.

</br>
If you successfully compiled your application and flash it, you should now see that LED1 toggles every second, and that you receive a callback whenever a button is pressed or released.

**Challenge:** </br>
***Without peeking at the solution below, try to implement your button handler so that it stores the button number of the button that was pressed, and prints it in the log only when the button was pressed (and not released). Try printing out the parameters `button_state` and `has_changed` to see what they look like when you press the buttons. You may find a methid that is even more elegant than the suggested method below.***
</br>
</br>
At this point, your main.c file should look something like this. You can use this as a template if you got stuck somewhere before this point:

```C
/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <logging/log.h>
#include <dk_buttons_and_leds.h>

#define LOG_MODULE_NAME app
LOG_MODULE_REGISTER(LOG_MODULE_NAME);
#define RUN_STATUS_LED DK_LED1
#define RUN_LED_BLINK_INTERVAL 1000

/* Callbacks */
void button_handler(uint32_t button_state, uint32_t has_changed)
{
	int button_pressed = 0;
	if (has_changed & button_state)
	{
		switch (has_changed)
		{
			case DK_BTN1_MSK:
				button_pressed = 1;
				break;
			case DK_BTN2_MSK:
				button_pressed = 2;
				break;
			case DK_BTN3_MSK:
				button_pressed = 3;
				break;
			case DK_BTN4_MSK:
				button_pressed = 4;
				break;
			default:
				break;
		}
		LOG_INF("Button %d pressed.", button_pressed);
	}
}

/* Configurations */
static void configure_dk_buttons_leds(void)
{
    int err;
    err = dk_leds_init();
    if (err) {
        LOG_ERR("Couldn't init LEDS (err %d)", err);
    }
    err = dk_buttons_init(button_handler);
    if (err) {
        LOG_ERR("Couldn't init buttons (err %d)", err);
    }
}

/* Main */
void main(void)
{
    int blink_status = 0;
	LOG_INF("Hello World! %s\n", CONFIG_BOARD);

    configure_dk_buttons_leds();

    LOG_INF("Running...");
    for (;;) {
        dk_set_led(RUN_STATUS_LED, (blink_status++)%2);
        k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
    }
}
```

### Step 3 - Adding Bluetooth
It is finally time to add bluetooth to our project. A hint was given in the project name, but in case you missed it, we will write an application that mimics some sort of bluetooth remote, where we will be able to send button presses to a connected Bluetooth Low Energy Central. We will also add the oppurtynity to write back to the remote control. That may not be a typical feature for a remote control, but for the purpose of learning how to communicate in both directions we will add this. The connected central can either be your phone, a computer, or another nRF52. For this guide we will use a separate DK and nRF Connect for Desktop -> Bluetooth, but if you only have one DK, you can use [nRF Connect for iOS or Android.](https://www.nordicsemi.com/Products/Development-tools/nRF-Connect-for-mobile)
</br>
</br>
Because we want to keep our main.c file as clutter free as possible, we will try to do most of the bluetooth configuration and handling in another file, and only push certain events back to main.c. Therefore we will start by adding a few custom files. Create a folder named `remote_service` inside your application file: *remote_controller\src\remote_service\.* You can either do this from VSC or your operating system. Inside this folder, create two files: `remote.h` and `remote.c`. To include these custom files to your project, open CMakeLists.txt, and add the following snippet at the end:

```C
# Custom files and folders

target_sources(app PRIVATE
    src/remote_service/remote.c
)

zephyr_library_include_directories(src/remote_service)
```
*If you wanted to add more .c files, you could do so by separating them using `;` after `remote.c`. and have one file per line.*
</br>
If you build your application you should see that the remote.c file appears under your *REMOTE_CONTROLLER* tab:

Application Tree | 
------------ |
<img src="https://github.com/edvinand/bluetooth_intro/blob/main/images/application_tree.PNG"> |

</br>
Open remote.c and add the line at the very top: </br>

```C
#include "remote.h"
```

If you right click "remote.h" that you just wrote, and click "Go to Definition" it should open the remote.h file in VSC. In remote.h, add:

```C
#include <zephyr.h>
#include <logging/log.h>
```

Now, try to create a function called `bluetooth_init()` in your remote.c file that you also need to declare in remote.h. Make the function return `0`, and check this return value in `main()`. Add whatever is needed in these two files so that you can use this function to log "Initializing Bluetooth". Remember to include remote.h from your main.c file.
</br>
*Hint 1: You shouldn't need to include any more files in remote.c.*
</br>
*Hint 2: Give remote.c another log module name, so that it is easy to see from the log what file that printed what lines.*
</br>


Now that we have our own file to do most of the Bluetooth, let us start by adding these four header files in our remote.h file:
```C
#include <bluetooth/bluetooth.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/hci.h>
```
For most Bluetooth Low Energy, these four files will do the job. Let us start by adding `bt_enable()` to our `bluetooth_init()` function. In order to see what input bt_enable() takes, you may want to build, and probably you will find out that bt_enable is not defined yet. The reason for this is, like earlier, that we have not enabled Bluetooth in our prj.conf file. Try to add the following:
```
# Configure Bluetooth
CONFIG_BT=y
CONFIG_BT_PERIPHERAL=y
CONFIG_BT_DEVICE_NAME="Remote_controller"
CONFIG_BT_DEVICE_APPEARANCE=0
CONFIG_BT_MAX_CONN=1
CONFIG_BT_LL_SOFTDEVICE=y

CONFIG_ASSERT=y
````
What we do here is:
- Enable Bluetooth,
- Support the peripheral (advertising) role
- Set our device_name, which we will use later
- Set the appearance. Look in the description of this configuration to see what this does.
- Set the maximum simultaneous connections to 1.
- Tell it to use the Nordic Softdevice Controller. 

</br>
After this sidetrack (rebuild/recompilation required), it is time to see what bt_enable does. In nRF Connect for VS Code, if you hold ctrl and hover bt_enable(), you should see the declaration of the function. If you ctrl click it, it should bring you to the definition. We can use this to see what it returns and what input parameters it takes.
</br>

VSC hint | 
------------ |
<img src="https://github.com/edvinand/bluetooth_intro/blob/main/images/VSC_hint.png"> |

</br>
So we see that it returns an `int` and it takes an input `bt_ready_cb_t`. By going to the definition of `bt_ready_cb_t` you'll see that it is:

```C
typedef void (*bt_ready_cb_t)(int err);
```

This means a function pointer. It means that it takes a callback function as an input parameter. The callback is on the form: `void callback_name(int err)`. Let us use a callback called `bt_ready`, which we will implement above `bluetooth_init()` in remote.c, and pass it onto `bt_enable()`.

```C
void bt_ready(int err)
{
    if (err) {
        LOG_ERR("bt_enable returned %d", err);
    }
}
```

We want to wait for our callback before we continue with our application. In order to do this, we will use a semaphore. Define the semaphore near the top of remote.c:

```C
static K_SEM_DEFINE(bt_init_ok, 0, 1);
```

After `bt_enable(bt_ready);` try to take the semaphore, so that the application waits until it is given from somewhere else, and then try to give it in the bt_ready callback. </br>
*Hint: The k_sem_take() requires a timeout. You can use K_FOREVER.*

</br>
</br>

#### Advertising
So far we have enabled Bluetooth, but now we didn't use it for anything. Let us add some Bluetooth advertising. We want to include two things in our advertisements. The device name and the UUID of the service that we will implement later. Let us start by adding the UUID (Universally Unique Identifier). I typically use an online UUID generator. Try using [this online UUID Generator](https://www.uuidgenerator.net/version4). In my case, I got a UUID which I translated to this format:
 
 ```C
 /* Add this to remote.h */
 /** @brief UUID of the Remote Service. **/
#define BT_UUID_REMOTE_SERV_VAL \
	BT_UUID_128_ENCODE(0xe9ea0001, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)
```

Copy your own generated UUID into the same format, and set the two last bytes of the first sections to 0001, like I did. This is so that it is easier to recognize them later. 
Also add the following line below the definition of your UUID:

```C
#define BT_UUID_REMOTE_SERVICE  BT_UUID_DECLARE_128(BT_UUID_REMOTE_SERV_VAL)
```

These are just two ways to define the same UUID, which we will use later. Now, open remote.c, and let us define the advertising packets. We will use two advertising packets. The normal advertising data, and something called scan response data.

Add this a suitable place in remote.c:

```C
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME)-1)


static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)
};

static const struct bt_data sd[] = {
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_REMOTE_SERV_VAL),
};
```

So we use the CONFIG_BT_DEVICE_NAME from our prj.conf file as our device name, and we apply this name in our advertising packet `ad[]`. In our Scan response packet, `sd[]` we add our randomly generated UUID. Now that we have the data we want to advertise, we can start the advertising from bluetooth_init(), after the bt_init_ok semaphore has been taken.

```C
/* This snippet belongs in bluetooth_init() in remote.c */
    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
    if (err){
        LOG_ERR("couldn't start advertising (err = %d", err);
        return err;
    }
```

Now your device should advertise if you flash it with the latest build. Open nRF Connect for Desktop/iOS/Android and start scanning for the device. If there are many BLE devices nearby, try to sort by RSSI (Received Signal Strength Indicator), or ad a filter to the advertising name:
</br>

Scan uisng nRF Connect for Desktop | 
------------ |
<img src="https://github.com/edvinand/bluetooth_intro/blob/main/images/scan_advertisements.PNG"> |

*Note: In your case it probably will not say "Remote Service" in the Services field, but rather the UUID that you generated. If you want to save this custom UUID in nRF Connect for Desktop, click the gear icon (settings) on the nrf5x device, and select "Open UUID definitions file". See if you can copy the template of one of the services, and insert your own UUID.*
</br>
</br>
You can actually connect to your device, since we claimed in the `BT_LE_ADV_CONN` that we are connectable. However, if you try to connect to it, you will see that other than the Generic Attribute and the Generic Access services, we don't actually have the custom service that we claimed to have in the advertising packet. We will fix that later, but first, let us try to inform our application that something actually connected to us.
</br>
</br>
We want to receive these events in our main.c file, so that we can keep track of the state of our device. Let us start by adding a struct containing the callbacks in main.c:
</br>

```C
struct bt_conn_cb bluetooth_callbacks = {
	.connected 	= on_connected,
	.disconnected 	= on_disconnected,
};
```

**Challenge**
***Implement these callbacks by looking at the bt_conn_cb struct definition (ctrl click it). For now, you can just print something using `LOG_INF()` in the events. Then try to pass the bluetooth_callbacks on into `bluetooth_init()`, and register the callbacks using `bt_conn_cb_register()` before the call to `bt_enable()`. If you are stuck, you can find a solution below.***
</br>
</br>
If you followed the guide this far, your files should look something like this. You can use this in case you got stuck somewhere. Please note that I also added some new code to the connected and disconnected events in main.c, and a current_conn parameter to keep track of the current connection. 
</br>
[main.c](https://github.com/edvinand/bluetooth_intro/blob/main/temp_files/snapshot1/main.c)</br>
[remote.c](https://github.com/edvinand/bluetooth_intro/blob/main/temp_files/snapshot1/remote_service/remote.c)</br>
[remote.h](https://github.com/edvinand/bluetooth_intro/blob/main/temp_files/snapshot1/remote_service/remote.h)</br>


### Step 4 - Adding our First Bluetooth Service
Let us add the service that we claim that we have when we advertise. We will use the macro BT_GATT_SERVICE_DEFINE to add our service. It is quite simple at the same time as it is quite complex. When we use this macro to create and add our service, the rest is done "under the hood" of NCS/Zephyr. By just adding this snippet to remote.c

```C
BT_GATT_SERVICE_DEFINE(remote_srv,
BT_GATT_PRIMARY_SERVICE(BT_UUID_REMOTE_SERVICE),
);
```

And voila! We have our first Bluetooth Low Energy service. Try to connect to it using nRF Connect, and see that you can see the service.
Our first service | 
------------ |
<img src="https://github.com/edvinand/bluetooth_intro/blob/main/images/first_service.PNG"> |

However, a service without any characteristics isn't very impressive. Let us add a characteristic that we can read from our Central.
