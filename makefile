all:
    g++ -Wall -Werror -O2 -std=c++14 -fsanitize=address main.cpp