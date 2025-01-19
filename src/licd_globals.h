/**
 * @file LICD_Globals.h
 * @brief Defines global constants for LICD (Lightweight I2C Communication Design) framework.
 *
 * This header file contains global constants used for I2C communication within the LICD framework. 
 * These constants include the default listener address, address space allocation, and the maximum 
 * number of slave devices supported by the master. 
 *
 * ## Constants
 * - `LICD_LISTENER_ADDRESS`: The address used by the master to listen for new slave devices during 
 *   their initial connection.
 * - `LICD_ADDRESS_SPACE`: The starting address in the I2C address space for slave devices.
 * - `LICD_DEVICE_COUNT`: The maximum number of slave devices that can be managed by the master.
 *
 * ## Usage Notes
 * - Ensure that these constants are consistent across all components of the LICD system to avoid
 *   communication conflicts.
 * - This file is intended for use in Arduino-compatible environments where the Wire library is used.
 *
 * @author ALVES Quentin
 * @date 18/01/2025
 * @version 1.0
 **/

#ifndef LICD_GLOBALS_H_
#define LICD_GLOBALS_H_

/**
 * @brief Address used by the master to listen for new slave devices.
 * 
 * This address is reserved for the initial connection phase when a slave device
 * communicates with the master to request a dynamic communication address.
 **/
#define LICD_LISTENER_ADDRESS 0x01

/**
 * @brief Starting address for the slave device address space.
 * 
 * This defines the first available address that can be assigned to slave devices
 * after their initial connection phase.
 **/
#define LICD_ADDRESS_SPACE 0x02

/**
 * @brief Maximum number of slave devices supported by the master.
 * 
 * Defines the maximum number of I2C slave devices that can be managed simultaneously.
 * The value is derived from the I2C protocol's 7-bit address space, minus reserved addresses.
 **/
#define LICD_DEVICE_COUNT 126

#endif /* !LICD_GLOBALS_H_ */
