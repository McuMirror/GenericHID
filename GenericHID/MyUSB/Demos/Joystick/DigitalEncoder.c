#include "Common.h"
#include "Serial.h"
#include "Controls.h"
#include "DigitalEncoder.h"
#include "Serial.h"
#include <MyUSB/Common/ISRMacro.h>         // PROGMEM tags readable by the ButtLoad project

#include <string.h>

#define MAX_ENCODERS		8				// 16 external interrupts, therefore 8 encoders

static bool bInitialised = false;			// first time through we need to initialise everything
static byte nEncoders;
static uint16_t nLastReading;
static bool bFirst;
static struct
{
    byte nBitA;
    byte nBitB;
    int8_t nCounter;
} Encoders[MAX_ENCODERS];

#define FromAB(a,b)		(((a)<<3) | ((b)<<2))
#define ToAB(a,b)		(((a)<<1) | (b))
static int8_t EncoderMap[16];					// determines whether to increment or decrement an encoder based on the mapping

// Encoders are processed via the external interrupts.  Therefore they can only be connected to pins...
// INT0,1,2,3 INT4,5,6,7 PCINT0,1,2,3,4,5,6,7
//  PD0,1,2,3  PE4,5,6,7    PB0,1,2,3,4,5,6,7

static void SetInterruptPort( byte nPortId, byte nPinId, bool bPullup )
{
    switch ( nPortId )
    {
	case PortB:	// port b has the pin change interrupts
	    DDRB &= ~(1<<nPinId);
	    if ( bPullup)
		PORTB |= 1<<nPinId;
	    PCICR |= _BV(PCIE0);
	    PCMSK0 |= 1<<nPinId;
	    break;

	case PortD:	// port d has external int0-3 (which line up nicely on pins PD0-3)
	    DDRD &= ~(1<<nPinId);
	    if ( bPullup)
		PORTD |= 1<<nPinId;
	    EICRA |= (_BV(ISC00)) << (2 * nPinId);		// interrupt on any edge
	    EIMSK |= 1<<nPinId;
	    break;

	case PortE:	// port e has external int4-7 (which line up nicely on pins PE4-7)
	    DDRE &= ~(1<<nPinId);
	    if ( bPullup)
		PORTE |= 1<<nPinId;
	    EICRB |= (_BV(ISC00)) << (2 * (nPinId-4));		// interrupt on any edge
	    EIMSK |= 1<<nPinId;
	    break;
    }
}


void InitDigitalEncoder( struct SDigitalEncoderControl *pData )
{
    if ( !bInitialised )
    {
	nEncoders = 0;
	bFirst = true;

	memset( EncoderMap, 0, sizeof(EncoderMap) );

	EncoderMap[FromAB(1,0) + ToAB(1,1)] = +1;
	EncoderMap[FromAB(1,0) + ToAB(0,0)] = -1;
	EncoderMap[FromAB(1,1) + ToAB(0,1)] = +1;
	EncoderMap[FromAB(1,1) + ToAB(1,0)] = -1;
	EncoderMap[FromAB(0,1) + ToAB(0,0)] = +1;
	EncoderMap[FromAB(0,1) + ToAB(1,1)] = -1;
	EncoderMap[FromAB(0,0) + ToAB(1,0)] = +1;
	EncoderMap[FromAB(0,0) + ToAB(0,1)] = -1;
    }

    // Set structure entry

    // set interrupt for a and b
    byte nPortA = GET_PORT_ID(pData->PortA);
    byte nPinA = GET_PORT_PIN(pData->PortA);
    SetInterruptPort( nPortA, nPinA, false );

    byte nPortB = GET_PORT_ID(pData->PortB);
    byte nPinB = GET_PORT_PIN(pData->PortB);
    SetInterruptPort( nPortB, nPinB, false );

    Encoders[nEncoders].nBitA = nPinA + (nPortA == PortB ? 0 : 8);
    Encoders[nEncoders].nBitB = nPinB + (nPortA == PortB ? 0 : 8);
    Encoders[nEncoders].nCounter = 0;
    nEncoders++;

    bInitialised = true;
}


void ReadDigitalEncoder( struct SDigitalEncoderControl *pData, byte **ReportBuffer, byte *nBit )
{
    byte nPortA = GET_PORT_ID(pData->PortA);
    byte nPinA = GET_PORT_PIN(pData->PortA);
    byte nBitA = nPinA + (nPortA == PortB ? 0 : 8);

    for ( byte i = 0; i < nEncoders; i++ )
	if ( nBitA == Encoders[i].nBitA )
	{
	    int8_t *ptr = &(Encoders[i].nCounter);
	    cli();
	    int8_t nCounter = *ptr;
	    //*ptr = 0;
	    sei();

	    PackData16( ReportBuffer, nBit, nCounter, 8 );
	    break;
	}
}


// common handler for all external interrupts
ISR(INT0_vect)
{
    uint16_t n;
    *((byte *)&n) = PINB;
    *(((byte *)&n)+1) = (PIND & 0xF) | (PINE & 0xF0);

    if ( bFirst )
    {
	bFirst = false;
	nLastReading = n;
    }
    else if ( nLastReading != n )
    {
	uint16_t nDiff = nLastReading ^ n;
	// check for changes and update counters
	for ( byte i = 0; i < nEncoders; i++ )
	{
	    if ( ( nDiff & (1<<Encoders[i].nBitA) ) ||
		( nDiff & (1<<Encoders[i].nBitB) ) )
	    {
		// a change.
		byte nIndex = 0;		// ->  0 0 0 0 LastA LastB A B
		if ( nLastReading & (1<<Encoders[i].nBitA) ) 
		    nIndex |= 0x08;
		if ( nLastReading & (1<<Encoders[i].nBitB) ) 
		    nIndex |= 0x04;			     
		if ( n & (1<<Encoders[i].nBitA) ) 
		    nIndex |= 0x02;
		if ( n & (1<<Encoders[i].nBitB) ) 
		    nIndex |= 0x01;

		Encoders[i].nCounter += EncoderMap[nIndex];
	    }
	}

	nLastReading = n;
    }
}

ISR(INT1_vect,ISR_ALIASOF(INT0_vect));
ISR(INT2_vect,ISR_ALIASOF(INT0_vect));
ISR(INT3_vect,ISR_ALIASOF(INT0_vect));
ISR(INT4_vect,ISR_ALIASOF(INT0_vect));
ISR(INT5_vect,ISR_ALIASOF(INT0_vect));
ISR(INT6_vect,ISR_ALIASOF(INT0_vect));
ISR(INT7_vect,ISR_ALIASOF(INT0_vect));
ISR(PCINT0_vect,ISR_ALIASOF(INT0_vect));
