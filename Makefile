INC_DIR:= ./inc/ ./inc/net/ ./inc/threadpool/ ./inc/timer/ ./inc/cache/
SRC_DIR:= ./src ./src/net ./src/threadpool ./src/timer ./src/cache/
SRCS:=$(wildcard ./src/*.cc) $(wildcard ./src/net/*.cc) $(wildcard ./src/net/*.c) $(wildcard ./src/threadpool/*.cc) $(wildcard ./src/timer/*.cc) $(wildcard ./src/cache/*.cc)
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -lpthread

CXX:=g++

CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) -Wno-deprecated

EXE:=./bin/main

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
