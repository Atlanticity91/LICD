/**
 * @file licd_wire_helper.h
 * @brief Provides utility functions for I2C communication using the Wire library.
 *
 * This header defines the `WireHelper` class, which contains static methods to
 * simplify I2C communication tasks such as reading, writing, and waiting for data
 * availability. The class is designed to work with the Arduino `Wire` library, enabling
 * seamless interaction with I2C slave devices.
 *
 * ## Features
 * - Write data to an I2C slave device with type safety.
 * - Read data from an I2C slave device with customizable timeout handling.
 * - Wait for data availability with timeout monitoring.
 *
 * ## Usage Example
 * ```
 * #include "licd_wire_helper.h"
 *
 * // Write example
 * int data_to_send = 42;
 * WireHelper::write(&data_to_send, 1);
 *
 * // Read example
 * int received_data = 0;
 * if ( WireHelper::read( &received_data, 1, 100 ) ) {
 *     Serial.println( received_data );
 * }
 *
 * // Wait example
 * if ( WireHelper::wait<int>( 50 ) ) {
 *     Serial.println( "Data available" );
 * }
 * ```
 *
 * ## Notes
 * - Ensure the Wire library is properly initialized before using the helper functions.
 * - This header is designed for use in Arduino-compatible environments.
 *
 * @author ALVES Quentin
 * @date 18/01/2025
 * @version 1.0
 **/

#ifndef _LICD_WIRE_HELPER_H_
#define _LICD_WIRE_HELPER_H_

#include <Wire.h>

/**
 * @class WireHelper
 * @brief Provides static utility functions for I2C read, write, and wait operations.
 * @author : ALVES Quentin
 * 
 * This class encapsulates helper methods to simplify communication with I2C devices.
 * All methods are static and intended for use with the Wire library.
 **/
class WireHelper final {

public:
	/**
	 * @brief Writes data to an I2C slave device.
	 * 
	 * @tparam T The type of data to be written.
	 * @param data Pointer to the data to write.
	 * @param count Number of elements of type T to write (must be >= 1).
	 **/
	template<typename T>
	static void write( const T* data, const uint32_t count ) {
		if ( count == 0 )
			return;

		const uint8_t* byte_ptr = reinterpret_cast<const uint8_t*>( data );
		const size_t data_size = sizeof( T ) * count;

		for ( size_t data_offset = 0; data_offset < data_size; data_offset++ )
			Wire.write( byte_ptr[ data_offset ] );
	};

	/**
	 * @brief Reads data from an I2C slave device.
	 * 
	 * @tparam T : The type of data to be read.
	 * @param data : Pointer to the memory where the read data will be stored.
	 * @param count : Number of elements of type T to read (must be >= 1).
	 * @param timeout : Maximum time (in milliseconds) to wait for the data.
	 * @return true if the read operation was successful; false otherwise.
	 **/
	template<typename T>
	static bool read( T* data, const uint32_t count, const uint64_t timeout ) {
		if ( count == 0 || !wait( timeout ) )
			return false;

		const size_t data_size = sizeof( T ) * count;
		uint8_t* byte_ptr = reinterpret_cast<uint8_t*>( data );
		size_t data_offset = 0;

		while ( Wire.available( ) && data_offset < data_size )
			byte_ptr[ data_offset++ ] = (uint8_t)Wire.read( );

		return ( data_offset == data_size );
	};

public:
	/**
	 * @brief Waits for data availability from an I2C slave device.
	 * 
	 * @tparam T : The type of data to wait for.
	 * @param timeout : Maximum time (in milliseconds) to wait for data availability.
	 * @return true if data becomes available within the timeout period; false otherwise.
	 **/
	template<typename T>
	static bool wait( const uint64_t timeout ) {
		uint64_t start_time = millis( );

		while ( Wire.available( ) < sizeof( T ) ) {
			if ( millis( ) - start_time > timeout ) {
				Serial.print( "[ERR] Wire : Waiting for data as timeout or not enough data as been available." );

				return false;
			}
		}

		return true;
	};

};

#endif /* !_LICD_WIRE_HELPER_H_ */
