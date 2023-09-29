
#environment

BINDIR 		:= $(if $(BINDIR),$(BINDIR),"/data/bin")
BINDIRTST := $(if $(BINDIRTST),$(BINDIRTST),"/data/bin/tst")
LIBDIR 		:= $(if $(LIBDIR),$(LIBDIR),"/data/lib")
OBJDIR 		:= $(if $(OBJDIR),$(OBJDIR),"/data/obj")
SWDIR 		:= $(if $(SWDIR),$(SWDIR),"/data/scm/swdev")

####### definitions #######

DEFINES    =
INCLUDES   =
PRJ        = tstcobloop
MODULE     = $(PRJ)m.so
TARGET     = $(PRJ)
SOURCE     = loop.cob
SOURCES    = sub1.cob sub2.cob

####### names and locations #######

NAMEM         = $(notdir $(MAKEFILE_LIST))
OBJPRJ	      = $(OBJDIR)/$(PRJ)
vpath	        %.o $(OBJDIR)/$(PRJ)
OBJECTS       = $(patsubst %,$(OBJPRJ)/%,$(SOURCES:.cob=.o))
PPATH	        = $(dir $(realpath $(MAKEFILE_LIST)))

####### compiler flags #######

export COB_CFLAGS  := $(WFLAGS) $(DEFINES) -O
export COB_IFLAGS  := -I$(SWDIR)/cobol/copy
COB_LDADD  	        = 
export COB_LDFLAGS := -L$(LIBDIR) -O -Wl,--no-as-needed
COB_LIBS            = -lcobutl
WFLAGS1		          = 
WFLAGS2		          = 
WFLAGS3		          = 
WFLAGS4		          = 
WFLAGS	            = $(WFLAGS1) $(WFLAGS2) $(WFLAGS3) $(WFLAGS4)

####### commands #######

COB_CC  = /usr/bin/cobc
COPY		= /usr/bin/cp -p -u
LINK   	= /usr/bin/cobc 
MAKEP		= /usr/bin/make -C
MKDIR		= /usr/bin/mkdir -p
RM      = /usr/bin/rm -f
RMDIR   = /usr/bin/rm -rf
SYMLINK = /usr/bin/ln -sf

####### targets #######

all:
	$(MAKEP) $(PPATH) allp	

allp: $(OBJPRJ) $(TARGET)

$(OBJPRJ):
	-$(MKDIR) $(OBJPRJ)

####### suffix and pattern rules #######

.SUFFIXES:

$(OBJPRJ)/%.o : %.cob
	$(COB_CC) -c $(COB_CFLAGS) -o "$@" "$<"

####### build rules #######

$(TARGET):  $(OBJECTS)
	-$(RM) $(BINDIRTST)/$(TARGET)
	-$(RM) $(LIBDIR)/$(MODULE)
	$(LINK) -b -o $(LIBDIR)/$(MODULE) $(OBJECTS) $(COB_LIBS)
	$(LINK) -xv -o $(BINDIRTST)/$(TARGET) $(SOURCE) $(LIBDIR)/$(MODULE) $(COB_LIBS)

clean:
	$(RMDIR) $(OBJPRJ)
	$(RM) $(BINDIRTST)/$(TARGET)
	$(RM) $(LIBDIR)/$(MODULE)

