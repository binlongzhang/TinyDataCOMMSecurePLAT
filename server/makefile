src=$(wildcard *.cpp)
objs=$(patsubst %.cpp,%.o,$(src))
target=serverMain
LIBS=-lstdc++ -lssl -lcrypto -pthread
$(target):$(objs)
	g++ -g -o $@ $^ $(LIBS) 

%.o:%.cpp
	g++ -g -std=c++11 -c $< -pthread

.PHONY:clean
clean:
	rm -f $(target) $(objs)
