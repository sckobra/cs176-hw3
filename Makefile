
CC = gcc
ARGS = -Wall

# Keep the original structure but build the current program in this directory.
all: PingClient

# Build the C UDP client that is present in this repo
PingClient: PingClient.c
	$(CC) $(ARGS) -o PingClient PingClient.c

.PHONY: run_server run_client clean

# Run the Python server 
run_server:
	python3 UDPPingerServer.py
	
# run client
run_client: PingClient
	./PingClient 127.0.0.1 32000

clean:
	rm -f PingClient *~ core
