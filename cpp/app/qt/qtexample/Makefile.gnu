
####### definitions #######

PRJROOT       = $(SWDIR)
PRJ           = qtexample
DESTDIR       = $(BINDIR)
TARGET        = $(PRJ)
DEFINES       =
SOURCES       = dialog.cpp mainwindow.cpp main.cpp
HEADERS       = dialog.h mainwindow.h
MOCS          = dialog.h mainwindow.h
LIBS          = -lQt5X11Extras -lQt5Widgets -lQt5Gui -lQt5Core -lmkbase -lxcb -lz -lm 
SOLN					= 

####### names and locations #######

OBJPRJ				= $(OBJDIR)/$(PRJ)
vpath					%_moc.cpp $(OBJDIR)/$(PRJ)
vpath					%.o $(OBJDIR)/$(PRJ)
vpath					%.cpp $(SWDIR)/cpp/$(PRJ)
OBJECTS       = $(patsubst %,$(OBJPRJ)/%,$(SOURCES:.cpp=.o))
SOURCES_MOC   = $(patsubst %,$(OBJPRJ)/%,$(MOCS:.h=_moc.cpp))
OBJECTS_MOC   = $(SOURCES_MOC:.cpp=.o)

####### compiler flags #######

WFLAGS1				= -Waddress -Warray-bounds -Wchar-subscripts -Wenum-compare -Wcomment -Wformat -Wmain  -Wmissing-braces -Wparentheses -Wreorder -Wreturn-type
WFLAGS2				= -Wsequence-point -Wsign-compare -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wvolatile-register-var -Wextra
WFLAGS3				= -Wunused-function -Wunused-label -Wunused-value -Wunused-variable
WFLAGS4				= -Wmaybe-uninitialized -Wc++11-compat -Wimplicit-int -Wimplicit-function-declaration -Wnonnull -Wpointer-sign
#WFLAGS				= $(WFLAGS1) $(WFLAGS2)
WFLAGS				= 
CFLAGS        = -pipe -g -fno-strict-aliasing -std=c++11 $(WFLAGS) -W -fPIC $(DEFINES)
CXXFLAGS      = -pipe -g -fno-strict-aliasing -std=c++11 $(WFLAGS) -W -fPIC  $(DEFINES)
IFLAGS				= -I$(SWDIR)/cpp -I$(SWDIR)/c -I$(QTDIR)/include -I$(QTDIR)/include/QtCore -I$(QTDIR)/include/QtWidgets -I$(QTDIR)/include/QtX11Extras
LFLAGS				= -L$(LIBDIR) -L$(QTDIR)/lib
LEXFLAGS      =
YACCFLAGS     = -d

####### commands #######

CC            = gcc
CXX           = g++
LEX           = flex
YACC          = yacc
DEFINES       = -DQT_SHARED -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB
LINK          = g++ $(SOLN)
AR            = ar cq
RANLIB        = ranlib -s
TAR           = tar -cf
COMPRESS      = gzip -9f
RMDIR         = rm -rf
RM            = rm -f
SYMLINK       = ln -sf
MKDIR					= mkdir -p
MOC           = $(QTDIR)/bin/moc $(DEFINES)

####### targets #######

all: $(OBJPRJ) $(TARGET)

$(OBJPRJ):
	-$(MKDIR) $(OBJPRJ)

####### suffix and pattern rules #######

.SUFFIXES:

$(OBJPRJ)/%_moc.o : %_moc.cpp
	$(CXX) -c $(CXXFLAGS) $(IFLAGS) -o "$@" "$<"

$(OBJPRJ)/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $(IFLAGS) -o "$@" "$<"

$(OBJPRJ)/%_moc.cpp : %.h
	$(MOC) "$<" -o "$@"

####### build rules #######

$(TARGET):  $(OBJECTS) $(OBJECTS_MOC)
	-$(RM) $(DESTDIR)/$(TARGET)
	$(LINK) $(LFLAGS) -o $(DESTDIR)/$(TARGET) $(OBJECTS) $(OBJECTS_MOC) $(LIBS)

clean:
	$(RM) $(DESTDIR)/$(TARGET)
	$(RMDIR) $(OBJPRJ)
	$(RM) *~ core *.core

.SECONDARY: $(SOURCES_MOC)
