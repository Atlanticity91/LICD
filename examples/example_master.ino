#include <licd.h>

LicDeviceManager device_manager;

void setup( ) {
	device_manager = LicDeviceManager( );
}

void loop( ) {
	device_manager.PollDevice( );

	// REST OF YOUR CODE
}
