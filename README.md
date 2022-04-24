# Assignment 3

## Developer Information

Developed by Magnus Rødseth, Julian Grande & Sebastian Sole.

## Running the application

### Using `cmake` and `make`

```sh
# Navigate to the project directory
cd assignment_3

# Create a build directory, if it does not already exist
mkdir build

# Navigate to the build directory
cd build

# Compile the project using cmake
cmake ..

# Build executable using make
make

# Run the application
./flush
```

### Without using `cmake` and `make`

```shell
# Navigate to the project directory
cd assignment_3

# Compile the project
gcc src/app.c src/background_tasks.c src/command_line_parser.c src/commands.c \
src/io_redirection.c src/linked_list.c src/logger.c src/main.c src/pipelines.c \
src/process.c -o flush

# Run the application
./flush
```

## Functionality

```shell
# Execute some system call
ls -l 
echo "Hello, world!"
sleep 2

# Change directory
cd ..
cd ~

# IO redirection
ls -l > out.txt
head -1 < README.md
head -1 < README.md > out.txt

# Start a background task
ls -l > out.txt &
sleep 10 &

# Get an overview of all background tasks
jobs

# Pipelines
ls -l | grep flush | wc -w
ls -l | grep flush > out.txt

# Terminate the program using CTRL + D
```