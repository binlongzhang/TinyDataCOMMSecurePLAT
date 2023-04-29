src = $(wildcard *.cpp)
target = $(patsubst %.cpp, %, $(src))
all: $(target)
%:%.cpp
	g++ $< -o $@ -lmysqlclient -lstdc++ -lpthread -ldl -lrt
clean:
	-rm -rf $(target)
.PHONY: all clean