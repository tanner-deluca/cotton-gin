####################
#
# Linux Makefile
#
####################


#
# Object Files
#

OBJECTS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))


#
# Project Info
#

PROJECT_LIBRARY = cotton_gin
PROJECT_LIBRARY_PATH = ../../libs
PROJECT = test_logger.exe


#
# Compiler Info
#

# c++
CC = g++

# includes
INCLUDE_PATHS = ../../include
INCLUDE_PARAMETERS = $(foreach d, $(INCLUDE_PATHS), -I$d)

# cotton gin libraries
GIN_LIBRARIES = cotton_gin_logger.a
LIBRARY_PARAMETERS = $(foreach d, $(GIN_LIBRARIES), $(PROJECT_LIBRARY_PATH)/$d)

# compiler flags
GLIB_CFLAGS = `pkg-config --cflags glib-2.0`
GLIB_LDFLAGS = `pkg-config --libs glib-2.0`

LFLAGS = -g -o $(PROJECT)
CPPFLAGS = -g $(INCLUDE_PARAMETERS) $(GLIB_CFLAGS) -Wall -pedantic -std=c++11 -Wno-unknown-pragmas -Wno-variadic-macros

GIN_FLAGS = $(LIBRARY_PARAMETERS)

#
# Compiling
#

$(PROJECT): $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) $(GIN_FLAGS) $(GLIB_LDFLAGS)


#
# Directory Cleanup
#

clean:
	rm *.o $(PROJECT)


#
# Documentation Creation
#

DOXYGEN = doxygen

docs:
	$(DOXYGEN) doxygen.cfg