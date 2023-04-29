src:=$(filter-out clientDemo.cpp serverDemo.cpp, $(wildcard *.cpp))
# src:=$(wildcard *.cpp)
objs=$(patsubst %.cpp,%.o,$(src))
target=clientDemo serverDemo
LIBS=-lstdc++ -lssl -lcrypto -pthread -lmysqlclient
serverDemo:$(objs)
	g++ -g -o $@ $@.cpp $^ $(LIBS) 

clientDemo:$(objs)
	g++ -g -o $@ $@.cpp $^ $(LIBS)

%.o:%.cpp
	g++ -g -std=c++11 -c $< $(LIBS) 

.PHONY:clean
clean:
	rm -f $(target) $(objs)
