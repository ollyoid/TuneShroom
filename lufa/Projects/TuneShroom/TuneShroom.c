/*
             LUFA Library
     Copyright (C) Dean Camera, 2019.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2019  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the TuneShroom
 */

#include "TuneShroom.h"

/** LUFA MIDI Class driver interface configuration and state information. This structure is
 *  passed to all MIDI Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */

uint8_t readTouch(TouchPad *pad);


USB_ClassInfo_MIDI_Device_t Keyboard_MIDI_Interface =
	{
		.Config =
			{
				.StreamingInterfaceNumber = INTERFACE_ID_AudioStream,
				.DataINEndpoint           =
					{
						.Address          = MIDI_STREAM_IN_EPADDR,
						.Size             = MIDI_STREAM_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint          =
					{
						.Address          = MIDI_STREAM_OUT_EPADDR,
						.Size             = MIDI_STREAM_EPSIZE,
						.Banks            = 1,
					},
			},
	};

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
		.Config =
			{
				.ControlInterfaceNumber         = INTERFACE_ID_CDC_CCI,
				.DataINEndpoint                 =
					{
						.Address                = CDC_TX_EPADDR,
						.Size                   = CDC_TXRX_EPSIZE,
						.Banks                  = 1,
					},
				.DataOUTEndpoint                =
					{
						.Address                = CDC_RX_EPADDR,
						.Size                   = CDC_TXRX_EPSIZE,
						.Banks                  = 1,
					},
				.NotificationEndpoint           =
					{
						.Address                = CDC_NOTIFICATION_EPADDR,
						.Size                   = CDC_NOTIFICATION_EPSIZE,
						.Banks                  = 1,
					},
			},
	};


/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main(void)
{
	SetupHardware();
	GlobalInterruptEnable();


	for (;;)
	{	
		MIDI_Task();
		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	LEDs_Init();
	USB_Init();
}


/* Read and write to USB serail*/
void SerialWrite(char *string)
{	
	CDC_Device_SendData(&VirtualSerial_CDC_Interface, string, strlen(string));
}

int SerialRead(char *data){
	int i = 0;
	if(CDC_Device_BytesReceived (&VirtualSerial_CDC_Interface)){
		while(CDC_Device_BytesReceived (&VirtualSerial_CDC_Interface)) {
			data[i] = CDC_Device_ReceiveByte (&VirtualSerial_CDC_Interface);
			i+=1;
		}
		data[i] = '\0';
		return 1;
	}
	return 0;
}

/** Event handler for the USB_Connect event.**/
void EVENT_USB_Device_Connect(void)
{
	LEDs_SetAllLEDs(LEDS_ALL_LEDS);
}

/** Event handler for the USB_Disconnect event. This indicates that the device is no longer connected, disables the sample update and PWM output timers and stops the USB and MIDI management tasks.
 */
void EVENT_USB_Device_Disconnect(void)
{
	LEDs_SetAllLEDs(LEDS_ALL_LEDS);
}

/** Event handler for the USB_ConfigurationChanged event. This is fired when the host set the current configuration
 *  of the USB device after enumeration - the device endpoints are configured and the MIDI management task started.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	/* Setup MIDI Data Endpoints */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(MIDI_STREAM_IN_EPADDR, EP_TYPE_BULK, MIDI_STREAM_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(MIDI_STREAM_OUT_EPADDR, EP_TYPE_BULK, MIDI_STREAM_EPSIZE, 1);
	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
	

	
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}


/** Task to handle the generation of MIDI note change events in response to presses of the board joystick, and send them
 *  to the host.
 */
void MIDI_Task(void)
{

	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;

	Endpoint_SelectEndpoint(MIDI_STREAM_IN_EPADDR);

	for(int k = 0; k<24; k++) {
		if (Endpoint_IsINReady())
		{
			uint8_t MIDICommand = 0;
			uint8_t MIDIPitch;
			uint8_t Channel = MIDI_CHANNEL(1);

			TouchPad * t= pads[k];
			uint8_t status = readTouch(t);

			if(status > 1 && t->state == 0) 
			{
				MIDICommand = MIDI_COMMAND_NOTE_ON;
				MIDIPitch   = 0x47 - k;
				t->state = 1;
			}
			else if (status == 1 && t->state == 1)
			{
				MIDICommand = MIDI_COMMAND_NOTE_OFF;
				MIDIPitch   = 0x47 - k;
				t->state = 0;
			}
			

			/* Check if a MIDI command is to be sent */
			if (MIDICommand)
			{
				MIDI_EventPacket_t MIDIEvent = (MIDI_EventPacket_t)
					{
						.Event       = MIDI_EVENT(0, MIDICommand),

						.Data1       = MIDICommand | Channel,
						.Data2       = MIDIPitch,
						.Data3       = MIDI_STANDARD_VELOCITY,
					};

				/* Write the MIDI event packet to the endpoint */
				Endpoint_Write_Stream_LE(&MIDIEvent, sizeof(MIDIEvent), NULL);

				/* Send the data in the endpoint to the host */
				Endpoint_ClearIN();
			}
		}

			/* Save previous joystick value for next joystick change detection */
			// PrevJoystickStatus = JoystickStatus;
		}
}
void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t *const CDCInterfaceInfo)
{
	/* You can get changes to the virtual CDC lines in this callback; a common
	   use-case is to use the Data Terminal Ready (DTR) flag to enable and
	   disable CDC communications in your application when set to avoid the
	   application blocking while waiting for a host to become ready and read
	   in the pending data from the USB endpoints.
	*/
	bool HostReady = (CDCInterfaceInfo->State.ControlLineStates.HostToDevice & CDC_CONTROL_LINE_OUT_DTR) != 0;

	(void)HostReady;
}



