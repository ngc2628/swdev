
####### definitions #######

PRJROOT       = $(SWDIR)
PRJ           = tstbcucof
DESTDIR       = $(BINDIR)
TARGET        = $(PRJ)
DEFINES       =
HEADER        = 
SOURCES       = main.cpp
LIBS          = -lshapes -lnumeric -losix -lauxx -lmkbase -lz -lm 
SOLN					= 

####### names and locations #######

OBJPRJ				= $(OBJDIR)/$(PRJ)
vpath					%.o $(OBJDIR)/$(PRJ)
vpath					%.cpp $(SWDIR)/cpp/$(PRJ)
OBJECTS       = $(patsubst %,$(OBJPRJ)/%,$(SOURCES:.cpp=.o))

####### compiler flags #######

WFLAGS1				= -Waddress -Warray-bounds -Wchar-subscripts -Wenum-compare -Wcomment -Wformat -Wmain  -Wmissing-braces -Wparentheses -Wreorder -Wreturn-type
WFLAGS2				= -Wsequence-point -Wsign-compare -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wvolatile-register-var -Wextra
WFLAGS3				= -Wunused-function -Wunused-label -Wunused-value -Wunused-variable
WFLAGS4				= -Wmaybe-uninitialized -Wc++11-compat -Wimplicit-int -Wimplicit-function-declaration -Wnonnull -Wpointer-sign
WFLAGS				= $(WFLAGS1) $(WFLAGS2)
CFLAGS        = -pipe -O2 -fno-strict-aliasing -std=c++11 $(WFLAGS) -W -fPIC $(DEFINES)
CXXFLAGS      = -pipe -O2 -fno-strict-aliasing -std=c++11 $(WFLAGS) -W -fPIC  $(DEFINES)
IFLAGS				= -I$(SWDIR)/cpp -I$(SWDIR)/c
LFLAGS				= -L$(LIBDIR) 
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

####### build rules #######

$(TARGET):  $(OBJECTS) 
	-$(RM) $(DESTDIR)/$(TARGET)
	$(LINK) $(LFLAGS) -o $(DESTDIR)/$(TARGET) $(OBJECTS) $(LIBS)

clean:
	$(RMDIR) $(OBJPRJ)
	$(RM) $(DESTDIR)/$(TARGET)
	$(RM) *~ core *.core

