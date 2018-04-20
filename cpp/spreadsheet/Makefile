
PRJROOT       = d:\progs\C
PRJ           = spreadsheet
LIBDIR        = d:\lib
BINDIR        = d:\bin
DESTDIR       = $(LIBDIR)

WFLAGS        = 
LFLAGS        = /LIBPATH:"$(LIBDIR)"   
INCPATH       = -I"$(PRJROOT)"
LIBS          = libshapes.lib libnumeric.lib libosix.lib libauxx.lib
HEADER        = spreadsheetdata.h spreadsheet.h
SOURCES       = spreadsheetdata.cpp spreadsheet.cpp
OBJECTS       = spreadsheetdata.obj spreadsheet.obj
TARGET        = lib$(PRJ).dll

CC            = cl /D_CRT_SECURE_NO_WARNINGS /GR /FC /MP2 /Wall /c
CXX           = cl /D_CRT_SECURE_NO_WARNINGS /GR /FC /MP2 /Wall /c
LEX           = flex
YACC          = yacc
DEFINES       = 
CFLAGS        =  $(WFLAGS) $(DEFINES)
CXXFLAGS      =  $(WFLAGS) $(DEFINES)
LEXFLAGS      = 
LINK          = link /DLL
AR            = ar cq
RANLIB        = ranlib -s
COMPRESS      = zip 
RM            = del

all: $(TARGET)

.cpp.obj:
	$(CC) $(CFLAGS) $(INCPATH) $<

$(TARGET): $(OBJECTS)   
	$(RM) $(DESTDIR)\$(TARGET) 
	$(LINK) $(LFLAGS) /OUT:$(DESTDIR)\$(TARGET) $(OBJECTS) $(LIBS) 

clean: 
	$(RM) $(OBJECTS) $(DESTDIR)\$(TARGET)


