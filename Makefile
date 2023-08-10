CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -Werror

all: ft_kalman

ft_kalman: 
	$(CC) $(CFLAGS) -I/eigen-3.4.0/Eigen/Dense ft_kalman.cpp KalmanFilter.cpp -o $@ $<

clean:
	rm -f ft_kalman

re: clean all