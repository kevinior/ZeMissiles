//*****************************************************************************
//
// usb_keyboard_structs.c - Data structures defining this USB device.
//
// Copyright (c) 2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 9453 of the EK-LM4F120XL Firmware Package.
//
//*****************************************************************************

#include "inc/hw_types.h"
#include "driverlib/usb.h"
#include "usblib/usblib.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/usbhid.h"
#include "usblib/device/usbdhid.h"
#include "usblib/device/usbdhidkeyb.h"
#include "usb_keyboard_structs.h"

//*****************************************************************************
//
// The languages supported by this device.
//
//*****************************************************************************
const unsigned char g_pLangDescriptor[] =
{
    4,
    USB_DTYPE_STRING,
    USBShort(USB_LANG_EN_US)
};

//*****************************************************************************
//
// The manufacturer string.
//
//*****************************************************************************
const unsigned char g_pManufacturerString[] =
{
    (14 + 1) * 2,
    USB_DTYPE_STRING,
    'K', 0, 'e', 0, 'v', 0, 'i', 0, 'n', 0, ' ', 0, 'O', 0, '\'', 0, 'R', 0,
    'o', 0, 'u', 0, 'r', 0, 'k', 0, 'e', 0
};

//*****************************************************************************
//
// The product string.
//
//*****************************************************************************
const unsigned char g_pProductString[] =
{
    (13 + 1) * 2,
    USB_DTYPE_STRING,
    'F', 0, 'a', 0, 'k', 0, 'e', 0, ' ', 0, 'k', 0, 'e', 0, 'y', 0, 'b', 0,
    'o', 0, 'a', 0, 'r', 0, 'd', 0
};

//*****************************************************************************
//
// The serial number string.
//
//*****************************************************************************
const unsigned char g_pSerialNumberString[] =
{
    (8 + 1) * 2,
    USB_DTYPE_STRING,
    '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0, '8', 0
};

//*****************************************************************************
//
// The data interface description string.
//
//*****************************************************************************
const unsigned char g_pDataInterfaceString[] =
{
    (23 + 1) * 2,
    USB_DTYPE_STRING,
    'F', 0, 'a', 0, 'k', 0, 'e', 0, ' ', 0, 'k', 0, 'e', 0, 'y', 0, 'b', 0,
    'o', 0, 'a', 0, 'r', 0, 'd', 0, ' ', 0, 'I', 0, 'n', 0, 't', 0, 'e', 0,
    'r', 0, 'f', 0, 'a', 0, 'c', 0, 'e', 0
};

//*****************************************************************************
//
// The configuration description string.
//
//*****************************************************************************
const unsigned char g_pConfigString[] =
{
    (27 + 1) * 2,
    USB_DTYPE_STRING,
    'F', 0, 'a', 0, 'k', 0, 'e', 0, ' ', 0, 'k', 0, 'e', 0, 'y', 0, 'b', 0,
    'o', 0, 'a', 0, 'r', 0, 'd', 0, ' ', 0, 'C', 0, 'o', 0, 'n', 0, 'f', 0,
    'i', 0, 'g', 0, 'u', 0, 'r', 0, 'a', 0, 't', 0, 'i', 0, 'o', 0, 'n', 0
};

//*****************************************************************************
//
// The descriptor string table.
//
//*****************************************************************************
const unsigned char * const g_pStringDescriptors[] =
{
    g_pLangDescriptor,
    g_pManufacturerString,
    g_pProductString,
    g_pSerialNumberString,
    g_pDataInterfaceString,
    g_pConfigString
};

#define NUM_STRING_DESCRIPTORS (sizeof(g_pStringDescriptors) /                \
                                sizeof(unsigned char *))

//*****************************************************************************
//
// The bulk device initialization and customization structures. In this case,
// we are using USBBuffers between the bulk device class driver and the
// application code. The function pointers and callback data values are set
// to insert a buffer in each of the data channels, transmit and receive.
//
// With the buffer in place, the bulk channel callback is set to the relevant
// channel function and the callback data is set to point to the channel
// instance data. The buffer, in turn, has its callback set to the application
// function and the callback data set to our bulk instance structure.
//
//*****************************************************************************
//
//*****************************************************************************
//
// The HID keyboard device private instance data.
//
//
//*****************************************************************************
static tHIDKeyboardInstance g_sKeyboardInstance;

//*****************************************************************************
//
// The HID keyboard device application instance data.
//
//
//*****************************************************************************
extern struct keyState key_state = {
		false,
		0,
		false,
		0
};

const tUSBDHIDKeyboardDevice g_sKeyboardDevice =
{
		//
		// The Vendor ID you have been assigned by USB-IF.
		//
		USB_VID_STELLARIS,
		//
		// The product ID you have assigned for this device.
		//
		USB_PID_KEYBOARD,
		//
		// The power consumption of your device in milliamps.
		//
		500,
		//
		// The value to be passed to the host in the USB configuration descriptor’s
		// bmAttributes field.
		//
		USB_CONF_ATTR_SELF_PWR,
		//
		// A pointer to your keyboard callback event handler.
		//
		MyKeyboardHandler,
		//
		// A value that you want passed to the callback alongside every event.
		//
		(void *)&key_state,
		//
		// A pointer to your string table.
		//
		g_pStringDescriptors,
		//
		// The number of entries in your string table. This must equal
		// (1 + (5 * (num languages))).
		//
		NUM_STRING_DESCRIPTORS,
		//
		// A pointer to the private instance data allocated for the API to use.
		//
		&g_sKeyboardInstance
};
