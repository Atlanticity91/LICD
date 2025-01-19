#include <licd.h>

LicDevice device;

void receive( int byte_count ) {
}

void request( ) {
}

void setup( ) {
	device = LicDevice( &receive, &request );
}

void loop( ) {
	// NORMAL ARDUINO LOOP
}
