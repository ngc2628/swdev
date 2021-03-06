
####### definitions #######

PRJROOT       = $(SWDIR)
PRJ           = ipcutl
DESTDIR       = $(LIBDIR)
TARGET        = lib$(PRJ).so
DEFINES       =
HEADER        = ipcutl.h
SOURCES       = ipcutl.c
LIBS          = -lz -lm
SOLN					= -shared

####### names and locations #######

OBJPRJ				= $(OBJDIR)/$(PRJ)
vpath					%.o $(OBJDIR)/$(PRJ)
vpath					%.cpp $(SWDIR)/c/$(PRJ)
OBJECTS       = $(patsubst %,$(OBJPRJ)/%,$(SOURCES:.c=.o))

####### compiler flags #######

WFLAGS1				= -Waddress -Warray-bounds -Wchar-subscripts -Wenum-compare -Wcomment -Wformat -Wmain  -Wmissing-braces -Wparentheses -Wreturn-type
WFLAGS2				= -Wsequence-point -Wsign-compare -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wvolatile-register-var -Wextra
WFLAGS3				= -Wunused-function -Wunused-label -Wunused-value -Wunused-variable
WFLAGS4				= -Wmaybe-uninitialized -Wimplicit-int -Wimplicit-function-declaration -Wnonnull -Wpointer-sign
WFLAGS				= $(WFLAGS1) $(WFLAGS2)
CFLAGS        = -pipe -O2 -fno-strict-aliasing $(WFLAGS) -W -fPIC $(DEFINES)
CXXFLAGS      = -pipe -O2 -fno-strict-aliasing $(WFLAGS) -W -fPIC  $(DEFINES)
IFLAGS				= -I$(SWDIR)/c
LFLAGS				= -L$(LIBDIR)
LEXFLAGS      =
YACCFLAGS     = -d

####### commands #######

CC            = gcc
CXX           = gcc
LEX           = flex
YACC          = yacc
LINK          = gcc $(SOLN)
AR            = ar cq
RANLIB        = ranlib -s
TAR           = tar -cf
COMPRESS      = gzip -9f
RM            = rm -rf
SYMLINK       = ln -sf
MKDIR					= mkdir -p

####### targets #######

all: $(OBJPRJ) $(TARGET)
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
	$(RM) $(OBJPRJ) $(DESTDIR)/$(TARGET)
	$(RM) *~ core *.core
	cd $(SWDIR)/c/ipc/server; make -f Makefile.gnu clean
	cd $(SWDIR)/c/ipc/client; make -f Makefile.gnu clean
	cd $(SWDIR)/c/ipc


