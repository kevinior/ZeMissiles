//*****************************************************************************
//
// usb_dev_bulk.c - Main routines for the generic bulk device example.
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

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "usblib/usblib.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/usbhid.h"
#include "usblib/device/usbdhid.h"
#include "usblib/device/usbdhidkeyb.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"

#include "usb_keyboard_structs.h"

//*****************************************************************************
//
// Fire Ze Missiles - pretends to be a USB keyboard
//
//*****************************************************************************


//*****************************************************************************
//
// The system tick rate expressed both as ticks per second and a millisecond
// period.
//
//*****************************************************************************
#define SYSTICKS_PER_SECOND     100
#define SYSTICK_PERIOD_MS       (1000 / SYSTICKS_PER_SECOND)

// The USB device takes a while from the connected event to being ready
// to actually do anything.
#define USB_READY_DELAY 110

// How quickly should we flash the button LED after firing? In ms.
#define BUTTON_FLASH_PERIOD		500
#define BUTTON_FLASH_TICKS		((BUTTON_FLASH_PERIOD / 2) / SYSTICK_PERIOD_MS)

//*****************************************************************************
//
// The global system tick counter.
//
//*****************************************************************************
volatile unsigned long g_ulSysTickCount = 0;

//*****************************************************************************
//
// Variables tracking transmit and receive counts.
//
//*****************************************************************************
volatile unsigned long g_ulTxCount = 0;
volatile unsigned long g_ulRxCount = 0;
#ifdef DEBUG
unsigned long g_ulUARTRxErrors = 0;
#endif

//*****************************************************************************
//
// Debug-related definitions and declarations.
//
// Debug output is available via UART0 if DEBUG is defined during build.
//
//*****************************************************************************
#ifdef DEBUG
//*****************************************************************************
//
// Map all debug print calls to UARTprintf in debug builds.
//
//*****************************************************************************
#define DEBUG_PRINT UARTprintf

#else

//*****************************************************************************
//
// Compile out all debug print calls in release builds.
//
//*****************************************************************************
#define DEBUG_PRINT while(0) ((int (*)(char *, ...))0)
#endif

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
    UARTprintf("Error at line %d of %s\n", ulLine, pcFilename);
    while(1)
    {
    }
}
#endif

//*****************************************************************************
//
// Interrupt handler for the system tick counter.
//
//*****************************************************************************
void
SysTickIntHandler(void)
{
    //
    // Update our system tick counter.
    //
    g_ulSysTickCount++;
}

//*****************************************************************************
//
// Handles keyboard notifications.
//
// \param pvCBData is the client-supplied callback pointer for this channel.
// \param ulEvent identifies the event we are being notified about.
// \param ulMsgValue is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// \return The return value is event-specific.
//
//*****************************************************************************
unsigned long
MyKeyboardHandler(
		void *pvCBData,
		unsigned long ulEvent,
		unsigned long ulMsgValue,
		void *pvMsgData
)
{
	struct keyState *state = (struct keyState *)pvCBData;

    //
    // Which event are we being sent?
    //
    switch(ulEvent)
    {
        //
        // We are connected to a host and communication is now possible.
        //
        case USB_EVENT_CONNECTED:
        {
        	DEBUG_PRINT("%d: Host connected.\n", g_ulSysTickCount);
            state->connected = true;
            state->ready_at = g_ulSysTickCount + USB_READY_DELAY;
            state->last_pressed = 0;
            state->key_pressed = false;
            break;
        }

        //
        // The host has disconnected.
        //
        case USB_EVENT_DISCONNECTED:
        {
            state->connected = false;
            DEBUG_PRINT("%d: Host disconnected.\n", g_ulSysTickCount);
            break;
        }

        //
        // Keyboard events (for debug purposes)
        case USBD_HID_KEYB_EVENT_SET_LEDS:
        {
        	state->connected = true;
        	DEBUG_PRINT("%d: USBD_HID_KEYB_EVENT_SET_LEDS %08x\n", g_ulSysTickCount, ulMsgValue);
        	break;
        }

        //
        // Ignore all other events and return 0.
        //
        default:
        {
        	DEBUG_PRINT("%d: EVENT %x\n", g_ulSysTickCount, ulEvent);
            break;
        }
    }

    return(0);
}

//*****************************************************************************
//
// States for the main loop
//
//*****************************************************************************
typedef enum
{
	AS_DISABLED = 0,
	AS_ENABLED,
	AS_ARMED,
	AS_FIRING_PRESS,
	AS_FIRING_RELEASE,
	AS_FIRED_1,
	AS_FIRED_2,
	AS_NUMSTATES
} AppState;

#ifdef DEBUG
char *g_stateNames[AS_NUMSTATES] = {
		"DISABLED",
		"ENABLED",
		"ARMED",
		"FIRING_PRESS",
		"FIRING_RELEASE",
		"FIRED_1",
		"FIRED_2"
};
#endif

//*****************************************************************************
//
// Macros for testing switch states
//
//*****************************************************************************
#define KEYSWITCH_PIN	GPIO_PIN_1
#define TOGGLE_PIN		GPIO_PIN_2
#define BUTTON_PIN		GPIO_PIN_4
#define KEYSWITCH_ON(S)		((S & KEYSWITCH_PIN) == 0)
#define KEYSWITCH_OFF(S)	((S & KEYSWITCH_PIN) != 0)
#define TOGGLE_ON(S)		((S & TOGGLE_PIN) != 0)
#define TOGGLE_OFF(S)		((S & TOGGLE_PIN) == 0)
#define BUTTON_ON(S)		((S & BUTTON_PIN) == 0)
#define BUTTON_OFF(S)		((S & BUTTON_PIN) != 0)

//*****************************************************************************
//
// Macros for switch LEDs
//
//*****************************************************************************
#define TOGGLE_LED_PIN		GPIO_PIN_3
#define BUTTON_LED_PIN		GPIO_PIN_5

tBoolean usb_key(tBoolean press)
{
	if (   !key_state.connected
			|| (g_ulSysTickCount < key_state.ready_at) )
		return false;

	// press the key
	DEBUG_PRINT("%d: usb_key %d\n", g_ulSysTickCount, press);
	unsigned long status = USBDHIDKeyboardKeyStateChange(
		(void *)&g_sKeyboardDevice,
		0,
		HID_KEYB_USAGE_ENTER,
		press
	);
	if (status != KEYB_SUCCESS)
	{
		UARTprintf("usb_key error %d\n", status);
		return false;
	}
	key_state.key_pressed = press;
	return true;
}

//*****************************************************************************
//
// This is the main application entry function.
//
//*****************************************************************************
int
main(void)
{
    unsigned long ulLoop;
    AppState state, oldState;
    unsigned long stateEntered;

    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    ROM_FPULazyStackingEnable();

    //
    // Set the clocking to run from the PLL at 50MHz
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    //
    // Configure the relevant pins such that UART0 owns them.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Enable the GPIO pins for the LED (PF2 & PF3).  
    //
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2);
    
    //
    // Enable the GPIO port used for the switches and LEDs.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    //
    // Enable the GPIO inputs for the switches (PE1, PE2, PE4)
    // and configure them appropriately.
    //
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4);
    ROM_GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    ROM_GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

    //
    // Enable the GPIO outputs for the LEDs (PE3, PE5).
    //
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_3|GPIO_PIN_5);
    ROM_GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_3|GPIO_PIN_5, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3|GPIO_PIN_5, GPIO_PIN_3|GPIO_PIN_5); // LEDs off

    //
    // Open UART0 and show the application name on the UART.
    //
    UARTStdioInit(0);
    UARTprintf("\033[2JUSB keyboard testing\n");
    UARTprintf("--------------------\n\n");

    //
    // Enable the GPIO peripheral used for USB, and configure the USB
    // pins.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //
    // Enable the system tick.
    //
    ROM_SysTickPeriodSet(ROM_SysCtlClockGet() / SYSTICKS_PER_SECOND);
    ROM_SysTickIntEnable();
    ROM_SysTickEnable();

    //
    // Tell the user what we are up to.
    //
    DEBUG_PRINT("Configuring USB\n");

    //
    // Set the USB stack mode to Device mode with VBUS monitoring.
    //
    USBStackModeSet(0, USB_MODE_FORCE_DEVICE, 0);

    //
    // Pass our device information to the USB library and place the device
    // on the bus.
    //
    if (USBDHIDKeyboardInit(0, &g_sKeyboardDevice) != &g_sKeyboardDevice)
    	UARTprintf("USBDHIDKeyboardInit failed\n");

    //
    // Wait for initial configuration to complete.
    //
    DEBUG_PRINT("Waiting for host...\n");

    state = oldState = AS_DISABLED;
    stateEntered = g_ulSysTickCount;

    //
    // Main application loop.
    //
    while(1)
    {
        //
        // Delay for a bit.
        //
        for(ulLoop = 0; ulLoop < 250000; ulLoop++)
        {
        }

        //
        // Turn off the Green LED.
        //
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);

		//
        // Turn off the switch LEDs
        //
        ROM_GPIOPinWrite(GPIO_PORTE_BASE, TOGGLE_LED_PIN|BUTTON_LED_PIN, TOGGLE_LED_PIN|BUTTON_LED_PIN);

        //
        // Read the state of the switches.
        //
        long switches = ROM_GPIOPinRead(GPIO_PORTE_BASE, KEYSWITCH_PIN|TOGGLE_PIN|BUTTON_PIN);
        //DEBUG_PRINT("%d: state %08x\n", g_ulSysTickCount, switches);

        switch (state)
        {
        	case AS_DISABLED:
        		if (KEYSWITCH_ON(switches))
        			state = AS_ENABLED;
        		break;

        	case AS_ENABLED:
        		if (KEYSWITCH_OFF(switches))
        			state = AS_DISABLED;
        		else if (TOGGLE_ON(switches))
        			state = AS_ARMED;
        		break;

        	case AS_ARMED:
        		if (KEYSWITCH_OFF(switches))
        			state = AS_DISABLED;
        		else if (TOGGLE_OFF(switches))
        			state = AS_ENABLED;
        		else if (BUTTON_ON(switches))
        			state = AS_FIRING_PRESS;
        		break;

        	case AS_FIRING_PRESS:
        		if (usb_key(true))
        			state = AS_FIRING_RELEASE;
        		break;

        	case AS_FIRING_RELEASE:
        		if (usb_key(false))
        			state = AS_FIRED_1;
        		break;

        	case AS_FIRED_1:
        		if (KEYSWITCH_OFF(switches) && TOGGLE_OFF(switches))
        			state = AS_DISABLED;
        		else if ((g_ulSysTickCount - stateEntered) > BUTTON_FLASH_TICKS)
        			state = AS_FIRED_2;
        		break;

        	case AS_FIRED_2:
        		if (KEYSWITCH_OFF(switches) && TOGGLE_OFF(switches))
        			state = AS_DISABLED;
        		else if ((g_ulSysTickCount - stateEntered) > BUTTON_FLASH_TICKS)
        			state = AS_FIRED_1;
        		break;
        }

        if (state != oldState)
        {
#ifdef DEBUG
        	UARTprintf("%d: state %s(%d) -> %s\n", g_ulSysTickCount, g_stateNames[oldState], stateEntered, g_stateNames[state]);
#endif
        	stateEntered = g_ulSysTickCount;
        }
        oldState = state;

        if (state != AS_DISABLED)
        	ROM_GPIOPinWrite(GPIO_PORTE_BASE, TOGGLE_LED_PIN, 0);
        if ((state == AS_ARMED) || (state == AS_FIRING_PRESS)
        		|| (state == AS_FIRING_RELEASE) || (state == AS_FIRED_1))
        	ROM_GPIOPinWrite(GPIO_PORTE_BASE, BUTTON_LED_PIN, 0);
        if ((state == AS_FIRING_PRESS) || (state == AS_FIRING_RELEASE)
        		|| (state == AS_FIRED_1) || (state == AS_FIRED_2))
        	ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);

		if (   !key_state.connected
			|| (g_ulSysTickCount < key_state.ready_at) )
			continue;
    }
}
