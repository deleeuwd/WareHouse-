runMe:	bin/Customer.o bin/Order.o bin/Action.o  bin/main.o  bin/Volunteer.o bin/WareHouse.o
	g++ -o bin/warehouse bin/Action.o bin/Customer.o bin/Order.o bin/main.o bin/Volunteer.o bin/WareHouse.o

valgrind: runMe
	valgrind --leak-check=full --show-reachable=yes ./bin/warehouse configFileExample.txt
	
# All Targets
all:	runMe	
	./bin/warehouse configFileExample.txt

# Depends on the source and header files
bin/Action.o: 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

bin/Customer.o: 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp

bin/main.o: 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

bin/Order.o: 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Order.o src/Order.cpp

bin/Volunteer.o: 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Volunteer.o src/Volunteer.cpp

bin/WareHouse.o: 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/WareHouse.o src/WareHouse.cpp

# Clean the build directory
clean:
	rm -f bin/*

daniel:
	echo daniel

tamar: daniel
	echo tamar

