# Diretórios
DIR_CONSUMIDOR = src/consumidor
DIR_PRODUTOR = src/produtor
DIR_OBJ = obj

# Arquivos fontes
SRC_CONSUMIDOR = $(DIR_CONSUMIDOR)/consumidor.c $(DIR_CONSUMIDOR)/main.c
SRC_PRODUTOR = $(DIR_PRODUTOR)/produtor.c $(DIR_PRODUTOR)/main.c

# Arquivos objetos
OBJ_CONSUMIDOR = $(SRC_CONSUMIDOR:$(DIR_CONSUMIDOR)/%.c=$(DIR_OBJ)/consumidor/%.o)
OBJ_PRODUTOR = $(SRC_PRODUTOR:$(DIR_PRODUTOR)/%.c=$(DIR_OBJ)/produtor/%.o)

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g

# Alvo principal
all: consumidor produtor

# Compilação do consumidor
consumidor: $(OBJ_CONSUMIDOR)
	$(CC) $(CFLAGS) -o $@ $(OBJ_CONSUMIDOR)

# Compilação do produtor
produtor: $(OBJ_PRODUTOR)
	$(CC) $(CFLAGS) -o $@ $(OBJ_PRODUTOR)

# Regra de compilação para arquivos .o
$(DIR_OBJ)/consumidor/%.o: $(DIR_CONSUMIDOR)/%.c | $(DIR_OBJ)/consumidor
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_OBJ)/produtor/%.o: $(DIR_PRODUTOR)/%.c | $(DIR_OBJ)/produtor
	$(CC) $(CFLAGS) -c $< -o $@

# Criação dos subdiretórios obj se não existirem
$(DIR_OBJ)/consumidor:
	mkdir -p $(DIR_OBJ)/consumidor

$(DIR_OBJ)/produtor:
	mkdir -p $(DIR_OBJ)/produtor

# Limpeza dos arquivos compilados
clean:
	rm -rf $(DIR_OBJ) consumidor produtor

.PHONY: all clean
