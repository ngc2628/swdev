
####### definitions #######

PRJ           = mkbase
#DESTDIR       = $(SWDIR)/c/$(PRJ)
DESTDIR       = $(LIBDIR)
TARGET        = lib$(PRJ).so
DEFINES       =
HEADER        = defs.h exportdefs.h mkutil.h mkmath.h mkconv.h mkla.h  mkana.h
SOURCES       = mkutil.c mkmath.c mkconv.c mkla.c mkana.c
LIBS          = -lz -lm
SOLN		  = -shared

####### names and locations #######

OBJPRJ				= $(OBJDIR)/$(PRJ)
vpath					%.o $(OBJDIR)/$(PRJ)
vpath					%.c $(DESTDIR)
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
RM            = rm -f
RMDIR         = rm -rf
SYMLINK       = ln -sf
MKDIR					= mkdir -p
COPY				  = cp -p -u

####### targets #######

all: $(OBJPRJ) $(TARGET)

$(OBJPRJ):
	-$(MKDIR) $(OBJPRJ)

install: all
	-$(MKDIR) $(INCDIR)/$(PRJ)
	$(COPY) $(HEADER) $(INCDIR)/$(PRJ)

uninstall:
	$(RMDIR) $(INCDIR)/$(PRJ)

####### suffix and pattern rules #######

.SUFFIXES:

$(OBJPRJ)/%.o : %.c
	$(CC) -c $(CFLAGS) $(IFLAGS) -o "$@" "$<"

####### build rules #######

$(TARGET):  $(OBJECTS)
	-$(RM) $(DESTDIR)/$(TARGET)
	$(LINK) $(LFLAGS) -o $(DESTDIR)/$(TARGET) $(OBJECTS) $(LIBS)

clean:
	$(RMDIR) $(OBJPRJ)
	$(RM) $(DESTDIR)/$(TARGET)
	$(RM) *~ core *.core

