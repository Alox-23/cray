#!/bin/bash

# Check if binary path is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <binary_path> [arguments...]"
    exit 1
fi

BINARY="$1"
shift
ARGS="$@"

# Check if binary exists and is executable
if [ ! -f "$BINARY" ] || [ ! -x "$BINARY" ]; then
    echo "Error: $BINARY is not an executable file"
    exit 1
fi

echo "Starting infinite loop: launch $BINARY, kill after 2 seconds, repeat..."
echo "Press Ctrl+C to stop"

# Infinite loop
while true; do
    # Launch the binary in background
    echo "$(date): Launching $BINARY $ARGS"
    "$BINARY" $ARGS &
    
    # Get the PID of the launched process
    PID=$!
    
    # Wait for 2 seconds
    sleep 2
    
    # Kill the process
    echo "$(date): Killing process $PID"
    kill $PID 2>/dev/null
    
    # Wait a brief moment to ensure clean shutdown
    sleep 0.1
    
    # Force kill if still running
    kill -9 $PID 2>/dev/null
    
    # Small delay before restarting
    sleep 0.5
done
