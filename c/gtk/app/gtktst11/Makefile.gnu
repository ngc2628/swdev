
####### definitions #######

PRJROOT       = $(SWDIR)
PRJ           = gtktst11
DESTDIR       = $(BINDIRTST)
TARGET        = $(PRJ)
DEFINES       =
HEADER        = cb.h
SOURCES       = cb.c main.c
LIBS          = -lgtk-3 -lgdk-3 -lgdk_pixbuf-2.0 -lgobject-2.0 -lglib-2.0 -lz -lm 
SOLN					=

####### names and locations #######

OBJPRJ				= $(OBJDIR)/$(PRJ)
vpath					%.o $(OBJDIR)/$(PRJ)
vpath					%.c $(SWDIR)/c/gtk/app/$(PRJ)
OBJECTS       = $(patsubst %,$(OBJPRJ)/%,$(SOURCES:.c=.o))

####### compiler flags #######

WFLAGS1				= -Waddress -Warray-bounds -Wchar-subscripts -Wenum-compare -Wcomment -Wformat -Wmain  -Wmissing-braces -Wparentheses -Wreturn-type
WFLAGS2				= -Wsequence-point -Wsign-compare -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wvolatile-register-var -Wextra
WFLAGS3				= -Wunused-function -Wunused-label -Wunused-value -Wunused-variable
WFLAGS4				= -Wmaybe-uninitialized -Wimplicit-int -Wimplicit-function-declaration -Wnonnull -Wpointer-sign
WFLAGS				= $(WFLAGS1) $(WFLAGS2)
CFLAGS        = -pipe -g -fno-strict-aliasing $(WFLAGS) -W -fPIC $(DEFINES)
CXXFLAGS      = -pipe -g -fno-strict-aliasing $(WFLAGS) -W -fPIC  $(DEFINES)
IFLAGS1       = -I/usr/include/gtk-3.0 -I/usr/include/freetype2 -I/usr/include/gdk-pixbuf-2.0
IFLAGS2       = -I/usr/include/pango-1.0 -I/usr/include/cairo -I/usr/include/harfbuzz -I/usr/include/atk-1.0
IFLAGS3       = -I/usr/lib64/glib-2.0/include -I/usr/include/glib-2.0
IFLAGS4       = -I/data/include
IFLAGS				= -I$(SWDIR)/c $(IFLAGS1) $(IFLAGS2) $(IFLAGS3) $(IFLAGS4)      
LFLAGS				= -L$(LIBDIR)
LEXFLAGS      =
YACCFLAGS     = -d

####### commands #######

CC            = gcc
CXX           = gcc
LEX           = flex
YACC          = yacc
DEFINES       =
LINK          = gcc $(SOLN)
AR            = ar cq
RANLIB        = ranlib -s
TAR           = tar -cf
COMPRESS      = gzip -9f
RMDIR         = rm -rf
RM            = rm -f
SYMLINK       = ln -sf
MKDIR					= mkdir -p

####### targets #######

all: $(OBJPRJ) $(TARGET)

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
	$(RMDIR) $(OBJPRJ)
	$(RM) $(DESTDIR)/$(TARGET)
	$(RM) *~ core *.core

