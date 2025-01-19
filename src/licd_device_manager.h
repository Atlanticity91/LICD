#ifndef LICD_DEVICE_MANAGER_H
#define LICD_DEVICE_MANAGER_H

struct LicDeviceHeader {

	uint32_t uuid = 0;
	uint32_t flags = 0;

};

/**
 * @class LicDeviceManager
 * @brief Provides I2C master device manager.
 * @author : ALVES Quentin
 * 
 * This class encapsulates methods to simplify communication with I2C devices.
 * All methods are intended for use with the Wire library and in the master code.
 **/
class LicDeviceManager final {

private:
	uint32_t m_retry_count;
	uint32_t m_retry_delay;
	uint32_t m_wait_delay;
	LicDeviceHeader m_devices[ LICD_DEVICE_COUNT ];

public:
	/**
	 * @brief Constructor to initialize the device manager.
	 * 
	 * @param retry_count Number of retry attempts for slave communication (default: 5).
	 * @param retry_delay Delay (in milliseconds) between retries (default: 30).
	 * @param wait_delay Delay (in milliseconds) for transmission wait time (default: 15).
	 **/
	LicDeviceManager( 
		const uint32_t retry_count = 5,
		const uint32_t retry_delay = 30,
		const uint32_t wait_delay = 15
	);

	/**
	 * @brief Destructor for the device manager.
	 **/
	~LicDeviceManager( );

	/**
	 * @brief Polls the device list to detect and register new devices.
	 * 
	 * This method scans for devices waiting for registration and adds them to the device list.
	 **/
	void PollDevice( );

private:
	/**
	 * @brief Checks for devices waiting to be registered.
	 * 
	 * @return true if a device is waiting for registration; false otherwise.
	 **/
	bool DoPollDevice( );

	/**
	 * @brief Registers a device to the device list and assigns it an I2C address.
	 * 
	 * @return The assigned I2C address for the registered device.
	 **/
	uint8_t RegisterDevice( );

};

#endif /* !LICD_DEVICE_MANAGER_H */
