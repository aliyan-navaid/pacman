debug: main.cpp
	g++ -DDEBUG main.cpp -o debug.out

production: main.cpp
	g++ main.cpp -o production.out