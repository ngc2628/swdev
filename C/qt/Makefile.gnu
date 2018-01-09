all: 
	cd $(CVER)/qt/util; make -f Makefile.gnu
	cd $(CVER)/qt/calculator; make -f Makefile.gnu
	cd $(CVER)/qt/txtcalculator; make -f Makefile.gnu
	cd $(CVER)/qt/spreadsheet; make -f Makefile.gnu
	cd $(CVER)/qt/simpleplot; make -f Makefile.gnu
	cd $(CVER)/qt
	ls -ltra $(LIBDIR)
clean:
	cd $(CVER)/qt/util; make -f Makefile.gnu clean
	cd $(CVER)/qt/calculator; make -f Makefile.gnu clean
	cd $(CVER)/qt/txtcalculator; make -f Makefile.gnu clean
	cd $(CVER)/qt/spreadsheet; make -f Makefile.gnu clean
	cd $(CVER)/qt/simpleplot; make -f Makefile.gnu clean
	cd $(CVER)/qt

