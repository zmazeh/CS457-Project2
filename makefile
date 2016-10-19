all: ss awget

ss: ss.c
	gcc -g ss.c -o ss
awget: awget.c
	gcc -g awget.c -o awget
