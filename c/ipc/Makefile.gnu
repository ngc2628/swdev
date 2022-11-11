
####### definitions #######

DEFINES       =
DESTDIR       = $(LIBDIR)
HEADER        = ipcutl.h
LIBS          = -lz -lm
PRJ           = ipcutl
PRJROOT       = $(SWDIR)
SOLN					= -shared
SOURCES       = ipcutl.c
TARGET        = lib$(PRJ).so

####### names and locations #######

NAMEM					= $(notdir $(MAKEFILE_LIST))
OBJPRJ				= $(OBJDIR)/$(PRJ)
vpath					%.o $(OBJDIR)/$(PRJ)
vpath					%.cpp $(SWDIR)/c/$(PRJ)
OBJECTS       = $(patsubst %,$(OBJPRJ)/%,$(SOURCES:.c=.o))
PATHP		      = $(dir $(realpath $(MAKEFILE_LIST)))

####### compiler flags #######

CFLAGS        = -pipe -O2 -fno-strict-aliasing $(WFLAGS) -W -fPIC $(DEFINES)
CXXFLAGS      = -pipe -O2 -fno-strict-aliasing $(WFLAGS) -W -fPIC  $(DEFINES)
IFLAGS				= -I$(SWDIR)/c
LFLAGS				= -L$(LIBDIR)
WFLAGS1				= -Waddress -Warray-bounds -Wchar-subscripts -Wenum-compare -Wcomment -Wformat -Wmain  -Wmissing-braces -Wparentheses -Wreturn-type
WFLAGS2				= -Wsequence-point -Wsign-compare -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wvolatile-register-var -Wextra
WFLAGS3				= -Wunused-function -Wunused-label -Wunused-value -Wunused-variable
WFLAGS4				= -Wmaybe-uninitialized -Wimplicit-int -Wimplicit-function-declaration -Wnonnull -Wpointer-sign
WFLAGS				= $(WFLAGS1) $(WFLAGS2)

####### commands #######

AR            = /usr/bin/ar cq
CC            = /usr/bin/gcc
COMPRESS      = /usr/bin/gzip -9f
COPY				  = /usr/bin/cp -p -u
CXX           = /usr/bin/gcc
LINK          = /usr/bin/gcc $(SOLN)
MKDIR					= /usr/bin/mkdir -p
MAKEP					= /usr/bin/make -C
RANLIB        = /usr/bin/ranlib -s
RM            = /usr/bin/rm -f
RMDIR         = /usr/bin/rm -rf
SYMLINK       = /usr/bin/ln -sf
TAR           = /usr/bin/tar -cf

####### targets #######

all: 
	$(MAKEP) $(PATHP) -f $(NAMEM) allp

allp: $(OBJPRJ) $(TARGET)
	cd $(SWDIR)/c/ipc/server; make -f Makefile.gnu
	cd $(SWDIR)/c/ipc/client; make -f Makefile.gnu
	cd $(SWDIR)/c/ipc

$(OBJPRJ):
	-$(MKDIR) $(OBJPRJ)

####### suffix and pattern rules #######

.SUFFIXES:

$(OBJPRJ)/%.o : %.c
	$(CXX) -c $(CXXFLAGS) $(IFLAGS) -o "$@" "$<"

####### build rules #######

$(TARGET):  $(OBJECTS)
	-$(RM) $(DESTDIR)/$(TARGET)
	$(LINK) $(LFLAGS) -o $(DESTDIR)/$(TARGET) $(OBJECTS) $(LIBS)

clean:
	$(RMDIR) $(OBJPRJ) $(DESTDIR)/$(TARGET)
	cd $(SWDIR)/c/ipc/server; make -f Makefile.gnu clean
	cd $(SWDIR)/c/ipc/client; make -f Makefile.gnu clean
	cd $(SWDIR)/c/ipc


