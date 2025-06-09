debug: main.cpp
	g++ -DDEBUG main.cpp -o debug.out
	./debug.out

production: main.cpp
	g++ main.cpp -o production.out
	./production.out


clean:
	rm *.out