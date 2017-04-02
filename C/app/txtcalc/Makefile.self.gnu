
PRJ           = txtcalc
HEADER        = 
SOURCES       = txtcalc.cpp main.self.cpp
PRJROOT       = .
LIBDIR        = .
BINDIR        = .
DESTDIR       = $(BINDIR)

WFLAGS1 = -Waddress -Warray-bounds -Wchar-subscripts -Wenum-compare -Wcomment -Wformat -Wmain  -Wmissing-braces -Wparentheses -Wreorder -Wreturn-type
WFLAGS2 =  -Wsequence-point -Wsign-compare -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wvolatile-register-var -Wextra
WFLAGS3 = -Wunused-function -Wunused-label -Wunused-value -Wunused-variable 
WFLAGS4 = -Wmaybe-uninitialized -Wc++11-compat -Wimplicit-int -Wimplicit-function-declaration -Wnonnull -Wpointer-sign
WFLAGS = $(WFLAGS1) $(WFLAGS2)
LFLAGS        = -L$(LIBDIR)  
INCPATH       = -I$(PRJROOT)
LIBS          =  -lz -lm 
OBJECTS       = ${SOURCES:.cpp=.o}
TARGET        = $(PRJ)

CC            = gcc
CXX           = g++
LEX           = flex
YACC          = yacc
DEFINES       = 
CFLAGS        = -pipe -O2 -fno-strict-aliasing $(WFLAGS) -W -fPIC $(DEFINES)
CXXFLAGS      = -pipe -O2 -fno-strict-aliasing $(WFLAGS) -W -fPIC  $(DEFINES)
LEXFLAGS      = 
YACCFLAGS     = -d
LINK          = g++
AR            = ar cq
RANLIB        = ranlib -s
TAR           = tar -cf
COMPRESS      = gzip -9f
RM            = rm -rf
SYMLINK       = ln -sf

all: $(TARGET)
	

####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

$(TARGET):  $(OBJECTS)   
	-$(RM) $(DESTDIR)/$(TARGET) 
	$(LINK) $(LFLAGS) -o $(DESTDIR)/$(TARGET) $(OBJECTS) $(LIBS) 

clean: 
	$(RM) $(OBJECTS) $(DESTDIR)/$(TARGET)
	$(RM) *~ core *.core



