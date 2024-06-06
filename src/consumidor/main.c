#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "consumidor.h"

int main(int argc, const char **argv) {
	int sfd, sfd_prod, ret, opt;
	struct sockaddr_in myaddr, prodaddr;
	socklen_t addrlen;
	ssize_t len_msg, len_msg_env;
	char buffer[BUFF_SIZE], buffer_enviar;
	unsigned long num;

	// Checa o número de argumentos
	if (argc != NUM_ARGS) {
		fprintf(stderr, "Uso: %s\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Coloca a memória da struct sockaddr_in tudo em 0
	memset(&myaddr, 0, sizeof(myaddr));

	// Inicializando a struct sockaddr_in
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(PORTA);
	
	// Descobre o IP da máquina e coloca na struct sockaddr_in
	getMyIp(&myaddr.sin_addr);

	// Mostra o IP da máquina
	printf("Meu IP: %s\n", inet_ntoa(myaddr.sin_addr));

	// Inicia o Socket
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// Configura o Socket para reutilizar a porta
	opt = 1;
	ret = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (ret == -1) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	// Bind o Socket
	ret = bind(sfd, (struct sockaddr*) &myaddr, sizeof(myaddr));
	if (ret == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}
	
	// Começa a ouvir
	ret = listen(sfd, SOMAXCONN);
	if (ret == -1) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	// Fica esperando a conexão
	printf("Aguardando conexão...\n");
	sfd_prod = accept(sfd, (struct sockaddr*) &prodaddr, &addrlen);
	if (sfd_prod == -1) {

		perror("accept");
		exit(EXIT_FAILURE);
	}

	printf("Conectado!\n");
	
	while (1) {
		// Espera receber uma mensagem
		len_msg = recv(sfd_prod, buffer, BUFF_SIZE, 0);
		if (len_msg == -1) {
			perror("recv");
			exit(EXIT_FAILURE);
		}
		// Se o tamanho da mensagem for 0, o cliente fechou a conexão abrutamente
		if (len_msg == 0) {
			break;
		}


		// Transforma a mensagem em um número e testa
		num = toNum(buffer);
		if (num == 0) {
			printf("Recebido o zero\n");
			break;
		}
		
		// Testa se o número é primo e printa
		printf("isPrime(%lu) = ", num);
		if (isPrime(num)) {
			buffer_enviar = 1;
			printf("true\n");
		}
		else {
			buffer_enviar = 0;
			printf("false\n");
		}

		// Envia a resposta
		len_msg_env = send(sfd_prod, &buffer_enviar, SIZE_SEND, 0);
		if (len_msg_env == -1) {
			perror("send");
			exit(EXIT_FAILURE);
		}

	}

	printf("Terminando programa\n");

	// Fecha os sockets
	ret = close(sfd_prod);
	if (ret == -1) {
		fprintf(stderr, "Produtor Socket ");
		perror("close");
		exit(EXIT_FAILURE);
	}
	ret = close(sfd);
	if (ret == -1) {
		fprintf(stderr, "My Socket ");
		perror("close");
		exit(EXIT_FAILURE);
	}
		
	return EXIT_SUCCESS;
}
