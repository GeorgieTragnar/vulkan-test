#!/bin/bash

# Kill the existing screens
# screen -S va-signalingserver -X quit
# screen -S va-httpserver -X quit

# Start the signaling server in a new screen
screen -dmS va-signalingserver bash -c "cd libdatachannel/examples/signaling-server-python && python3 signaling-server.py"
screen -dmS va-httpserver bash -c "cd libdatachannel/examples/streamer && python3 -m http.server --bind 127.0.0.1 8080"

echo "Servers restarted in screen sessions."
