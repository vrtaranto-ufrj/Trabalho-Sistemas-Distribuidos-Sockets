#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>

#include "consumidor.h"

void getMyIp(struct in_addr *ip) {
	struct ifaddrs *addrs;
	int ret, family;
	char host[NI_MAXHOST];

	ret = getifaddrs(&addrs);
	if (ret == -1) {
		perror("getifaddrs");
		exit(EXIT_SUCCESS);
	}

	for (struct ifaddrs *ifa = addrs; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL) {
			continue;
		}

		family = ifa->ifa_addr->sa_family;

		if (family == AF_INET && !(ifa->ifa_flags & IFF_LOOPBACK)) {
			ret = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (ret != 0) {
				fprintf(stderr, "getnameinfo() failed: %s\n", gai_strerror(ret));
				exit(EXIT_FAILURE);
			}

			ret = inet_aton(host, ip);
			if (ret == 0) {
				fprintf(stderr, "Formato IP invalido!\n");
				exit(EXIT_FAILURE);
			}
			break;
		}
	}
	freeifaddrs(addrs);
}

bool isPrime(unsigned long num) {
	if (num == 1) {
		return false;
	}
	if (num == 2 || num == 3) {
		return true;
	}
	if (num % 2 == 0 || num % 3 == 0) {
		return false;
	}
	for (unsigned long i = 5; i * i <= num; i += 6) {
		if (num % i == 0 || num % (i + 2) == 0) {
			return false;
		}
	}

	return true;
}

unsigned long toNum(const char *num_str) {
	char *endptr;
	unsigned long num;
	
	errno = 0;
	num = strtoul(num_str, &endptr, BASE);
	if (errno != 0) {
		perror("strtol");
		exit(EXIT_FAILURE);
	}

	if (endptr == num_str) {
		fprintf(stderr, "Nenhum digito foi encontrado\n");
		exit(EXIT_FAILURE);
	}

	// Se chegou até aqui, a string começa com digitos
	
	return num;

}
