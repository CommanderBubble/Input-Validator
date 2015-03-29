all : main.o example.exe

main.o : 
	g++ -c main.cpp -o main.o

validatedinput.o : 
	g++ -c ValidatedInput.cpp -o ValidatedInput.o

example.exe : 
	g++ -o example.exe main.o