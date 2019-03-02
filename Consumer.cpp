/*
  Consumer.cpp

  make by ZZO.
  example from Keyboard.cpp.

  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Consumer.h"

#if defined(_USING_HID)

//================================================================================
//================================================================================
//	Consumer

static const uint8_t _hidReportDescriptor[] PROGMEM = {
	// Consumer Control Descriptor
	0x05, 0x0c,			// USAGE_PAGE (Consumer Devices)
	0x09, 0x01,			// USAGE (Consumer Control)
	0xa1, 0x01,			// COLLECTION (Application)
	0x85, 0x04,			// REPORT_ID (4)
	0x15, 0x00,			// LOGICAL_MINIMUM (0)
	0x25, 0x01,			// LOGICAL_MAXIMUM (1)
	0x75, 0x01,			// REPORT_SIZE (1)
	0x95, 0x10,			// REPORT_COUNT (16)
	0x09, 0xe2,			// USAGE (Mute)					 0
	0x09, 0xe9,			// USAGE (Volume Increment)		 1
	0x09, 0xea,			// USAGE (Volume Decrement)		 2
	0x09, 0xcd,			// USAGE (Play/Pause)			 3
	0x09, 0xb7,			// USAGE (Stop)					 4
	0x09, 0xb5,			// USAGE (Scan Next Track)		 5
	0x09, 0xb6,			// USAGE (Scan Previous Track)	 6
	0x0a, 0x83, 0x01,	// USAGE (Media Player)			 7
	0x0a, 0x92, 0x01,	// USAGE (Calculator)			 8
	0x09, 0xb7,			// USAGE (Stop)					 9	Dummy
	0x09, 0xb7,			// USAGE (Stop)					10	Dummy
	0x09, 0xb7,			// USAGE (Stop)					11	Dummy
	0x09, 0xb7,			// USAGE (Stop)					12	Dummy
	0x09, 0xb7,			// USAGE (Stop)					13	Dummy
	0x09, 0xb7,			// USAGE (Stop)					14	Dummy
	0x09, 0xb7,			// USAGE (Stop)					15  Dummy
	0x81, 0x62,			// INPUT (Data,Var,Abs,NPrf,Null)
	0xc0,

	// System Control Descriptor
	0x05, 0x01, 		// Usage Page (Generic Desktop)
	0x09, 0x80, 		// Usage (System Control)
	0xA1, 0x01, 		// Collection (Application)
	0x85, 0x05, 		// Report ID 0x05 [SYSTEM CTRL]
	0x09, 0x82,         // USAGE (System Sleep)			0
	0x15, 0x00,         // LOGICAL_MINIMUM (0)
	0x25, 0x01,         // LOGICAL_MAXIMUM (1)
	0x75, 0x01,         // REPORT_SIZE (1)
	0x95, 0x03,         // REPORT_COUNT (1)
	0x81, 0x02,         // INPUT (Data,Var,Abs)
	0x95, 0x05,         // REPORT_COUNT (15)
	0x81, 0x03,         // INPUT (Cnst,Var,Abs)
	0xc0,
};

Consumer_::Consumer_(void)
{
	static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
	HID().AppendDescriptor(&node);
}

void Consumer_::begin(void)
{
}

void Consumer_::end(void)
{
}

void Consumer_::process(ConsumerKeys index, bool isPress)
{
	if( isPress){
		press(index);
	}else{
		release(index);
	}
}

void Consumer_::press(ConsumerKeys index)
{
	u8 type = 4 + (index / 128);	// Report ID
	u8 bitidx = (u8)(index & 0x7f);
	u16 bit = (u16)1 << bitidx;

	u8 m[2];
	m[0] = bit & 0x0ff;
	m[1] = bit >> 8;

	HID().SendReport(type, m, 2);
}

void Consumer_::release(ConsumerKeys index)
{
	u8 type = 4 + (index / 128);	// Report ID

	u8 m[2];
	m[0] = 0;
	m[1] = 0;
	HID().SendReport(type, m, 2);
}

Consumer_ Consumer;
#endif
