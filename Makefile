CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -Werror

all:
	kalman

kalman: ft_kalman.cpp
	$(CC) $(CFLAGS) -I/incs/eigen.h -o $@ $<

clean:
	rm -f ft_kalman
