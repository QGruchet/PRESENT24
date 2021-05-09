OFLAGS=gcc -Wall -Wextra -O3 -c -g -pedantic
OPTI=gcc -O3 -Wall -Wextra -pedantic
TFLAGS= -pthread
DFLAGS=chiffrement.o dechiffrement.o fonction.o attaque.o main.o
NAME=GRUCHET

all:
	$(OPTI) $(TFLAGS) src/* -o main

compile: $(DFLAGS)
	$(OPTI) -o main $(DFLAGS)

main.o: main.c chiffrement.h dechiffrement.h attaque.h
	$(OFLAGS) main.c

dechiffrement.o: dechiffrement.c fonction.h
	$(OFLAGS) dechiffrement.c

chiffrement.o: chiffrement.c fonction.h
	$(OFLAGS) chiffrement.c

fonction.o: fonction.c fonction.h
	$(OFLAGS) fonction.c

attaque.o: attaque.c attaque.h
	$(OFLAGS) attaque.c

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main -a 1

tar:
	@mkdir -p $(NAME)
	@cp -r src/ Makefile README.md Rapport.pdf $(NAME)
	@tar zcvf $(NAME).tar.gz $(NAME)
	@rm -Rf $(NAME)

clean:
	rm -f main
	rm -f $(NAME).tar.gz
	ls -l
