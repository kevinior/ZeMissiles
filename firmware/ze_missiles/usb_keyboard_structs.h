#ifndef _USB_KEYBOARD_STRUCTS_H_
#define _USB_KEYBOARD_STRUCTS_H_

struct keyState {
	tBoolean connected;
	unsigned long ready_at;
	tBoolean key_pressed;
	unsigned long last_pressed;
};

extern struct keyState key_state;

extern unsigned long MyKeyboardHandler(
		void *pvCBData,
		unsigned long ulEvent,
		unsigned long ulMsgValue,
		void *pvMsgData
);
extern const tUSBDHIDKeyboardDevice g_sKeyboardDevice;

#endif
