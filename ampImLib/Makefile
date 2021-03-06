# Author: Arvind Pereira

# Modify the includes, sources, headers, executable and
# other flags accordingly!
INCLUDES = -I ./ -I/usr/local/include $(OPENCV_INCS) $(ZXING_INCS)
LIBS = -L/usr/lib/ -L/usr/local/lib/ -lm -pthread -lpthread $(OPENCV_LIBS) $(ZXING_LIBS)

OPENCV_LIBS = $(shell pkg-config --libs opencv)
OPENCV_INCS = $(shell pkg-config --cflags opencv)

ZXING_LIBS = -L/usr/local/lib/ -lzxing -liconv
ZXING_INCS = -I../zxing-2.2/cpp/core/src/

# Compiler to be used
CC = g++
DEBUG = -g
LFLAGS = -Wall $(DEBUG)
CFLAGS = -Wall -c

# Definition of source and header files for first program
EXE1_SRCS = ampGaborLocator.cpp ampImageEnhancer.cpp
EXE1_HDRS = ampGaborLocator.h ampImageEnhancer.h OpenCVBitmapSource.h
EXE1_OBJS = $(EXE1_SRCS:.cpp=.o)


EXE2_SRCS = ampFastLocator.cpp ampImageEnhancer.cpp
EXE2_HDRS = ampFastLocator.h ampImageEnhancer.h OpenCVBitmapSource.h
EXE2_OBJS = $(EXE2_SRCS:.cpp=.o)

# Definition of source and header files for second program

# List of executables to be compiled
EXE1 = gabLocalize
EXE2 = fastLocalize

# Make executables
all: $(EXE1) $(EXE2)

#	if command -v doxygen; then make docs; \
#	fi

$(EXE1): $(EXE1_OBJS) $(EXE1_HDRS)
	$(CC) -o $(EXE1) $(EXE1_OBJS) $(LFLAGS) $(LIBS)    

$(EXE2): $(EXE2_OBJS) $(EXE2_HDRS)
	$(CC) -o $(EXE2) $(EXE2_OBJS) $(LFLAGS) $(LIBS)    

.cpp.o: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

#$(EXE1_OBJS): $(EXE1_SRCS) $(EXE1_HDRS)

# Make a tar file for archiving the code
tar:
	tar -czf $(TAR_FILE_NAME) $(SOURCES) $(HEADERS) Makefile

	TAR_FILE_NAME = `date +'%Y%m%d_%H%M%S_'`$(EXE1)$(EXE2).tar.gz

# Remove object files, executables and docs
clean:
	rm -f $(OBJ)
	rm -f $(EXE)
	rm -rf docs

OBJ = $(EXE1_OBJS) $(EXE2_OBJS)
EXE = $(EXE1) $(EXE2)

# All source and header files (Used for archival purposes)
SOURCES = $(EXE1_SRCS) $(EXE2_SRCS)
HEADERS = $(EXE1_HDRS) $(EXE2_HDRS)

# Creating documentation using Doxygen
docs: $(SOURCES) $(HEADERS)
	if command -v doxygen; then doxygen Doxyfile; \
	else echo "Doxygen not found. Not making documentation."; fi
