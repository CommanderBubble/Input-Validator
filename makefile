all : main.o example.exe

main.o :
	g++ -c main.cpp -o main.o

example.exe :
	g++ -o example.exe main.o