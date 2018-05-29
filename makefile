all:SkipList

test.o: test.cpp   
	g++ -ldl -std=c++0x -Wall -Wextra -pedantic -c test.cpp



SkipList: test.o  
	g++ -ldl -std=c++0x -Wall -Wextra -pedantic test.o -o Skip
	
clean:
	rm -f *.o Skip
