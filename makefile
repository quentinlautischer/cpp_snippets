build:
	g++ -std=c++17 -obin/snippets.o main.cpp -D _DEBUG 
build_release:
	g++ -std=c++17 -obin/snippets.o main.cpp
run:
	./bin/snippets.o ${ARGS}
