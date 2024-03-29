
# environment

BINDIR    := $(if $(BINDIR),$(BINDIR),"/data/bin")
BINDIRTST := $(if $(BINDIRTST),$(BINDIRTST),"/data/bin/tst")
LIBDIR    := $(if $(LIBDIR),$(LIBDIR),"/data/lib")
OBJDIR    := $(if $(OBJDIR),$(OBJDIR),"/data/obj")
SWDIR     := $(if $(SWDIR),$(SWDIR),"/data/scm/swdev")

####### definitions #######

DEFINES       =
INCLUDES      =
MODULE        = $(PRJ)m.so
PRJ           = tstcobcmdline
SOURCE        = cmdline.cbl
SOURCES       = sub1.cbl sub2.cbl
TARGET        = $(PRJ)

####### names and locations #######

NAMEM         = $(notdir $(MAKEFILE_LIST))
vpath         %.o $(OBJDIR)/$(PRJ)
OBJPRJ        = $(OBJDIR)/$(PRJ)
OBJECTS       = $(patsubst %,$(OBJPRJ)/%,$(SOURCES:.cbl=.o))
PPATH         = $(dir $(realpath $(MAKEFILE_LIST)))

####### compiler flags #######

export COB_CFLAGS  := $(WFLAGS) $(DEFINES) -O
export COB_IFLAGS  := -I$(SWDIR)/cobol/copy
COB_LDADD           =
export COB_LDFLAGS := -L$(LIBDIR) -O -Wl,--no-as-needed
COB_LIBS            = -lcobutl
WFLAGS1				      = 
WFLAGS2				      = 
WFLAGS3				      = 
WFLAGS4				      = 
WFLAGS				      = $(WFLAGS1) $(WFLAGS2) $(WFLAGS3) $(WFLAGS4)

####### commands #######

COB_CC        = /usr/bin/cobc
COPY		      = /usr/bin/cp -p -u
LINK          = /usr/bin/cobc 
MAKEP		      = /usr/bin/make -C
MKDIR		      = /usr/bin/mkdir -p
RM            = /usr/bin/rm -f
RMDIR         = /usr/bin/rm -rf
SYMLINK       = /usr/bin/ln -sf

####### targets #######

all:
	$(MAKEP) $(PPATH) allp	

allp: $(OBJPRJ) $(TARGET)

$(OBJPRJ):
	-$(MKDIR) $(OBJPRJ)

####### suffix and pattern rules #######

.SUFFIXES:

$(OBJPRJ)/%.o : %.cbl
	$(COB_CC) -c $(COB_IFLAGS) $(COB_CFLAGS) -o "$@" "$<"

####### build rules #######

$(TARGET):  $(OBJECTS)
	-$(RM) $(BINDIRTST)/$(TARGET)
	-$(RM) $(LIBDIR)/$(MODULE)
	$(LINK) -b $(COB_IFLAGS) -o $(LIBDIR)/$(MODULE) $(OBJECTS) $(COB_LIBS)
	$(LINK) -x -vv $(COB_IFLAGS) -o $(BINDIRTST)/$(TARGET) $(SOURCE) $(LIBDIR)/$(MODULE) $(COB_LIBS)

clean:
	$(RMDIR) $(OBJPRJ)
	$(RM) $(BINDIRTST)/$(TARGET)
	$(RM) $(LIBDIR)/$(MODULE)

