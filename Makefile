.SUFFIXES: .cpp

CXX = g++
CXXFLAGS = -std=c++11 -I./include -I./external -framework OpenGL
LDFLAGS = -lglfw -lglew

ODIR = include
LIBDIR = external
OBJ = main.o $(ODIR)/shader.o $(ODIR)/camera.o $(ODIR)/ruler.o $(ODIR)/model_loader.o $(ODIR)/control.o $(ODIR)/physics.o $(LIBDIR)/lodepng.o

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

run: $(OBJ)
	$(CXX) $(CXXFLAGS) $? $(LDFLAGS)
	./a.out

clean:
	rm -f ./include/*.o $(OBJ) *.out
