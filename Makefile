.SUFFIXES: .cpp

CXX = g++-7
CXXFLAGS = -std=c++11 -I./include -framework OpenGL
LDFLAGS = -lglfw -lglew

ODIR = include
OBJ = main.o $(ODIR)/shader.o $(ODIR)/camera.o $(ODIR)/ruler.o

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

run: $(OBJ)
	$(CXX) $(CXXFLAGS) $? $(LDFLAGS)
	./a.out

clean:
	rm -f ./include/*.o $(OBJ) *.out
