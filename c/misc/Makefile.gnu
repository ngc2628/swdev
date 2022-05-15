####### definitions #######

DEFINES       =
DESTDIR       = $(BINDIR)
HEADER        = 
LIBS          = -lmkbase -lz -lm
PRJ           = misc
PRJROOT       = $(SWDIR)
SOLN					= 
SOURCES       = rgb2int.c pun.c
TARGET        = misc
TARGET1       = rgb2int
TARGET2       = pun

####### names and locations #######

NAMEM         = $(notdir $(MAKEFILE_LIST))
OBJPRJ				= $(OBJDIR)/$(PRJ)
vpath					%.o $(OBJDIR)/$(PRJ)
vpath					%.c $(SWDIR)/c/$(PRJ)
OBJECTS       = $(patsubst %,$(OBJPRJ)/%,$(SOURCES:.c=.o))
PATHP         = $(dir $(realpath $(MAKEFILE_LIST)))

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
CXX           = /usr/bin/gcc
COMPRESS      = /usr/bin/gzip -9f
LINK          = /usr/bin/gcc $(SOLN)
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

$(OBJPRJ)/%.o : %.c
	$(CXX) -c $(CXXFLAGS) $(IFLAGS) -o "$@" "$<"

####### build rules #######

$(TARGET):  $(OBJECTS)   
	-$(RM) $(DESTDIR)/$(TARGET1) 
	-$(RM) $(DESTDIR)/$(TARGET2) 
	$(LINK) $(LFLAGS) -o $(DESTDIR)/$(TARGET1) $(OBJPRJ)/$(TARGET1).o $(LIBS) 
	$(LINK) $(LFLAGS) -o $(DESTDIR)/$(TARGET2) $(OBJPRJ)/$(TARGET2).o $(LIBS) 

clean: 
	$(RMDIR) $(OBJPRJ)
	$(RM) $(DESTDIR)/$(TARGET1)
	$(RM) $(DESTDIR)/$(TARGET2)
	$(RM) *~ core *.core

