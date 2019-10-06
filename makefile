LDIR=./lib

all: 11 22 33 44
11:
	gcc sham.c -o sham -Llib -linfodef -lrandombytes -lm -Wall
22:
	gcc rsa.c -o rsa -Llib -linfodef -lrandombytes -lm -Wall
33:
	gcc l-gamal.c -o lgamal -Llib -linfodef -lrandombytes -lm -Wall
44:
	gcc vernam.c -o vernam -Llib -linfodef -lrandombytes -lm -Wall
