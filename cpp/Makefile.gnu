
all:
	cd $(SWDIR)/cpp/auxx; make -f Makefile.gnu
	cd $(SWDIR)/cpp/numeric; make -f Makefile.gnu
	cd $(SWDIR)/cpp/osix; make -f Makefile.gnu
	cd $(SWDIR)/cpp/graphic/shapes; make -f Makefile.gnu
	cd $(SWDIR)/cpp/graphic/charts/simpleplot; make -f Makefile.gnu
	cd $(SWDIR)/cpp/spreadsheet; make -f Makefile.gnu
	cd $(SWDIR)/cpp/calculator; make -f Makefile.gnu
	cd $(SWDIR)/cpp/txtcalculator; make -f Makefile.gnu
ifdef QTDIR
	cd $(SWDIR)/cpp/qt; make -f Makefile.gnu
endif
	cd $(SWDIR)/cpp/app; make -f Makefile.gnu
	cd $(SWDIR)/cpp
clean:
	cd $(SWDIR)/cpp/app; make -f Makefile.gnu clean
ifdef QTDIR
	cd $(SWDIR)/cpp/qt; make -f Makefile.gnu clean
endif
	cd $(SWDIR)/cpp/txtcalculator; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/calculator; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/spreadsheet; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/graphic/charts/simpleplot; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/graphic/shapes; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/osix; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/numeric; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/auxx; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp
  
