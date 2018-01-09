all:
	cd $(CVER)/app/pun; make -f Makefile.gnu
	cd $(CVER)/app/qt/calculatorapp; make -f Makefile.gnu
	cd $(CVER)/app/qt/txtcalculatorapp; make -f Makefile.gnu
	cd $(CVER)/app/qt/spreadsheetapp; make -f Makefile.gnu
	cd $(CVER)/app/qt/simpleplotapp; make -f Makefile.gnu
	cd $(CVER)/app/rgb2int; make -f Makefile.gnu
	cd $(CVER)/app/txtcalc; make -f Makefile.gnu
	ls -ltr $(BINDIR)
	cd $(CVER)/app
clean:
	cd $(CVER)/app/pun; make -f Makefile.gnu clean
	cd $(CVER)/app/qt/txtcalculatorapp; make -f Makefile.gnu clean
	cd $(CVER)/app/qt/calculatorapp; make -f Makefile.gnu clean
	cd $(CVER)/app/qt/spreadsheetapp; make -f Makefile.gnu clean
	cd $(CVER)/app/qt/simpleplotapp; make -f Makefile.gnu clean
	cd $(CVER)/app/rgb2int; make -f Makefile.gnu clean
	cd $(CVER)/app/txtcalc; make -f Makefile.gnu clean
	cd $(CVER)/app


