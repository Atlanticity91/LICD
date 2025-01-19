/**
 * @file LICD_Commands.h
 * @brief Defines command constants for LICD (Lightweight I2C Communication Design) framework.
 *
 * This header file contains command codes used within the LICD framework for I2C communication. 
 * These commands facilitate operations such as requesting UUIDs, assigning dynamic addresses 
 * to slave devices, and handling retries during communication failures.
 *
 * ## Commands
 * - `LICD_COMMAND_UUID`: Command to request the UUID of a slave device.
 * - `LICD_COMMAND_ASSIGN`: Command to assign a dynamic address to a slave device.
 * - `LICD_COMMAND_RETRY`: Command to instruct a slave device to retry an operation.
 *
 * ## Usage Notes
 * - These command codes are intended for use with the LICD protocol and should be consistent 
 *   across all master and slave devices to ensure proper communication.
 * - This file is designed for use in Arduino-compatible environments.
 *
 * @author ALVES Quentin
 * @date 18/01/2025
 * @version 1.0
 **/

#ifndef LICD_COMMANDS_H_
#define LICD_COMMANDS_H_

/**
 * @brief Command to request the UUID of a slave device.
 * 
 * This command allows the master to query the unique identifier (UUID)
 * of a slave device during the initialization or discovery process.
 **/
#define LICD_COMMAND_UUID 0x01

/**
 * @brief Command to assign a dynamic address to a slave device.
 * 
 * This command is used by the master to provide a slave device with
 * a unique dynamic address for communication during runtime.
 **/
#define LICD_COMMAND_ASSIGN 0x02

/**
 * @brief Command to instruct a slave device to retry an operation.
 * 
 * This command is sent by the master to request the slave device to
 * retry a previously failed or incomplete operation.
 **/
#define LICD_COMMAND_RETRY 0x03

#endif /* !LICD_COMMANDS_H_ */
