# Adopted from:
# https://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/

CC = g++
SRCDIR = src
BUILDDIR = build
TARGET = bin/mrtp_cli

SRCEXT = cpp
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# To compile without OpenMP, comment out -fopenmp
CFLAGS = -W -Wall -pedantic -O2 -fopenmp
LIB = -lm -lpng -fopenmp
INC = -I/usr/include/eigen3 -I/usr/include/png++ -I./include -I./cpptoml/include


$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean
