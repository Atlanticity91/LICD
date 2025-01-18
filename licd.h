/**
 * LICD Library Header
 * 
 * This library use <Wire.h> for I2C.
 **/
#ifndef _LICD_H_
#define _LICD_H_

#include <Wire.h>

// Define address for master listening to new device,
// this address is used for the first connection of 
// slave device to exchange with the master is dynamic
// communication address for the rest of the runtime.
#define LICD_LISTENER_ADDRESS 0x01
#define LICD_ADDRESS_SPACE 0x02
#define LICD_DEVICE_COUNT 126

#define LICD_COMMAND_UUID 0x01
#define LICD_COMMAND_ASSIGN 0x02
#define LICD_COMMAND_RETRY 0x03

typedef uint8_t LicDeviceAddress;

typedef void (*LicDeviceReceive)( int byte_count );
typedef void (*LicDeviceRequest)( void );

class WireHelper final {

public:
	/**
	 * write template
	 * @note : Write data for slave.
	 * @template T : Query data type.
	 * @param data : Pointer to data to copy.
	 * @param count : Count of element T store in the pointer, at least 1.
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
	 * read template
	 * @note : Read data for slave.
	 * @template T : Query data type.
	 * @param data : Pointer to data to store.
	 * @param count : Count of element T to store in the pointer, at least 1.
	 * @param timeout : Query timeout delay for data availability by slave.
	 * @return : Return true when readed data size are equal to query size.
	 **/
	template<typename T>
	static bool read( T* data, const uint32_t count, const uint64_t timeout ) {
		if ( count == 0 || wait( timeout ) )
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
	 * wait template
	 * @note : Wait for data availability.
	 * @template T : Query data type.
	 * @param timeout : Query timeout delay for data availability by slave.
	 * @return : Return false when data is available and is not timed out.
	 **/
	template<typename T>
	static bool wait( const uint64_t timeout ) {
		uint64_t start_time = millis( );

		while ( Wire.available( ) < sizeof( T ) ) {
			if ( millis( ) - start_time > timeout ) {
				Serial.print( "[ERR] Wire : Waiting for data as timeout or not enough data as been available." );

				return true;
			}
		}

		return false;
	};

};

struct LicDeviceHeader {

	uint32_t uuid = 0;
	uint32_t flags = 0;

};

class LicDeviceManager final {

private:
	uint32_t m_retry_count;
	uint32_t m_retry_delay;
	uint32_t m_wait_delay;
	LicDeviceHeader m_devices[ LICD_DEVICE_COUNT ];

public:
	/**
	 * Constructor
	 * @param retry_count : Query slave retry count.
	 * @param retry_delay : Query retry waiting delay in milliseconds.
	 * @param wait_delay : Query transmission waiting delay in milliseconds.
	 **/
	LicDeviceManager( 
		const uint32_t retry_count = 5,
		const uint32_t retry_delay = 30,
		const uint32_t wait_delay = 15
	);

	/**
	 * Destructor
	 **/
	~LicDeviceManager( );

	/**
	 * PollDevice method
	 * @note : Poll device list to register new ones.
	 **/
	void PollDevice( );

private:
	/**
	 * DoPollDevice function
	 * @note : Check for device waiting for registration.
	 * @return : Return true when device wait for register.
	 **/
	bool DoPollDevice( );

	/**
	 * RegisterDevice function
	 * @note : Register device to the device list and give is I2C address.
	 * @return : Return 
	 **/
	uint8_t RegisterDevice( );

};

class LicDevice {

protected:
	LicDeviceAddress m_address;
	LicDeviceReceive m_receive;
	LicDeviceRequest m_request;

public:
	LicDevice( 
		LicDeviceReceive receive_handler, 
		LicDeviceRequest request_handler 
	);

	~LicDevice( );

private:
	void Create(
		LicDeviceReceive receive_handler,
		LicDeviceRequest request_handler 
	) {
		Wire.begin( m_address );
		Wire.onReceive( receive_handler );
		Wire.onRequest( request_handler );
	};

private:
	static void ReceiveAddress( int byte_count );

public:
	bool GetIsValid( ) const;

	LicDeviceAddress GetAddress( ) const;

public:
	operator bool ( ) const;

	operator LicDeviceAddress ( ) const;

};

#endif
