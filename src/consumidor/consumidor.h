#pragma once

#include <netinet/in.h>
#include <stdbool.h>

#define NUM_ARGS 1
#define PORTA 10005u
#define BUFF_SIZE 20
#define SIZE_SEND 1
#define BASE 10

void getMyIp(struct in_addr *ip);
unsigned long toNum(const char *num_str);
bool isPrime(unsigned long num);
