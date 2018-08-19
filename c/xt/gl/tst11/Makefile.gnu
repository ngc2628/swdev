####### definitions #######

PRJROOT       = $(SWDIR)
PRJ           = gltst11
DESTDIR       = $(BINDIR)
TARGET        = $(PRJ)
DEFINES       =
HEADER        = globals.h objects.h desktop.h edit.h menus.h glw.h lights.h form.h dialogs.h
SOURCES       = objects.c desktop.c dialogs.c edit.c menus.c glw.c lights.c form.c runloop.c main.c
LIBS          = -lmkbase -lGLU -lGL -lXm -lXext -lXt -lX11 -lz -lm
SOLN					= 

####### names and locations #######

OBJPRJ				= $(OBJDIR)/$(PRJ)
vpath					%.o $(OBJDIR)/$(PRJ)
vpath					%.c $(SWDIR)/c/$(PRJ)
OBJECTS       = $(patsubst %,$(OBJPRJ)/%,$(SOURCES:.c=.o))

####### compiler flags #######

WFLAGS1				= -Waddress -Warray-bounds -Wchar-subscripts -Wenum-compare -Wcomment -Wformat -Wmain  -Wmissing-braces -Wparentheses -Wreorder -Wreturn-type
WFLAGS2				= -Wsequence-point -Wsign-compare -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wvolatile-register-var -Wextra
WFLAGS3				= -Wunused-function -Wunused-label -Wunused-value -Wunused-variable 
WFLAGS4				= -Wmaybe-uninitialized -Wc++11-compat -Wimplicit-int -Wimplicit-function-declaration -Wnonnull -Wpointer-sign
WFLAGS				= $(WFLAGS1) $(WFLAGS2)
CFLAGS        = -pipe -O2 -fno-strict-aliasing $(WFLAGS) -W -fPIC $(DEFINES)
CXXFLAGS      = -pipe -O2 -fno-strict-aliasing $(WFLAGS) -W -fPIC  $(DEFINES)
IFLAGS				= -I$(SWDIR)/c
LFLAGS				= -L$(LIBDIR) 
LEXFLAGS      = 
YACCFLAGS     = -d

####### commands #######

CC            = gcc
CXX           = g++
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
