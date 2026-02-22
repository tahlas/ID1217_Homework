#!/bin/bash

NUM_CLIENTS=$1

# Start the server in the background
java Server &
SERVER_PID=$!

# Wait a moment to ensure the server is ready
sleep 2

# Start clients
for i in $(seq 1 $NUM_CLIENTS)
do
    java Client $i &
done

# Wait for all clients to finish
wait

# Optionally kill the server process
kill $SERVER_PID