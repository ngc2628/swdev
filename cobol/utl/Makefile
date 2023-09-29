
#environment

BINDIR := $(if $(BINDIR),$(BINDIR),/data/bin)
BINDIRTST := $(if $(BINDIRTST),$(BINDIRTST),/data/bin/tst)
LIBDIR := $(if $(LIBDIR),$(LIBDIR),/data/lib)
OBJDIR := $(if $(OBJDIR),$(OBJDIR),/data/obj)

####### definitions #######

DEFINES       =
INCLUDES      =
PRJ           = cobutl
SOURCES       = num53.cbl now-str.cbl
TARGET        = lib$(PRJ).so

####### names and locations #######

NAMEM         = $(notdir $(MAKEFILE_LIST))
OBJPRJ				= $(OBJDIR)/$(PRJ)
vpath					%.o $(OBJDIR)/$(PRJ)
OBJECTS       = $(patsubst %,$(OBJPRJ)/%,$(SOURCES:.cbl=.o))
PATHP         = $(dir $(realpath $(MAKEFILE_LIST)))

####### compiler flags #######

export COB_CFLAGS  := $(WFLAGS) $(DEFINES) -O
export COB_IFLAGS	 := -I$(SWDIR)/cobol/copy
COB_LDADD     			= 
export COB_LDFLAGS := -O -L$(LIBDIR) -Wl,--no-as-needed
COB_LIBS      			= 
WFLAGS1							= 
WFLAGS2							= 
WFLAGS3							= 
WFLAGS4							= 
WFLAGS							= $(WFLAGS1) $(WFLAGS2) $(WFLAGS3) $(WFLAGS4)

####### commands #######

COB_CC        = /usr/bin/cobc -v
LINK          = /usr/bin/cobc -v
MAKEP         = /usr/bin/make -C
RM            = /usr/bin/rm -f
RMDIR         = /usr/bin/rm -rf
SYMLINK       = /usr/bin/ln -sf
MKDIR					= /usr/bin/mkdir -p
COPY				  = /usr/bin/cp -p

####### targets #######

all: 
	$(MAKEP) $(PATHP) -f $(NAMEM) allp

allp: $(OBJPRJ) $(TARGET)

$(OBJPRJ):
	-$(MKDIR) $(OBJPRJ)

####### suffix and pattern rules #######

.SUFFIXES:

$(OBJPRJ)/%.o : %.cbl
	$(COB_CC) -c $(COB_CFLAGS) $(COB_IFLAGS) -o $@ $<

####### build rules #######

$(TARGET):  $(OBJECTS)
	-$(RM) $(LIBDIR)/$(TARGET)
	$(LINK) -b -o $(LIBDIR)/$(TARGET) $(OBJECTS)

clean:
	$(RMDIR) $(OBJPRJ)
	$(RM) $(LIBDIR)/$(TARGET)

