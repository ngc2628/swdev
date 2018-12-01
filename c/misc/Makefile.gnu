####### definitions #######

PRJROOT       = $(SWDIR)
PRJ           = misc
DESTDIR       = $(BINDIR)
TARGET        = misc
TARGET1       = rgb2int
TARGET2       = pun
DEFINES       =
HEADER        = 
SOURCES       = rgb2int.c pun.c
LIBS          = -lmkbase -lz -lm
SOLN					= 

####### names and locations #######

OBJPRJ				= $(OBJDIR)/$(PRJ)
vpath					%.o $(OBJDIR)/$(PRJ)
vpath					%.c $(SWDIR)/c/$(PRJ)
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
	-$(RM) $(DESTDIR)/$(TARGET1) 
	-$(RM) $(DESTDIR)/$(TARGET2) 
	$(LINK) $(LFLAGS) -o $(DESTDIR)/$(TARGET1) $(OBJPRJ)/$(TARGET1).o $(LIBS) 
	$(LINK) $(LFLAGS) -o $(DESTDIR)/$(TARGET2) $(OBJPRJ)/$(TARGET2).o $(LIBS) 

clean: 
	$(RMDIR) $(OBJPRJ)
	$(RM) $(DESTDIR)/$(TARGET1)
	$(RM) $(DESTDIR)/$(TARGET2)
	$(RM) *~ core *.core

