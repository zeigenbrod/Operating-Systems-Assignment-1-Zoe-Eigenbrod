# Project 1 README


## Files Included

producer.c - The producer process code. Generates item and puts in buffer.

consumer.c - The consumer process code. Removes item from buffer.


## How to use

### 1. Compile

$ gcc producer.c -pthread -lrt -o producer

$ gcc consumer.c -pthread -lrt -o consumer


### 2. Run
$ ./producer & ./consumer &

### Note: Need to run in Unix/Linux terminal


