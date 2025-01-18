/**
 * LICD Library SOURCE
 **/

#include "licd.h"

/**
 * === LicDeviceManager ===
 **/
// PUBLIC
LicDeviceManager::LicDeviceManager( 
	const uint32_t retry_count = 5,
	const uint32_t retry_delay = 30,
	const uint32_t wait_delay = 15
) 
	: m_retry_count{ retry_count },
	m_retry_delay{ retry_delay },
	m_wait_delay{ wait_delay },
	m_devices{ }
{
	Wire.begin( );
}

LicDeviceManager::~LicDeviceManager( ) { }

void LicDeviceManager::PollDevice( ) {
	if ( !DoPollDevice( ) )
		return;

	uint8_t new_address = RegisterDevice( );

    Wire.beginTransmission( LICD_LISTENER_ADDRESS );
    	
    if ( new_address > LICD_LISTENER_ADDRESS ) {
    	Wire.write( LICD_COMMAND_ASSIGN );
    	Wire.write( new_address );
    } else 
    	Wire.write( LICD_COMMAND_RETRY );

    Wire.endTransmission( );
}

// PRIVATE
bool LicDeviceManager::DoPollDevice( ) {
	uint32_t error = 5;

	for ( uint32_t retry_count = 0; error > 0 && retry_count < m_retry_count; retry_count++ ) {
		Wire.beginTransmission( LICD_LISTENER_ADDRESS );
		Wire.write( LICD_COMMAND_UUID );

		error = Wire.endTransmission( );

		delay( m_retry_delay );
	}

	switch ( error ) {
		case 1 : Serial.print( "[ERR] Wire : Data too long to fit in transmit buffer." ); break;
		case 2 : Serial.print( "[ERR] Wire : Received NACK on transmit of address." ); break;
		case 3 : Serial.print( "[ERR] Wire : Received NACK on transmit of data." ); break;
		case 4 : Serial.print( "[ERR] Wire : Undefined error behavior." ); break;

		default : break;
	}
	
	return ( error == 0 );
}

uint8_t LicDeviceManager::RegisterDevice( ) {
	LicDeviceHeader header = LicDeviceHeader( );
	uint8_t new_address = LICD_LISTENER_ADDRESS;

	delay( m_wait_delay );

	Wire.requestFrom( LICD_LISTENER_ADDRESS, sizeof( LicDeviceHeader ) );

	delay( m_wait_delay );

	if ( WireHelper.read( &header, 1, 150 ) ) {
		uint8_t address_offset = 0;

		while ( ( new_address == LICD_LISTENER_ADDRESS ) && ( address_offset < LICD_DEVICE_COUNT ) ) {
			if ( m_devices[ address_offset ].uuid > 0 )
				continue;

			new_address = ( LICD_ADDRESS_SPACE + address_offset );

			memcpy( &m_devices[ address_offset ], &header, sizeof( LicDeviceHeader ) );
		}
	} else
		Serial.print( "[ERR] Wire : Data too short or too long to fit the transmit buffer.");

	return new_address;
}

/**
 * === LicDevice ===
 **/
// PUBLIC
LicDevice::LicDevice( 
	LicDeviceReceive receive_handler, 
	LicDeviceRequest request_handler 
)
	: m_address{ LICD_LISTENER_ADDRESS },
	m_receive{ receive_handler },
	m_request{ request_handler }
{
	Create( ReceiveAddress, nullptr );
}

LicDevice::~LicDevice( ) { }

// PRIVATE
void LicDevice::Create(
	LicDeviceReceive receive_handler,
	LicDeviceRequest request_handler 
) {
	Wire.begin( m_address );
	Wire.onReceive( receive_handler );
	Wire.onRequest( request_handler );
}

// PRIVATE STATIC
void LicDevice::ReceiveAddress( int byte_count ) {
	if ( Wire.available( ) ) {
		uint8_t command = Wire.read( );

		if ( command == LICD_COMMAND_UUID ) {
			Wire.write( 0x00 );
		} else if ( command == LICD_COMMAND_ASSIGN ) {
			m_address = Wire.read( );
				
			Create( m_receive, m_request );
		} else if ( command == LICD_COMMAND_RETRY ) {
		}
	}

	delay( 30 );
}

// PUBLIC GET
bool LicDevice::GetIsValid( ) const {
	return ( m_address > LICD_LISTENER_ADDRESS );
}

LicDeviceAddress LicDevice::GetAddress( ) const { 
	return m_address;
}

// OPERATOR
LicDevice::operator bool ( ) const {
	return GetIsValid( );
}

LicDevice::operator LicDeviceAddress ( ) const {
	return GetAddress( );
}
