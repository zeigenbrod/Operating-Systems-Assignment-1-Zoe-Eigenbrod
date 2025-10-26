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

## Example Output

Produced 1 [1,__]

Produced 2 [1,2]

Consumed 1 [__,2]

Produced 3 [3,2]

Consumed 2 [3,__]

Produced 4 [4,__]

Produced 5 [4,5]

Consumed 3 [__,5]

Consumed 4 [__,__]

Produced 6 [6,__]


