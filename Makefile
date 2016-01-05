# CC=g++
# CFLAGS=-std=c++11

# sandbox: final_v.cpp parse.cpp
# 	$(CC) -o sandbox final_v.cpp parse.cpp -std=c++11

CC=g++
CFLAGS=-std=c++11 -w
DEPS = parse.h
OBJ = final_v.out parse.out

%.out: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sandbox: $(OBJ)
	g++ -o $@ $^ $(CFLAGS)