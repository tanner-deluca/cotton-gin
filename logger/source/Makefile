##################
#                #
# Linux Makefile #
#                #
##################


#
# Object Files
#

OBJECTS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))


#
# Project Info
#

PROJECT_LIBRARY = cotton_gin
PROJECT_LIBRARY_PATH = ../../libs
PROJECT = logger
MAJOR = 1
MINOR = 0.1


#
# Compiler Info
#

# clang
CC = g++

# includes
INCLUDE_PATHS = ../include ../../general
INCLUDE_PARAMETERS = $(foreach d, $(INCLUDE_PATHS), -I$d)

# compiler flags
GLIB_CFLAGS = `pkg-config --cflags glib-2.0`
GLIB_LDFLAGS = `pkg-config --libs glib-2.0`

LFLAGS = -g -shared -Wl,-soname,$(PROJECT_LIBRARY)_$(PROJECT).so.$(MAJOR) -o $(PROJECT_LIBRARY_PATH)/$(PROJECT_LIBRARY)_$(PROJECT).so.$(MAJOR).$(MINOR)
CPPFLAGS = -g -std=c++11 -fPIC -Wall -pedantic -Wno-unknown-pragmas -Wno-variadic-macro $(INCLUDE_PARAMETERS) $(GLIB_CFLAGS)


#
# Compiling
#

$(PROJECT): $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) $(GLIB_LDFLAGS)


#
# Directory Cleanup
#

clean:
	rm *.o $(PROJECT_LIBRARY_PATH)/$(PROJECT_LIBRARY)_$(PROJECT).*


#
# Static Library Creation
#

static:
	ar rcs $(PROJECT_LIBRARY_PATH)/$(PROJECT_LIBRARY)_$(PROJECT).a $(OBJECTS)


#
# Documentation Creation
#

DOXYGEN = doxygen

docs:
	$(DOXYGEN) doxygen.cfg