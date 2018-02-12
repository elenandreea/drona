.PHONEY:build clean

build:livrare

livrare:livrare.c
	gcc -Wall livrare.c -o livrare

clean:
	rm -rf livrare
