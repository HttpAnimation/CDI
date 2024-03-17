#!/bin/bash

# Create Builds folder if it doesn't exist
mkdir -p Builds

# Get current date and time
current_datetime=$(date +'%Y-%m-%d_%H-%M-%S')

# Create a folder with the current date and time
build_folder="Builds/$current_datetime"
mkdir -p "$build_folder"

# Compile c scripts into the build folder
gcc CDI.c -o "$build_folder/CDI" `pkg-config --cflags --libs gtk+-3.0`

# Copy files into the build folder
cp CDI.c "$build_folder/CDI.c"
cp README.md "$build_folder/README.md"
cp LICENSE "$build_folder/LICENSE"

# Create a Version.ini file with the current date and time
echo "$current_datetime" > "$build_folder/Version.ini"

# Create README file
readme_file="$build_folder/README.txt"
echo "Build created at: $current_datetime" > "$readme_file"
echo "This folder contains builds for the server and client programs." >> "$readme_file"
echo "This is auto message and might work or might not." >> "$readme_file"

# Print the path to the build folder
echo "Builds created at: $build_folder"
echo "./$build_folder/CDI"