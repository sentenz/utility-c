// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_NETWORK_H_
#define INCLUDE_UTILITY_C_NETWORK_H_

#ifdef S_HAVE_CONFIG_H
  #include "utility-c/config.h"
#endif

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Sets the IPv4 address of a network interface.
/// @param ip The IPv4 address to set.
/// @param interface The name of the network interface to set the address for.
/// @return 0 on success, -1 on failure.
int network_setIpv4Address(const char *ip, const char *interface);

/// @brief Gets the IPv4 address of a network interface.
/// @param interface The name of the network interface to get the address for.
/// @return The IPv4 address of the network interface, or NULL if not found.
/// @note The caller is responsible for freeing the memory using free().
char *network_getIpv4Address(const char *interface);

/// @brief Gets the IPv6 address of a network interface.
/// @param interface The name of the network interface to get the address for.
/// @return The IPv6 address of the network interface, or NULL if not found.
char *network_getIpv6Address(const char *interface);

/// @brief Sets the IPv4 netmask of a network interface.
/// @param netmask The IPv4 netmask to set.
/// @param interface The name of the network interface to set the netmask for.
/// @return 0 on success, -1 on failure.
int network_setIpv4Netmask(const char *netmask, const char *interface);

/// @brief Gets the IPv4 netmask of a network interface.
/// @param interface The name of the network interface to get the netmask for.
/// @return The IPv4 netmask of the network interface, or NULL if not found.
/// @note The caller is responsible for freeing the memory using free().
char *network_getIpv4Netmask(const char *interface);

/// @brief Gets the IPv4 gateway address.
/// @return The IPv4 gateway address, or NULL if not found.
/// @note The caller is responsible for freeing the memory using free().
char *network_getIpv4Gateway();

/// @brief Sets the IPv4 network configuration.
/// @param ip The IPv4 address to set.
/// @param netmask The IPv4 netmask to set.
/// @param gateway The IPv4 gateway address to set.
/// @return 0 on success, -1 on failure.
int network_setIpv4Network(const char *ip, const char *netmask, const char *gateway);

/// @brief Sets the hostname of the device.
/// @param hostname The hostname to set.
/// @return 0 on success, -1 on failure.
int network_setHostname(const char *hostname);

/// @brief Gets the hostname of the device.
/// @return The hostname of the device, or NULL if not found.
/// @note The caller is responsible for freeing the memory using free().
char *network_getHostname();

/// @brief Sets the username of the device.
/// @param username The username to set.
/// @return 0 on success, -1 on failure.
int network_setUsername(const char *username);

/// @brief Gets the username of the device.
/// @return The username of the device, or NULL if not found.
/// @note The caller is responsible for freeing the memory using free().
char *network_getUsername();

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_NETWORK_H_
