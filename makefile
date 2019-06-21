%/.: 
	mkdir -p $(@D)

SOURCES = HashMap/HashMap.cpp HashMap/Block.cpp Hashmap/Memory.cpp HashMap/Utils.cpp

build/example: | build/.
	g++ -std=c++11 -Wall example.cpp $(SOURCES) -o build/example
