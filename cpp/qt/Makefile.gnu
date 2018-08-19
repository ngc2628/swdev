all:
	cd $(SWDIR)/cpp/qt/util; make -f Makefile.gnu
	cd $(SWDIR)/cpp/qt/calculator; make -f Makefile.gnu
	cd $(SWDIR)/cpp/qt/txtcalculator; make -f Makefile.gnu
	cd $(SWDIR)/cpp/qt/spreadsheet; make -f Makefile.gnu
	cd $(SWDIR)/cpp/qt/simpleplot; make -f Makefile.gnu
	cd $(SWDIR)/cpp/qt
clean:
	cd $(SWDIR)/cpp/qt/simpleplot; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/qt/spreadsheet; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/qt/txtcalculator; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/qt/calculator; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/qt/util; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/qt

