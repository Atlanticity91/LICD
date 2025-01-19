#ifndef LICD_DEVICE_H
#define LICD_DEVICE_H

/**
 * LicDeviceAddress typedef
 * @note : Defined address type for the library.
 **/
typedef uint8_t LicDeviceAddress;

/**
 * 
 **/
typedef void (*LicDeviceReceive)( int byte_count );
typedef void (*LicDeviceRequest)( void );

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

	void Reset( );

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

	LicDeviceReceive GetReceive( ) const;

	LicDeviceRequest GetRequest( ) const;

public:
	operator bool ( ) const;

	operator LicDeviceAddress ( ) const;

};

#endif /* !LICD_DEVICE_H */
