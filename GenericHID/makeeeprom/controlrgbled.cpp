#include "stdafx.h"
#include "controlrgbled.h"

ControlRGBLED::ControlRGBLED(void)
{
}

ControlRGBLED::~ControlRGBLED(void)
{
}

bool ControlRGBLED::Load( const QDomElement &elem, QString *sError )
{
    if ( !GetPort( elem, "PortR", m_nPortR, sError ) )
	return false;
    if ( !GetPort( elem, "PortG", m_nPortG, sError ) )
	return false;
    if ( !GetPort( elem, "PortB", m_nPortB, sError ) )
	return false;
    if ( !XMLUtility::getAttributeString( elem, "Name", m_sName, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "UsagePage", m_nUsagePage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeUShort( elem, "Usage", m_nUsage, 0, 0xFFFF, sError ) )
	return false;
    if ( !XMLUtility::getAttributeBool( elem, "Source", m_bSink, sError ) )
	return false;
    m_bSink = !m_bSink;
    return true;
}

ByteArray ControlRGBLED::GetHIDReportDescriptor( StringTable &table, int &nBits ) const
{
    HIDReportDescriptorBuilder Desc;

    Desc.UsagePage(m_nUsagePage);
    Desc.Usage(m_nUsage);
    Desc.LogicalMinimum(0);
    Desc.LogicalMaximum(7);
    Desc.ReportSize(3);
    Desc.ReportCount(1);
    nBits += 3;
    if (m_sName.length() > 0)
        Desc.StringIndex(table[m_sName]);
    Desc.Output(EDataType::Data, EVarType::Variable, ERelType::Absolute, EWrapType::NoWrap, ELinearType::Linear, EPreferedType::NoPreferred, ENullPositionType::NoNullPosition, EVolatileType::NonVolatile, EBufferType::BitField);

    return Desc;
}

        // returns the micro controller application data
ByteArray ControlRGBLED::GetControlConfig( byte nReportId ) const
{
    struct SRGBLEDControl config;
    memset( &config, 0, sizeof(config) );

    config.hdr.Type = BicolourLED;
    config.hdr.ReportId = nReportId;
    config.hdr.Length = sizeof(config);
    config.PortR = (byte)m_nPortR;
    config.PortG = (byte)m_nPortG;
    config.PortB = (byte)m_nPortB;
    config.Options = (m_bSink ? (1<<RGB_SINK) : 0 );

    return ByteBuffer((byte *)&config, sizeof(config) );
}
