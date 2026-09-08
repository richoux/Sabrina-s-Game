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

all: $(BINDIR)/sabrinasgame_v1 $(BINDIR)/sabrinasgame_v2

debug: CXXFLAGS=$(CXXFLAGSDEBUG)
debug: $(BINDIR)/sabrinasgame_v1 $(BINDIR)/sabrinasgame_v2

$(BINDIR)/sabrinasgame_v1: $(OBJDIR)/main_v1.o $(OBJDIR)/build_v1.o $(OBJDIR)/utils.o
	$(CXX) -o $@ $^

$(BINDIR)/sabrinasgame_v2: $(OBJDIR)/main_v2.o $(OBJDIR)/build_v2.o $(OBJDIR)/utils.o
	$(CXX) -o $@ $^

$(OBJDIR)/main_v1.o: $(SRCDIR)/main.cpp $(OBJDIR)/build_v1.o $(OBJDIR)/utils.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/main.cpp -o $@

$(OBJDIR)/build_v1.o: $(SRCDIR)/build.cpp $(OBJDIR)/utils.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/build.cpp -o $@

$(OBJDIR)/main_v2.o: $(SRCDIR)/main.cpp $(OBJDIR)/build_v2.o $(OBJDIR)/utils.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/main.cpp -o $@

$(OBJDIR)/build_v2.o: $(SRCDIR)/build.cpp $(OBJDIR)/utils.o
	$(CXX) $(CXXFLAGS) -DV2 -I$(HPPDIR) -c $(SRCDIR)/build.cpp -o $@

$(OBJDIR)/utils.o: $(SRCDIR)/utils.cpp
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/utils.cpp -o $@

.PHONY: clean

clean:
	rm -fr core $(BINDIR)/sabrinasgame_v1 $(BINDIR)/sabrinasgame_v2 $(OBJDIR)/main_v1.o $(OBJDIR)/main_v2.o $(OBJDIR)/build_v1.o $(OBJDIR)/build_v2.o $(OBJDIR)/utils.o
