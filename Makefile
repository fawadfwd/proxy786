SOURCE = main.c accept.c read_parse.c
HEADER = header.h

proxy_server: $(SOURCE) $(HEADER)
	gcc -g -lpthread $(SOURCE) -o proxy-server -lssl -lcrypto

clean:
	rm -f proxy_server a.out
