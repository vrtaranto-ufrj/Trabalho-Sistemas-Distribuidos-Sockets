#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "produtor.h"

int main(int argc, const char **argv) {
	const char *ip_str = argv[1];
	int sfd, ret, num;
	struct sockaddr_in addr;
	ssize_t len_msg, len_msg_recv;
	char buffer[BUFF_SIZE], buffer_receber, *endptr;
	unsigned long quantidade, vezes;

	// Checa o número de argumentos
	if (argc != NUM_ARGS) {
		fprintf(stderr, "Uso: %s <ip> <numero-a-gerar>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Transforma o argv[2] em um inteiro
	errno = 0;
	quantidade = strtoul(argv[2], &endptr, BASE);
	if (errno != 0) {
		perror("strtoul");
		exit(EXIT_FAILURE);
	}

	if (endptr == argv[2]) {
		fprintf(stderr, "Nenhum digito foi encontrado\n");
	}
	
	
	// Coloca a mémoria da struct sockaddr_in tudo em 0
	memset(&addr, 0, sizeof(addr));

	// Inicializando a struct sockaddr_in
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORTA);
	ret = inet_pton(AF_INET, ip_str, &addr.sin_addr);
	if (ret == 0) {
		fprintf(stderr, "Formato IP invalido!\n");
		exit(EXIT_FAILURE);
	}

	// Inicia o Socket
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// Conecta com o consumidor
	ret = connect(sfd, (struct sockaddr*) &addr, sizeof(addr));
	if (ret == -1) {
		perror("connect");
		exit(EXIT_FAILURE);
	}

	printf("Conectado!\n");
	
	num = 1;
	vezes = 0;

	srand(time(NULL));
	while (1) {

		if (vezes == quantidade) {
			// Chegou ao fim das vezes
			num = 0;
		}
		else {
			// Gera o número aleatório
			num = geraAleatorio(num);
		}

		
		// Converte para String e coloca no buffer
		toStr(num, buffer);
		
		// Manda o número
		len_msg = send(sfd, buffer, strlen(buffer)+1, 0);
		if (len_msg == -1) {
			perror("send");
			exit(EXIT_FAILURE);
		}
		
		// Espera receber a resposta
		len_msg_recv = recv(sfd, &buffer_receber, SIZE_RECV, 0);
		if (len_msg_recv == -1) {
			perror("recv");
			exit(EXIT_FAILURE);
		}
		// Se o tamanho da mensagem for 0, então a conexão foi fechada abruptamente
		if (len_msg_recv == 0) {
			break;
		}
		
		// Checa se o número era primo
		if (buffer_receber) {
			printf("O número %d é primo\n", num);
		}
		else {
			printf("O número %d não é primo\n", num);
		}
		
		vezes++;
	}

	// Fecha o socket
	ret = close(sfd);
	if (ret == -1) {
		perror("close");
		exit(EXIT_FAILURE);
	} 

	return EXIT_SUCCESS;
}
