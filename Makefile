NAME=main

all: $(SRC)
	gcc $(NAME).c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
