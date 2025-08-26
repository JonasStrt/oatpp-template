#!/bin/bash

# Load environment variables from .env file
set -a  # automatically export all variables
source .env
set +a  # disable automatic export

# Start the application with GDB
gdb --args ./build/my-project-exe
