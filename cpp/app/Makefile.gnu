all:
	cd $(CPPDIR)/app/pun; make -f Makefile.gnu
	cd $(CPPDIR)/app/qt/calculatorapp; make -f Makefile.gnu
	cd $(CPPDIR)/app/qt/txtcalculatorapp; make -f Makefile.gnu
	cd $(CPPDIR)/app/qt/spreadsheetapp; make -f Makefile.gnu
	cd $(CPPDIR)/app/qt/simpleplotapp; make -f Makefile.gnu
	cd $(CPPDIR)/app/rgb2int; make -f Makefile.gnu
	cd $(CPPDIR)/app/txtcalc; make -f Makefile.gnu
	ls -ltr $(BINDIR)
	cd $(CPPDIR)/app
clean:
	cd $(CPPDIR)/app/pun; make -f Makefile.gnu clean
	cd $(CPPDIR)/app/qt/txtcalculatorapp; make -f Makefile.gnu clean
	cd $(CPPDIR)/app/qt/calculatorapp; make -f Makefile.gnu clean
	cd $(CPPDIR)/app/qt/spreadsheetapp; make -f Makefile.gnu clean
	cd $(CPPDIR)/app/qt/simpleplotapp; make -f Makefile.gnu clean
	cd $(CPPDIR)/app/rgb2int; make -f Makefile.gnu clean
	cd $(CPPDIR)/app/txtcalc; make -f Makefile.gnu clean
	cd $(CPPDIR)/app


