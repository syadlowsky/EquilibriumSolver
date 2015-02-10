EXE = GEF

OBJS = \
	EquilibriumFlow.o HornerPolynomial.o Bush.o\
	GraphEdge.o Origin.o BushNode.o ABGraph.o\
	AlgorithmBSolver.o BarGeraImporter.o BushEdge.o

OBJDIR = ./objs/

INCL = -I./include

VPATH = src:objs

# C++ Compiler command
CXX = c++

CXXFLAGS = \
	-g -pipe -pedantic-errors -Wparentheses -Wreturn-type\
	-Wcast-qual -Wall -Wpointer-arith -Wwrite-strings -Wconversion -O3\
	-march=native -pg -stdlib=libc++ -std=c++11

# additional C++ Compiler options for linking

all: $(EXE)


.SUFFIXES: .cpp .o

$(EXE): $(OBJS)
	bla=;\
	for file in $(OBJS); do bla=$(OBJDIR)"$$file $$bla"; done; \
	$(CXX) $$bla $(CXXFLAGS) -o $@

clean:
	bla=;\
	for file in $(OBJS); do bla=$(OBJDIR)"$$file $$bla"; done; \
	rm -rf $(EXE) $$bla

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCL) -c -o $(OBJDIR)$@ $<
