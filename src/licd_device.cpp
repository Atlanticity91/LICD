/**
 * ====================
 * LicDevice
 * ====================
 */

// PUBLIC METHODS

/**
 * @brief Constructs a `LicDevice` with receive and request handlers.
 *
 * @param receive_handler Function pointer for handling received data.
 * @param request_handler Function pointer for handling requests from the master.
 **/
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

/**
 * @brief Destructor for `LicDevice`.
 **/
LicDevice::~LicDevice( ) { }

// PRIVATE METHODS

/**
 * @brief Initializes the device and sets up handlers.
 *
 * @param receive_handler Function pointer for receive events.
 * @param request_handler Function pointer for request events.
 **/
void LicDevice::Create(
	LicDeviceReceive receive_handler,
	LicDeviceRequest request_handler 
) {
	Wire.begin( m_address );
	Wire.onReceive( receive_handler );
	Wire.onRequest( request_handler );
}

// PRIVATE STATIC METHODS

/**
 * @brief Handles address assignment and communication commands.
 *
 * @param byte_count Number of bytes received in the communication.
 **/
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

// PUBLIC GETTERS

/**
 * @brief Checks if the device has a valid I2C address.
 *
 * @return true if the device has a valid address; false otherwise.
 **/
bool LicDevice::GetIsValid( ) const {
	return ( m_address > LICD_LISTENER_ADDRESS );
}

/**
 * @brief Retrieves the device's I2C address.
 *
 * @return The current I2C address of the device.
 **/
LicDeviceAddress LicDevice::GetAddress( ) const { 
	return m_address;
}

LicDeviceReceive LicDevice::GetReceive( ) const {
	return m_receive;
}

LicDeviceRequest LicDevice::GetRequest( ) const {
	return m_request;
}

// OPERATORS

/**
 * @brief Checks the validity of the device.
 *
 * @return true if the device is valid; false otherwise.
 **/
LicDevice::operator bool ( ) const {
	return GetIsValid( );
}

/**
 * @brief Retrieves the device's I2C address.
 *
 * @return The device's I2C address.
 **/
LicDevice::operator LicDeviceAddress ( ) const {
	return GetAddress( );
}
