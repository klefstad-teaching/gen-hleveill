CXX = g++
CXXFLAGS = -std=c++11 -ggdb -Wall
LDFLAGS = -ggdb
O = Inst.o registers.o testInst.o

testInst: $O
	g++ $(LDFLAGS) $O -o testInst

clean:
	/bin/rm -f $O testInst testInst.out

test: testInst
	testInst > testInst.out
