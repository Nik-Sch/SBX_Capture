.PHONY: clean

all:
	gcc capture.c -o cap.o -lpcap

clean:
	rm *.o

