src:=$(filter-out testDataBase.cpp, $(wildcard *.cpp))
# src:=$(wildcard *.cpp)
objs=$(patsubst %.cpp,%.o,$(src))
target=serverMain
LIBS=-lstdc++ -lssl -lcrypto -pthread -lmysqlclient
$(target):$(objs)
	g++ -g -o $@ $^ $(LIBS) 

%.o:%.cpp
	g++ -g -std=c++11 -c $< $(LIBS) 

.PHONY:clean
clean:
	rm -f $(target) $(objs)
