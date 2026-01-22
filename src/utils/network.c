// SPDX-License-Identifier: Apache-2.0

#include "utility-c/utils/network.h"

#ifdef S_POSIX
  #include <arpa/inet.h>
  #include <limits.h>
  #include <net/if.h>
  #include <netdb.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <unistd.h>

  #include <ifaddrs.h>
  #include <net/route.h>
  #include <sys/ioctl.h>
#else
// FIXME(Sentenz) ecos: the standard includes cause compiler errors, replace them
// with native includes to fix the compiler problems
// #include <stdlib.h>

  #include "bspadapt.h"
  #include "nv_data.h"
  #include "usriod_utils.h"
#endif  // S_POSIX

#ifdef S_POSIX

static int internal_posix_setIpv4Address(const char *ip, const char *interface) {
  struct ifreq ifr;

  /* Create AF_INET (network interface IPv4) socket */
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    return -1;
  }

  /* Define IPv4 Address type */
  ifr.ifr_addr.sa_family = AF_INET;
  /* Define the port name where to attached the network, e.g. eth0 */
  /* Use strncpy to avoid reading past the source string. Ensure null-termination. */
  strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);
  ifr.ifr_name[IFNAMSIZ - 1] = S_NULL_CHAR;

  struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
  /* Convert ip address in correct format */
  inet_pton(AF_INET, ip, &addr->sin_addr);

  /* Set the ip address */
  int err = ioctl(fd, SIOCSIFADDR, &ifr);
  if (err != 0) {
    return -1;
  }

  /* Cleanup */
  close(fd);

  return 0;
}

static char *internal_posix_getIpv4Address(const char *interface) {
  struct ifaddrs *ifaddr, *ifa;
  int family, s;

  char host[NI_MAXHOST];
  memset(host, 0, NI_MAXHOST);

  if (getifaddrs(&ifaddr) == -1) {
    return NULL;
  }

  /* Walk through linked list, maintaining head pointer so we can free list later */
  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL) {
      continue;
    }

    family = ifa->ifa_addr->sa_family;

    /* AF_INET interface address, except lo interface */
    if ((family == AF_INET) && (0 == (ifa->ifa_flags & IFF_LOOPBACK))) {
  #if defined(POSSIBLE_DEAD_CODE)
  // TODO(Sentenz) use passing "interface" argument if not NULL
  // if (family == AF_INET) {
  //   if ((interface != NULL) && (strcmp(ifa->ifa_name, interface) != 0)) {
  //     continue;
  //   }
  #endif  // POSSIBLE_DEAD_CODE

      s = getnameinfo(ifa->ifa_addr,
                      sizeof(struct sockaddr_in),
                      host,
                      NI_MAXHOST,
                      NULL,
                      0,
                      NI_NUMERICHOST);
      if (s != 0) {
        return NULL;
      }
    }
  }

  /* Cleanup */
  if (ifaddr) {
    freeifaddrs(ifaddr);
    ifaddr = NULL;
  }

  /* Check if host has a valid content */
  if (host[0] == S_NULL_CHAR) {
    return NULL;
  }

  return strdup(host);
}

static char *internal_posix_getIpv6Address(const char *interface) {
  struct ifaddrs *ifaddr, *ifa;
  int family, s;

  char host[NI_MAXHOST];
  memset(host, 0, NI_MAXHOST);

  if (getifaddrs(&ifaddr) == -1) {
    return NULL;
  }

  /* Walk through linked list, maintaining head pointer so we can free list later */
  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL) {
      continue;
    }

    family = ifa->ifa_addr->sa_family;

    /* AF_INET interface address, except lo interface */
    if ((family == AF_INET6) && (0 == (ifa->ifa_flags & IFF_LOOPBACK))) {
  #if defined(POSSIBLE_DEAD_CODE)
  // TODO(Sentenz) use passing "interface" argument if not NULL
  // if (family == AF_INET6) {
  //   if ((interface != NULL) && (strcmp(ifa->ifa_name, interface) != 0)) {
  //     continue;
  //   }
  #endif  // POSSIBLE_DEAD_CODE

      s = getnameinfo(ifa->ifa_addr,
                      sizeof(struct sockaddr_in6),
                      host,
                      NI_MAXHOST,
                      NULL,
                      0,
                      NI_NUMERICHOST);
      if (s != 0) {
        return NULL;
      }
    }
  }

  /* Cleanup */
  if (ifaddr) {
    freeifaddrs(ifaddr);
    ifaddr = NULL;
  }

  /* Check if host has a valid content */
  if (host[0] == S_NULL_CHAR) {
    return NULL;
  }
  return strdup(host);
}

static int internal_posix_setIpv4Netmask(const char *netmask, const char *interface) {
  struct ifreq ifr;

  /* Create AF_INET (network interface IPv4) socket */
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    return -1;
  }

  /* Define IPv4 Address type */
  ifr.ifr_addr.sa_family = AF_INET;
  /* Define the port name where to attached the network, e.g. eth0 */
  /* Use strncpy to avoid reading past the source string. Ensure null-termination. */
  strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);
  ifr.ifr_name[IFNAMSIZ - 1] = S_NULL_CHAR;

  struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
  /* Convert netmask address in correct format to write */
  inet_pton(AF_INET, netmask, &addr->sin_addr);

  /* Set the netmask address */
  int err = ioctl(fd, SIOCGIFNETMASK, &ifr);
  if (err != 0) {
    return -1;
  }

  /* Cleanup */
  close(fd);

  return 0;
}

static char *internal_posix_getIpv4Netmask(const char *interface) {
  struct ifaddrs *ifaddr, *ifa;
  int family, s;

  char host[NI_MAXHOST];
  memset(host, 0, NI_MAXHOST);

  if (getifaddrs(&ifaddr) == -1) {
    return NULL;
  }

  /* Walk through linked list, maintaining head pointer so we can free list later */
  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL) {
      continue;
    }

    family = ifa->ifa_addr->sa_family;

    /* AF_INET interface address, except lo interface */
    if ((family == AF_INET) && (0 == (ifa->ifa_flags & IFF_LOOPBACK))) {
  #if defined(POSSIBLE_DEAD_CODE)
  // TODO(Sentenz) use passing "interface" argument if not NULL
  // if (family == AF_INET) {
  //   if ((interface != NULL) && (strcmp(ifa->ifa_name, interface) != 0)) {
  //     continue;
  //   }
  #endif  // POSSIBLE_DEAD_CODE

      s = getnameinfo(ifa->ifa_netmask,
                      sizeof(struct sockaddr_in),
                      host,
                      NI_MAXHOST,
                      NULL,
                      0,
                      NI_NUMERICHOST);
      if (s != 0) {
        return NULL;
      }
    }
  }

  /* Cleanup */
  if (ifaddr) {
    freeifaddrs(ifaddr);
    ifaddr = NULL;
  }

  /* Check if host has a valid content */
  if (host[0] == S_NULL_CHAR) {
    return NULL;
  }

  return strdup(host);
}

static char *internal_posix_getIpv4Gateway() {
  return NULL;
}

static int internal_posix_setIpv4Network(const char *ip, const char *netmask, const char *gateway) {
  /* Create the socket */
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    return -1;
  }

  struct rtentry route;
  memset(&route, 0, sizeof(route));

  struct sockaddr_in *addr;
  addr                  = (struct sockaddr_in *)&route.rt_gateway;
  addr->sin_family      = AF_INET;
  addr->sin_addr.s_addr = inet_addr(gateway);
  addr                  = (struct sockaddr_in *)&route.rt_dst;
  addr->sin_family      = AF_INET;
  addr->sin_addr.s_addr = inet_addr(ip);
  addr                  = (struct sockaddr_in *)&route.rt_genmask;
  addr->sin_family      = AF_INET;
  addr->sin_addr.s_addr = inet_addr(netmask);
  route.rt_flags        = RTF_UP | RTF_GATEWAY;
  route.rt_metric       = 0;

  int err = ioctl(fd, SIOCADDRT, &route);
  if (err != 0) {
    return -1;
  }

  return 0;
}

static int internal_posix_setHostname(const char *hostname) {
  return sethostname(hostname, strlen(hostname));
}

static char *internal_posix_getHostname() {
  char hostname[HOST_NAME_MAX];
  memset(hostname, 0, HOST_NAME_MAX);

  gethostname(hostname, HOST_NAME_MAX);

  /* Check if host has a valid content */
  if (hostname[0] == S_NULL_CHAR) {
    return NULL;
  }

  return strdup(hostname);
}

static int internal_posix_setUsername(const char *username) {
  /* GCC Linker warning: setlogin is not implemented and will always fail */
  // return setlogin(username);
  return -1;
}

static char *internal_posix_getUsername() {
  char username[LOGIN_NAME_MAX];
  memset(username, 0, LOGIN_NAME_MAX);

  getlogin_r(username, LOGIN_NAME_MAX);

  /* Check if host has a valid content */
  if (username[0] == S_NULL_CHAR) {
    return NULL;
  }

  return strdup(username);
}

#else

/**
 * @brief Input (from parameter), check and store new device name
 * @param[in]       char*    deviceName
 * @return          int     result of operation
 *
 * @attention The new device name is stored afterwards in the private data block
 * of the nv_data.
 */
static int internal_pnio_nvDataStoreDeviceName(char *deviceName) {
  size_t deviceNameLen = strlen(deviceName);

  if (deviceNameLen == 0 || deviceNameLen >= DEVICE_NAME_MAXLEN) {
    return -1;
  }

  return ParamStoreDeviceName(deviceName, deviceNameLen);
}

/**
 * @brief Input (no Input), restore device name
 * @return          char*     restored device name
 *
 * @attention The current device name is restored from the private data block of
 * the nv_data.
 */
static char *internal_pnio_nvDataRestoreDeviceName() {
  char *deviceName;
  int deviceNameLen;

  /* Get current device name stored in non volatile data */
  int err = Bsp_nv_data_restore(PNIO_NVDATA_DEVICENAME, (PNIO_VOID **)&deviceName, &deviceNameLen);
  if (err == 0) {
    OsFree(deviceName);
    return NULL;
  }

  /* add \0 after text string, so it will end printf*/
  deviceName[deviceNameLen] = 0x00; /* \0 */

  return deviceName;
}

/**
 * @brief Input (from parameter), check and store new Ip suite
 * @param[in]       char*    ipSuite
 * @return          int     result of operation
 *
 * @attention The new Ip suite is stored afterwards in the private data block of
 * the nv_data.
 */
static int internal_pnio_nvDataStoreIpAddress(char *ipSuite) {
  return ParamStoreIpAddress(ipSuite);
}

/**
 * @brief Input (no Input), restore device name
 * @return          char*     restored device name
 *
 * @attention The new device name is restored from the private data block of the
 * nv_data.
 */
static char *internal_pnio_nvDataRestoreIpAddress() {
  PNIO_UINT8 *ipSuite    = NULL;
  PNIO_UINT32 ipSuiteLen = 0;
  char *ipAddress        = NULL;
  size_t ipAddressLen    = IP_SUITE_LEN + 3;

  /* Get current ip suite stored in non volatile data */
  int err = Bsp_nv_data_restore(PNIO_NVDATA_IPSUITE, (PNIO_VOID **)&ipSuite, &ipSuiteLen);
  if (err == 0) {
    OsFree(ipSuite);
    return NULL;
  }

  ipAddress = malloc(ipAddressLen);
  // FIXME(Sentenz) lint: never use sprintf, use snprintf instead
  sprintf(ipAddress,
          "%03d.%03d.%03d.%03d",
          *(ipSuite + 0),
          *(ipSuite + 1),
          *(ipSuite + 2),
          *(ipSuite + 3));

  OsFree(ipSuite);

  return ipAddress;
}

/**
 * @brief Input (no Input), restore subnet mask
 * @return          char*     restored subnet mask
 *
 * @attention The current subnet mask is restored from the private data block of
 * the nv_data.
 */
static char *internal_pnio_nvDataRestoreSubnetMask() {
  PNIO_UINT8 *ipSuite    = NULL;
  PNIO_UINT32 ipSuiteLen = 0;
  char *subnetMask       = NULL;
  size_t subnetMaskLen   = IP_SUITE_LEN + 3;

  /* Get current ip suite stored in non volatile data */
  int err = Bsp_nv_data_restore(PNIO_NVDATA_IPSUITE, (PNIO_VOID **)&ipSuite, &ipSuiteLen);
  if (err == 0) {
    OsFree(ipSuite);
    return NULL;
  }

  subnetMask = malloc(subnetMaskLen);
  // FIXME(Sentenz) lint: never use sprintf, use snprintf instead
  sprintf(subnetMask,
          "%03d.%03d.%03d.%03d",
          *(ipSuite + 4),
          *(ipSuite + 5),
          *(ipSuite + 6),
          *(ipSuite + 7));

  OsFree(ipSuite);

  return subnetMask;
}

/**
 * @brief Input (no Input), restore gateway
 * @return          char*     restored gateway
 *
 * @attention The current gateway is restored from the private data block of the
 * nv_data.
 */
static char *internal_pnio_nvDataRestoreGateway() {
  PNIO_UINT8 *ipSuite    = NULL;
  PNIO_UINT32 ipSuiteLen = 0;
  char *gateway          = NULL;
  size_t gatewayLen      = IP_SUITE_LEN + 3;

  /* Get current ip suite stored in non volatile data */
  int err = Bsp_nv_data_restore(PNIO_NVDATA_IPSUITE, (PNIO_VOID **)&ipSuite, &ipSuiteLen);
  if (err == 0) {
    OsFree(ipSuite);
    return NULL;
  }

  gateway = malloc(gatewayLen);
  // FIXME(Sentenz) lint: never use sprintf, use snprintf instead
  sprintf(gateway,
          "%03d.%03d.%03d.%03d",
          *(ipSuite + 8),
          *(ipSuite + 9),
          *(ipSuite + 10),
          *(ipSuite + 11));

  OsFree(ipSuite);

  return gateway;
}

#endif  // S_POSIX

/**
 * External functions
 */

int network_setIpv4Address(const char *ip, const char *interface) {
#ifdef S_POSIX
  return internal_posix_setIpv4Address(ip, interface);
#else
  UNUSED(interface);
  return internal_pnio_nvDataStoreIpAddress(ip);
#endif  // S_POSIX
}

char *network_getIpv4Address(const char *interface) {
#ifdef S_POSIX
  return internal_posix_getIpv4Address(interface);
#else
  UNUSED(interface);
  return internal_pnio_nvDataRestoreIpAddress();
#endif  // S_POSIX
}

char *network_getIpv6Address(const char *interface) {
#ifdef S_POSIX
  return internal_posix_getIpv6Address(interface);
#else
  UNUSED(interface);
  // TODO(Sentenz) add PNIO getter IPv6 function
  return NULL;
#endif  // S_POSIX
}

int network_setIpv4Netmask(const char *netmask, const char *interface) {
#ifdef S_POSIX
  return internal_posix_setIpv4Netmask(netmask, interface);
#else
  UNUSED(netmask);
  UNUSED(interface);
  // TODO(Sentenz) add PNIO setter netmask function
  return NULL;
#endif  // S_POSIX
}

char *network_getIpv4Netmask(const char *interface) {
#ifdef S_POSIX
  return internal_posix_getIpv4Netmask(interface);
#else
  UNUSED(interface);
  return internal_pnio_nvDataRestoreSubnetMask();
#endif  // S_POSIX
}

char *network_getIpv4Gateway() {
#ifdef S_POSIX
  // TODO(Sentenz) add POSIX getter gateway function
  return internal_posix_getIpv4Gateway();
#else
  return internal_pnio_nvDataRestoreGateway();
#endif  // S_POSIX
}

int network_setIpv4Network(const char *ip, const char *netmask, const char *gateway) {
#ifdef S_POSIX
  return internal_posix_setIpv4Network(ip, netmask, gateway);
#else
  UNUSED(ip);
  UNUSED(netmask);
  UNUSED(gateway);
  // TODO(Sentenz) add PNIO setter network function
  return NULL;
#endif  // S_POSIX
}

int network_setHostname(const char *hostname) {
#ifdef S_POSIX
  return internal_posix_setHostname(hostname);
#else
  return internal_pnio_nvDataStoreDeviceName(hostname);
#endif  // S_POSIX
}

char *network_getHostname() {
#ifdef S_POSIX
  return internal_posix_getHostname();
#else
  return internal_pnio_nvDataRestoreDeviceName();
#endif  // S_POSIX
}

int network_setUsername(const char *username) {
#ifdef S_POSIX
  return internal_posix_setUsername(username);
#else
  /* Not supported by PNIO */
  return 0;
#endif  // S_POSIX
}

char *network_getUsername() {
#ifdef S_POSIX
  return internal_posix_getUsername();
#else
  /* Not supported by PNIO */
  return NULL;
#endif  // S_POSIX
}
