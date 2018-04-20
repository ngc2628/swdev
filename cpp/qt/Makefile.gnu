all: 
	cd $(CPPDIR)/qt/util; make -f Makefile.gnu
	cd $(CPPDIR)/qt/calculator; make -f Makefile.gnu
	cd $(CPPDIR)/qt/txtcalculator; make -f Makefile.gnu
	cd $(CPPDIR)/qt/spreadsheet; make -f Makefile.gnu
	cd $(CPPDIR)/qt/simpleplot; make -f Makefile.gnu
	cd $(CPPDIR)/qt
	ls -ltra $(LIBDIR)
clean:
	cd $(CPPDIR)/qt/util; make -f Makefile.gnu clean
	cd $(CPPDIR)/qt/calculator; make -f Makefile.gnu clean
	cd $(CPPDIR)/qt/txtcalculator; make -f Makefile.gnu clean
	cd $(CPPDIR)/qt/spreadsheet; make -f Makefile.gnu clean
	cd $(CPPDIR)/qt/simpleplot; make -f Makefile.gnu clean
	cd $(CPPDIR)/qt

