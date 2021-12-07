
#include <zephyr.h>
#include <logging/log.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/hci.h>

/** @brief UUID of the Remote Service. **/
#define BT_UUID_REMOTE_SERV_VAL \
	BT_UUID_128_ENCODE(0xe9ea0001, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

/** @brief UUID of the Button Characteristic. **/
#define BT_UUID_REMOTE_BUTTON_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0002, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

#define BT_UUID_REMOTE_SERVICE  BT_UUID_DECLARE_128(BT_UUID_REMOTE_SERV_VAL)
#define BT_UUID_REMOTE_BUTTON_CHRC 	BT_UUID_DECLARE_128(BT_UUID_REMOTE_BUTTON_CHRC_VAL)



void set_button_value(uint8_t btn_value);
int bluetooth_init(struct bt_conn_cb *bt_cb);