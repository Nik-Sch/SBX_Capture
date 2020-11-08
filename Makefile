.PHONY: clean

build:
	gcc capture.c -o capture -lpcap

clean:
	rm capture

