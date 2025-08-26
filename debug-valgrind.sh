#!/bin/bash

# Load environment variables from .env file
set -a  # automatically export all variables
source .env
set +a  # disable automatic export

# Start the application with Valgrind for memory debugging
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./build/my-project-exe
