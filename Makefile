CC=cc

all:
	$(CC) ssplit.c -o ssplit -lm

clean:
	rm -f ./ssplit
