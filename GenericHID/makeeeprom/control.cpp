#include "stdafx.h"
#include "control.h"
#include "controlanalogencoder.h"
#include "controlbicolourled.h"
#include "controlcounter.h"
#include "controldigitalencoder.h"
#include "controldirectionalswitch.h"
#include "controlkeymatrix.h"
#include "controllcd.h"
#include "controlled.h"
#include "controlpotentiometer.h"
#include "controlpwm.h"
#include "controlrgbled.h"
#include "controlrotaryswitch.h"
#include "controlswitch.h"
#include "controltricolourled.h"

Control::Control(void)
{
}

Control::~Control(void)
{
}


Control *Control::MakeControl( const QDomElement &elem, QString *sError )
{
    Control *pControl = NULL;
    if ( elem.nodeName().compare( "DirectionalSwitch", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlDirectionalSwitch();
    else if ( elem.nodeName().compare( "AnalogIn", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlPotentiometer();
    else if ( elem.nodeName().compare( "Switch", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlSwitch();
    else if ( elem.nodeName().compare( "TricolourLED", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlTriColourLED();
    else if ( elem.nodeName().compare( "DigitalEncoder", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlDigitalEncoder();
    else if ( elem.nodeName().compare( "KeyMatrix", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlKeyMatrix();
    else if ( elem.nodeName().compare( "RotarySwitch", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlRotarySwitch();
    else if ( elem.nodeName().compare( "LCD", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlLCD();
    else if ( elem.nodeName().compare( "LED", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlLED();
    else if ( elem.nodeName().compare( "BicolourLED", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlBiColourLED();
    else if ( elem.nodeName().compare( "RGBLED", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlRGBLED();
    else if ( elem.nodeName().compare( "PWM", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlPWM();
    else if ( elem.nodeName().compare( "Counter", Qt::CaseInsensitive ) == 0 )
	pControl = new ControlCounter();
    else
    {
	if ( sError != NULL )
	    *sError = QString( "Unknown node type <%1>").arg(elem.nodeName());
    }

    if ( pControl != NULL )
    {
	if ( !pControl->Load( elem, sError ) )
	{
	    delete pControl;
	    pControl = NULL;
	}
    }

    return pControl;
}

