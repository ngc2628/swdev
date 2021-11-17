all:
	cd $(SWDIR)/cpp/app/qt/calculatorapp; make -f Makefile.gnu
	cd $(SWDIR)/cpp/app/qt/simpleplotapp; make -f Makefile.gnu
	cd $(SWDIR)/cpp/app/qt/spreadsheetapp; make -f Makefile.gnu
	cd $(SWDIR)/cpp/app/qt/txtcalculatorapp; make -f Makefile.gnu
	cd $(SWDIR)/cpp/app/qt/qtexample; make -f Makefile.gnu
	cd $(SWDIR)/cpp/app/qt/wflag; make -f Makefile.gnu
	cd $(SWDIR)/cpp/app/qt
clean:
	cd $(SWDIR)/cpp/app/qt/wflag; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/app/qt/qtexample; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/app/qt/txtcalculatorapp; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/app/qt/spreadsheetapp; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/app/qt/simpleplotapp; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/app/qt/calculatorapp; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/app/qt


