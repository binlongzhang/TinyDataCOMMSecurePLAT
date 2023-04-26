src=$(wildcard *.cpp)
objs=$(patsubst %.cpp,%.o,$(src))
target=clientMain
LIBS=-lstdc++ -lssl -lcrypto
$(target):$(objs)
	g++ -g -o $@ $^ $(LIBS) 

%.o:%.cpp
	g++ -g -std=c++11 -c $<

.PHONY:clean
clean:
	rm -f $(target) $(objs)
