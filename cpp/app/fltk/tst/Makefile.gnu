
####### definitions #######

DEFINES  = -D_FORTIFY_SOURCE=2 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT
DESTDIR  = $(BINDIR)
HEADER   = 
LIBS     = -lshapes -lnumeric -losix -ltools -lmkbase -lfltk -lXrender -lXfixes -lXext -lXft -lfontconfig -lXinerama -lpthread -ldl -lX11 -lm -lz
PRJ      = fltktst11
PRJROOT  = $(SWDIR)
SOLN	 = 
SOURCES  = main.cpp
TARGET   = $(PRJ)

####### names and locations #######

NAMEM					= $(notdir $(MAKEFILE_LIST))
OBJPRJ   = $(OBJDIR)/$(PRJ)
vpath    %.o $(OBJDIR)/$(PRJ)
vpath	 %.cpp $(SWDIR)/cpp/$(PRJ)
OBJECTS  = $(patsubst %,$(OBJPRJ)/%,$(SOURCES:.cpp=.o))
PATHP		      = $(dir $(realpath $(MAKEFILE_LIST)))

####### compiler flags #######

CFLAGS   = -fstack-clash-protection $(WFLAGS) -mtune=generic -O2 -pipe -W -fPIC $(DEFINES) -c
CXXFLAGS = -fvisibility-inlines-hidden $(CFLAGS)
IFLAGS	 = -I/usr/include/freetype2 -I$(SWDIR)/cpp -I$(SWDIR)/c
LFLAGS	 = -L$(LIBDIR) -Wl,-z,relro -Wl,-z,now -Wl,--as-needed
WFLAGS1  = -Waddress -Warray-bounds -Wchar-subscripts -Wenum-compare -Wcomment -Wformat -Wmain  -Wmissing-braces -Wparentheses -Wreorder -Wreturn-type
WFLAGS2	 = -Wsequence-point -Wsign-compare -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wvolatile-register-var -Wextra
WFLAGS3	 = -Wunused-function -Wunused-label -Wunused-value -Wunused-variable
WFLAGS4	 = -Wmaybe-uninitialized -Wc++11-compat -Wimplicit-int -Wimplicit-function-declaration -Wnonnull -Wpointer-sign
WFLAGS	 = $(WFLAGS1) $(WFLAGS2)

####### commands #######

AR            = /usr/bin/ar cq
CC            = /usr/bin/gcc
COMPRESS      = /usr/bin/gzip -9f
COPY				  = /usr/bin/cp -p -u
CXX           = /usr/bin/g++
LINK          = /usr/bin/g++ $(SOLN)
MAKEP					= /usr/bin/make -C
MKDIR					= /usr/bin/mkdir -p
RANLIB        = /usr/bin/ranlib -s
RM            = /usr/bin/rm -f
RMDIR         = /usr/bin/rm -rf
SYMLINK       = /usr/bin/ln -sf
TAR           = /usr/bin/tar -cf

####### targets #######

all: 
	$(MAKEP) $(PATHP) -f $(NAMEM) allp

allp: $(OBJPRJ) $(TARGET)

$(OBJPRJ):
	-$(MKDIR) $(OBJPRJ)

####### suffix and pattern rules #######

.SUFFIXES:

$(OBJPRJ)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(IFLAGS) -o "$@" "$<"

####### build rules #######

$(TARGET):  $(OBJECTS) 
	-$(RM) $(DESTDIR)/$(TARGET)
	$(LINK) $(LFLAGS) -o $(DESTDIR)/$(TARGET) $(OBJECTS) $(LIBS)

clean:
	$(RMDIR) $(OBJPRJ)
	$(RM) $(DESTDIR)/$(TARGET)

