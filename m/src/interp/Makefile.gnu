
PRJROOT       = $(MVER)
PRJ           = interp
HEADER        =
SOURCES       = xxspline.cc xxsplineinterpol.cc xxsplineeval.cc
DESTDIR       = $(PRJROOT)/interp

WFLAGS1 = -Waddress -Warray-bounds -Wchar-subscripts -Wenum-compare -Wcomment -Wformat -Wmain  -Wmissing-braces -Wparentheses -Wreorder -Wreturn-type
WFLAGS2 =  -Wsequence-point -Wsign-compare -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wvolatile-register-var -Wextra
WFLAGS3 = -Wunused-function -Wunused-label -Wunused-value -Wunused-variable
WFLAGS4 = -Wmaybe-uninitialized -Wc++11-compat -Wimplicit-int -Wimplicit-function-declaration -Wnonnull -Wpointer-sign
WFLAGS = $(WFLAGS1) $(WFLAGS2)
LFLAGS        = -L$(OCTAVEDIR)/lib -L$(LIBDIR)
INCPATH       = -I$(OCTAVEDIR)/include -I$(PRJROOT) -I$(CVER)
LIBS          = -lnumeric -lauxx -lz -lm
OBJECTS       = ${SOURCES:.cc=.oct}
TARGET        = xxoct

CC            = gcc
CXX           = g++
LEX           = flex
YACC          = yacc
DEFINES       =
CFLAGS        = -pipe -O2 -fno-strict-aliasing $(WFLAGS) -W -fPIC $(DEFINES)
CXXFLAGS      = -pipe -O2 -fno-strict-aliasing $(WFLAGS) -W -fPIC  $(DEFINES)
LEXFLAGS      =
YACCFLAGS     = -d
LINK          = g++ -shared
AR            = ar cq
RANLIB        = ranlib -s
TAR           = tar -cf
COMPRESS      = gzip -9f
MV            = mv -f
RMDIR         = rm -rf
RM            = rm -f
SYMLINK       = ln -sf
MKOCTFILE	  = $(OCTAVEDIR)/bin/mkoctfile

all: $(TARGET)


####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C .oct

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.oct:
	$(MKOCTFILE) $(INCPATH) $(LFLAGS) $(LIBS) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

$(TARGET):  $(OBJECTS)
	$(MV) $(OBJECTS) $(DESTDIR)
	$(RM) *.o

clean:
	$(RM) $(DESTDIR)/$(OBJECTS)
	$(RM) *~ core *.core


