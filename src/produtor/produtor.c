#include "produtor.h"

#include <stdlib.h>
#include <stdio.h>

int geraAleatorio(int ultimo) {
	int delta;

	delta = rand() & 101;
	return ultimo + delta;

}

void toStr(int num, char *str) {
	snprintf(str, MAX_BYTES_INT, "%d", num);
}
