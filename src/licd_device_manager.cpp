/**
 * @file LicDeviceManager.cpp
 * @brief Implementation of LICD device management and slave device functionality.
 *
 * This file implements the `LicDeviceManager` and `LicDevice` classes, which handle
 * I2C device management and operations within the LICD framework. It includes methods
 * for polling, registering devices, and handling communication commands.
 *
 * ## Classes
 * - `LicDeviceManager`: Manages I2C slave devices by polling and registering them.
 * - `LicDevice`: Represents an individual I2C slave device and handles its operations.
 *
 * ## Dependencies
 * - Requires `licd.h` for shared constants, `LICD_Globals.h`, and `Wire.h`.
 *
 * @author ALVES Quentin
 * @date 18/01/2025
 * @version 1.0
 **/

#include "licd.h"

/**
 * ====================
 * LicDeviceManager
 * ====================
 */

// PUBLIC METHODS

/**
 * @brief Constructs the `LicDeviceManager` with retry and delay configurations.
 *
 * @param retry_count Number of retry attempts for communication (default: 5).
 * @param retry_delay Delay (in milliseconds) between retries (default: 30).
 * @param wait_delay Delay (in milliseconds) for transmission wait time (default: 15).
 **/
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

/**
 * @brief Destructor for `LicDeviceManager`.
 **/
LicDeviceManager::~LicDeviceManager( ) { }

/**
 * @brief Polls for new devices and registers them.
 *
 * This method checks if any devices are waiting for registration and assigns them a
 * dynamic I2C address.
 **/
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

// PRIVATE METHODS

/**
 * @brief Checks for devices waiting to be registered.
 *
 * Sends a UUID query command to detect devices ready for registration.
 *
 * @return true if a device is waiting for registration, false otherwise.
 **/
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

/**
 * @brief Registers a new device and assigns it an I2C address.
 *
 * Reads the device header and assigns a unique address from the available address space.
 *
 * @return The assigned I2C address for the new device.
 **/
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
