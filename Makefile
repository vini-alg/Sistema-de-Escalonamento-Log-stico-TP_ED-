CC = g++
CFLAGS = -Wall -std=c++11 -g
INC = -I./include
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Lista de arquivos fonte
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
# Lista de arquivos objeto gerados a partir dos fontes
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
# Nome do executável
EXECUTABLE = $(BINDIR)/tp1.out

all: $(BINDIR) $(OBJDIR) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)/*.o $(EXECUTABLE)

.PHONY: all clean
