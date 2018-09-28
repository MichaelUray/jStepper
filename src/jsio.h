/*
 * jsio.h
 *
 *  Created on: Sep 18, 2018
 *      Author: johnny
 */

#ifndef JSIO_H
#define JSIO_H

#include <stdint.h>
#include <avr/pgmspace.h>

//
// determine CPU family
//
#define AVR_ATmega2560_FAMILY (defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__))
#define AVR_ATmega328_FAMILY (defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328p__))

//
// timer defs
//
#define TIMER_SEL_1 1
#define TIMER_SEL_3 3
#define TIMER_SEL_4 4
#define TIMER_SEL_5 5

enum {
	TMR_1_CMPA=0,
	TMR_1_CMPB,
	TMR_1_CMPC,
	TMR_1_OVF,
	TMR_3_CMPA,
	TMR_3_CMPB,
	TMR_3_CMPC,
	TMR_3_OVF,
	TMR_4_CMPA,
	TMR_4_CMPB,
	TMR_4_CMPC,
	TMR_4_OVF,
	TMR_5_CMPA,
	TMR_5_CMPB,
	TMR_5_CMPC,
	TMR_5_OVF,
};


//
// common timer register control bits
//
#define TOIE 0
#define OCIEA 1		// timer compare interrupt enab
#define OCIEB 2
#define OCIEC 3

#define TOV 0
#define OCFA 1		// clear pending interrupts
#define OCFB 2
#define OCFC 3

//
// macros for accessing timer IO regs
//
#define TCCRA IO_REG8(_TCCRA)
#define TCCRB IO_REG8(_TCCRB)
#define TCNT IO_REG16(_TCNT)
#define OCRA IO_REG16(_OCRA)
#define OCRB IO_REG16(_OCRB)
#define OCRC IO_REG16(_OCRC)
#define TIMSK IO_REG16(_TIMSK)
#define TIFR IO_REG16(_TIFR)

//
// timer IO addresses for timer 1, 3, 4, & 5
//
// timer 1
#define _TCCR1A 0x80
#define _TCCR1B	0x81
#define _TCNT1 0x84
#define _OCR1A 0x88
#define _OCR1B 0x8A
#define _OCR1C 0x8C
#define _TIMSK1 0x6F
#define _TIFR1 0x36

// timer 3
#define _TCCR3A 0x90
#define _TCCR3B	0x91
#define _TCNT3 0x94
#define _OCR3A 0x98
#define _OCR3B 0x9A
#define _OCR3C 0x9C
#define _TIMSK3 0x71
#define _TIFR3 0x38

// timer 4
#define _TCCR4A 0xA0
#define _TCCR4B	0xA1
#define _TCNT4 0xA4
#define _OCR4A 0xA8
#define _OCR4B 0xAA
#define _OCR4C 0xAC
#define _TIMSK4 0x72
#define _TIFR4 0x39

// timer 5
#define _TCCR5A 0x120
#define _TCCR5B	0x121
#define _TCNT5 0x124
#define _OCR5A 0x128
#define _OCR5B 0x12A
#define _OCR5C 0x12C
#define _TIMSK5 0x73
#define _TIFR5 0x3A

//
// Macros for accessing I/O regs
//
#define IO_REG8(_reg) (*(volatile uint8_t *)(_reg))
#define IO_REG16(_reg) (*(volatile uint16_t *)(_reg))

//
// fast I/O routines that replace pinMode, digitalWrite, etc.
//
#define _PIN_OUTPUT(pin) (IO_REG16(pgm_read_word(ddr_lu + pin)) |= pgm_read_byte(bit_lu + pin))
#define _PIN_INPUT(pin) (IO_REG16(pgm_read_word(ddr_lu + pin)) &= ~(pgm_read_byte(bit_lu + pin)))
#define PIN_OUTPUT(pin) _PIN_OUTPUT(pin)
#define PIN_INPUT(pin) _PIN_INPUT(pin)

#define _PIN_READ(pin) (bool)(IO_REG16(pgm_read_word(pin_lu + pin)) & pgm_read_byte(bit_lu + pin))
#define _PIN_WRITE(pin, V) do{ if(V){ IO_REG16(pgm_read_word(port_lu + pin)) |= pgm_read_byte(bit_lu + pin); } \
		else { IO_REG16(pgm_read_word(port_lu + pin)) &= ~(pgm_read_byte(bit_lu + pin)); } }while(0)

#define PIN_READ(pin) _PIN_READ(pin)
#define PIN_WRITE(pin, V) _PIN_WRITE(pin, V)

#define PIN_INPUT_PULLUP(pin) do{ _PIN_INPUT(pin); _PIN_WRITE(pin, 0x01); }while(0)
#define PIN_OUT_WRITE(pin, V) do{ _PIN_OUTPUT(pin); _PIN_WRITE(pin, V); }while(0)

//
// misc I/O macros
//
#define DELAY_1_NOP __asm__("nop\n\t")  // on 16 Mhz cpu - 62.5ns

//
// macros for bit manipulation
//
#define SETb(x,y) x |= (1 << y)
#define CLRb(x,y) x &= ~(1<< y)
#define CMPb(x,y) x & (1 << y)


//===================================================================
// Arduino to real world converter
// Arduino wanted to make programming simpler but the arcane
// alphabet soup IO mapping doesn't work with dynamic IO assignment.
//
// The following lookup tables provide the physical I/O
// register addresses for DDR, PORT, PIN, and a bit mask.
// The above macros (PIN_OUTPUT, PIN_WRITE, etc.) index these
// tables with the Arduino style pin number to perfom fast and
// dynamic IO ops.
//
const uint16_t ddr_lu[87] PROGMEM {
	0x2D, 0x2D, 0x2D, 0x2D, 0x33, 0x2D, 0x101, 0x101,
	0x101, 0x101, 0x24, 0x24, 0x24, 0x24, 0x104, 0x104,
	0x101, 0x101, 0x2A, 0x2A, 0x2A, 0x2A, 0x21, 0x21,
	0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x27, 0x27,
	0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x2A, 0x33,
	0x33, 0x33, 0x10A, 0x10A, 0x10A, 0x10A, 0x10A, 0x10A,
	0x10A, 0x10A, 0x24, 0x24, 0x24, 0x24, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x107, 0x107,
	0x107, 0x107, 0x107, 0x107, 0x107, 0x107, 0x33, 0x33,
	0x104, 0x104, 0x104, 0x104, 0x104, 0x104, 0x2D, 0x2D,
	0x2D, 0x2A, 0x2A, 0x2A, 0x101, 0x101, 0xFF
};

const uint16_t port_lu[87] PROGMEM {
	0x2E, 0x2E, 0x2E, 0x2E, 0x34, 0x2E, 0x102, 0x102,
	0x102, 0x102, 0x25, 0x25, 0x25, 0x25, 0x105, 0x105,
	0x102, 0x102, 0x2B, 0x2B, 0x2B, 0x2B, 0x22, 0x22,
	0x22, 0x22,	0x22, 0x22, 0x22, 0x22, 0x28, 0x28,
	0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x2B, 0x34,
	0x34, 0x34, 0x10B, 0x10B, 0x10B, 0x10B, 0x10B, 0x10B,
	0x10B, 0x10B, 0x25, 0x25, 0x25, 0x25, 0x31, 0x31,
	0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x108, 0x108,
	0x108, 0x108, 0x108, 0x108, 0x108, 0x108, 0x34, 0x34,
	0x105, 0x105, 0x105, 0x105, 0x105, 0x105, 0x2E, 0x2E,
	0x2E, 0x2B, 0x2B, 0x2B, 0x102, 0x102, 0xFF
};

const uint16_t pin_lu[87] PROGMEM {
	0x2C, 0x2C, 0x2C, 0x2C, 0x32, 0x2C, 0x100, 0x100,
	0x100, 0x100, 0x23, 0x23, 0x23, 0x23, 0x103, 0x103,
	0x100, 0x100, 0x29, 0x29, 0x29, 0x29, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x26, 0x26,
	0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x29, 0x32,
	0x32, 0x32, 0x109, 0x109, 0x109, 0x109, 0x109, 0x109,
	0x109, 0x109, 0x23, 0x23, 0x23, 0x23, 0x2F, 0x2F,
	0x2F, 0x2F, 0x2F, 0x2F, 0x2F, 0x2F, 0x106, 0x106,
	0x106, 0x106, 0x106, 0x106, 0x106, 0x106, 0x32, 0x32,
	0x103, 0x103, 0x103, 0x103, 0x103, 0x103, 0x2C, 0x2C,
	0x2C, 0x29, 0x29, 0x29, 0x100, 0x100, 0xFF
};

const uint8_t bit_lu[87] PROGMEM {
	0x01, 0x02,	0x10, 0x20, 0x20, 0x08, 0x08, 0x10,
	0x20, 0x40, 0x10, 0x20, 0x40, 0x80, 0x02, 0x01,
	0x01, 0x02, 0x08, 0x04, 0x02, 0x01, 0x01, 0x02,
	0x04, 0x08, 0x10, 0x20, 0x40, 0x80,	0x80, 0x40,
	0x20, 0x10,	0x08, 0x04,	0x02, 0x01, 0x80, 0x04,
	0x02, 0x01, 0x80, 0x40,	0x20, 0x10, 0x08, 0x04,
	0x02, 0x01, 0x08, 0x04, 0x02, 0x01, 0x01, 0x02,
	0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x01, 0x02,
	0x04, 0x08,	0x10, 0x20, 0x40, 0x80,	0x10, 0x08,
	0x04, 0x08, 0x80, 0x10,	0x20, 0x40,	0x04, 0x40,
	0x80, 0x10,	0x20, 0x40, 0x04, 0x80, 0x00
};



#endif /* JSIO_H */
