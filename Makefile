# Compiler flags
MYFLAGS=
CXXFIRSTFLAGS= -O3 -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-use-after-free
CXXFIRSTFLAGSDEBUG= -g -O0 -DDEBUG -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-use-after-free

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CXX=g++
	CXXFLAGS= -std=c++20 $(CXXFIRSTFLAGS) $(MYFLAGS)
	CXXFLAGSDEBUG= -std=c++20 $(CXXFIRSTFLAGSDEBUG) $(MYFLAGS)
endif
ifeq ($(UNAME_S),Darwin)
	CXX=clang++
	CXXFLAGS= -std=c++20  -stdlib=libc++ $(CXXFIRSTFLAGS) $(MYFLAGS)
	CXXFLAGSDEBUG= -std=c++20  -stdlib=libc++ $(CXXFIRSTFLAGSDEBUG) $(MYFLAGS)
endif

# Directories
SRCDIR=src
HPPDIR=include 
OBJDIR=obj
BINDIR=bin

# Files
SOURCES=$(foreach sdir, $(SRCDIR), $(wildcard $(sdir)/*.cpp))
OBJECTS=$(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SOURCES)))

vpath %.cpp $(SRCDIR)
vpath %.o $(OBJDIR)

# Reminder, 'cause it is easy to forget makefile's fucked-up syntax...
# $@ is what triggered the rule, ie the target before :
# $^ is the whole dependencies list, ie everything after :
# $< is the first item in the dependencies list

# Rules

all: $(BINDIR)/sabrinasgame_v1 $(BINDIR)/sabrinasgame_v2 $(BINDIR)/sabrinasgame_v3

debug: CXXFLAGS=$(CXXFLAGSDEBUG)
debug: $(BINDIR)/sabrinasgame_v1 $(BINDIR)/sabrinasgame_v2 $(BINDIR)/sabrinasgame_v3

$(BINDIR)/sabrinasgame_v1: $(OBJDIR)/main.o $(OBJDIR)/solution.o $(OBJDIR)/solution_v1.o $(OBJDIR)/utils.o
	$(CXX) -o $@ $^

$(BINDIR)/sabrinasgame_v2: $(OBJDIR)/main.o $(OBJDIR)/solution.o $(OBJDIR)/solution_v2.o $(OBJDIR)/utils.o
	$(CXX) -o $@ $^

$(BINDIR)/sabrinasgame_v3: $(OBJDIR)/main.o $(OBJDIR)/solution.o $(OBJDIR)/solution_v3.o $(OBJDIR)/utils.o
	$(CXX) -o $@ $^

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(OBJDIR)/solution.o $(OBJDIR)/utils.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -Ithirdparty -c $(SRCDIR)/main.cpp -o $@

$(OBJDIR)/solution.o: $(SRCDIR)/solution.cpp $(OBJDIR)/utils.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -Ithirdparty -c $(SRCDIR)/solution.cpp -o $@

$(OBJDIR)/solution_v1.o: $(SRCDIR)/solution_v1.cpp $(OBJDIR)/solution.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/solution_v1.cpp -o $@

$(OBJDIR)/solution_v2.o: $(SRCDIR)/solution_v2.cpp $(OBJDIR)/solution.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/solution_v2.cpp -o $@

$(OBJDIR)/solution_v3.o: $(SRCDIR)/solution_v3.cpp $(OBJDIR)/solution.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -Ithirdparty -c $(SRCDIR)/solution_v3.cpp -o $@

$(OBJDIR)/utils.o: $(SRCDIR)/utils.cpp
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/utils.cpp -o $@

.PHONY: clean

clean:
	rm -fr core $(BINDIR)/sabrinasgame_v1 $(BINDIR)/sabrinasgame_v2 $(BINDIR)/sabrinasgame_v3 $(OBJDIR)/main.o $(OBJDIR)/solution.o $(OBJDIR)/solution_v1.o $(OBJDIR)/solution_v2.o $(OBJDIR)/solution_v3.o $(OBJDIR)/utils.o
