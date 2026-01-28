// SPDX-License-Identifier: Apache-2.0

/**
 * @file network.h
 * @brief Network configuration and system identity utilities.
 *
 * This module provides functions for configuring network interfaces, managing
 * IP addresses, and setting system identity parameters such as hostname and
 * username.
 *
 * @note Most setter functions require elevated privileges (root/administrator).
 * @note Network interface names are platform-specific (e.g., "eth0", "enp3s0").
 *
 * @warning Incorrect network configuration can result in loss of connectivity.
 *
 * @example
 * @code
 * // Get current network configuration
 * char *ip = network_getIpv4Address("eth0");
 * char *netmask = network_getIpv4Netmask("eth0");
 * char *gateway = network_getIpv4Gateway();
 *
 * printf("IP: %s, Netmask: %s, Gateway: %s\n", ip, netmask, gateway);
 *
 * free(ip);
 * free(netmask);
 * free(gateway);
 *
 * // Configure a static IP (requires root)
 * network_setIpv4Network("192.168.1.100", "255.255.255.0", "192.168.1.1");
 * @endcode
 */

#ifndef INCLUDE_UTILITY_C_NETWORK_H_
#define INCLUDE_UTILITY_C_NETWORK_H_

#ifdef S_HAVE_CONFIG_H
  #include "utility-c/config.h"
#endif

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Sets the IPv4 address of a network interface.
 *
 * Configures the specified network interface with the given IPv4 address.
 *
 * @param[in] ip        The IPv4 address in dotted-decimal notation
 *                      (e.g., "192.168.1.100").
 * @param[in] interface The name of the network interface (e.g., "eth0").
 *
 * @return 0 on success.
 * @return -1 on failure (e.g., invalid address, interface not found,
 *         insufficient privileges).
 *
 * @pre @p ip must be a valid IPv4 address string.
 * @pre @p interface must be a valid network interface name.
 * @pre Caller typically needs root/administrator privileges.
 *
 * @see network_getIpv4Address, network_setIpv4Network
 */
int network_setIpv4Address(const char *ip, const char *interface);

/**
 * @brief Gets the IPv4 address of a network interface.
 *
 * Retrieves the primary IPv4 address assigned to the specified network
 * interface.
 *
 * @param[in] interface The name of the network interface (e.g., "eth0").
 *
 * @return A newly allocated string containing the IPv4 address in
 *         dotted-decimal notation.
 * @return NULL if the interface is not found or has no IPv4 address.
 *
 * @note The caller is responsible for freeing the returned string using free().
 *
 * @see network_setIpv4Address, network_getIpv6Address
 */
char *network_getIpv4Address(const char *interface);

/**
 * @brief Gets the IPv6 address of a network interface.
 *
 * Retrieves the primary IPv6 address assigned to the specified network
 * interface.
 *
 * @param[in] interface The name of the network interface (e.g., "eth0").
 *
 * @return A newly allocated string containing the IPv6 address.
 * @return NULL if the interface is not found or has no IPv6 address.
 *
 * @note The caller is responsible for freeing the returned string using free().
 *
 * @see network_getIpv4Address
 */
char *network_getIpv6Address(const char *interface);

/**
 * @brief Sets the IPv4 netmask of a network interface.
 *
 * Configures the subnet mask for the specified network interface.
 *
 * @param[in] netmask   The IPv4 netmask in dotted-decimal notation
 *                      (e.g., "255.255.255.0").
 * @param[in] interface The name of the network interface (e.g., "eth0").
 *
 * @return 0 on success.
 * @return -1 on failure (e.g., invalid netmask, interface not found).
 *
 * @pre @p netmask must be a valid IPv4 subnet mask.
 * @pre Caller typically needs root/administrator privileges.
 *
 * @see network_getIpv4Netmask, network_setIpv4Network
 */
int network_setIpv4Netmask(const char *netmask, const char *interface);

/**
 * @brief Gets the IPv4 netmask of a network interface.
 *
 * Retrieves the subnet mask configured on the specified network interface.
 *
 * @param[in] interface The name of the network interface (e.g., "eth0").
 *
 * @return A newly allocated string containing the IPv4 netmask in
 *         dotted-decimal notation.
 * @return NULL if the interface is not found or has no netmask configured.
 *
 * @note The caller is responsible for freeing the returned string using free().
 *
 * @see network_setIpv4Netmask
 */
char *network_getIpv4Netmask(const char *interface);

/**
 * @brief Gets the default IPv4 gateway address.
 *
 * Retrieves the system's default gateway (router) address.
 *
 * @return A newly allocated string containing the gateway IPv4 address.
 * @return NULL if no default gateway is configured.
 *
 * @note The caller is responsible for freeing the returned string using free().
 *
 * @see network_setIpv4Network
 */
char *network_getIpv4Gateway();

/**
 * @brief Configures the complete IPv4 network settings.
 *
 * Sets the IP address, netmask, and default gateway in a single operation.
 * This is useful for configuring a static IP address.
 *
 * @param[in] ip      The IPv4 address in dotted-decimal notation.
 * @param[in] netmask The IPv4 netmask in dotted-decimal notation.
 * @param[in] gateway The IPv4 gateway address in dotted-decimal notation.
 *
 * @return 0 on success.
 * @return -1 on failure.
 *
 * @pre All parameters must be valid IPv4 addresses.
 * @pre Caller typically needs root/administrator privileges.
 *
 * @see network_setIpv4Address, network_setIpv4Netmask, network_getIpv4Gateway
 */
int network_setIpv4Network(const char *ip, const char *netmask, const char *gateway);

/**
 * @brief Sets the system hostname.
 *
 * Changes the hostname of the local machine.
 *
 * @param[in] hostname The new hostname to set.
 *
 * @return 0 on success.
 * @return -1 on failure (e.g., invalid hostname, insufficient privileges).
 *
 * @pre @p hostname must be a valid hostname (alphanumeric with hyphens,
 *      not starting or ending with a hyphen).
 * @pre Caller typically needs root/administrator privileges.
 *
 * @note Changes may require a reboot to take full effect on some systems.
 *
 * @see network_getHostname
 */
int network_setHostname(const char *hostname);

/**
 * @brief Gets the system hostname.
 *
 * Retrieves the current hostname of the local machine.
 *
 * @return A newly allocated string containing the hostname.
 * @return NULL if the hostname could not be retrieved.
 *
 * @note The caller is responsible for freeing the returned string using free().
 *
 * @see network_setHostname
 */
char *network_getHostname();

/**
 * @brief Sets the current username.
 *
 * Changes the username for the current session or user account.
 *
 * @param[in] username The new username to set.
 *
 * @return 0 on success.
 * @return -1 on failure (e.g., invalid username, insufficient privileges).
 *
 * @pre Caller typically needs root/administrator privileges.
 *
 * @see network_getUsername
 */
int network_setUsername(const char *username);

/**
 * @brief Gets the current username.
 *
 * Retrieves the username of the current user.
 *
 * @return A newly allocated string containing the username.
 * @return NULL if the username could not be retrieved.
 *
 * @note The caller is responsible for freeing the returned string using free().
 *
 * @see network_setUsername
 */
char *network_getUsername();

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_NETWORK_H_
