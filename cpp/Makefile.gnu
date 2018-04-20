
all:
	cd $(CPPDIR)/auxx; make -f Makefile.gnu
	cd $(CPPDIR)/numeric; make -f Makefile.gnu
	cd $(CPPDIR)/osix; make -f Makefile.gnu
	cd $(CPPDIR)/graphic/shapes; make -f Makefile.gnu
	cd $(CPPDIR)/graphic/charts/simpleplot; make -f Makefile.gnu
	cd $(CPPDIR)/spreadsheet; make -f Makefile.gnu
	cd $(CPPDIR)/calculator; make -f Makefile.gnu
	cd $(CPPDIR)/txtcalculator; make -f Makefile.gnu
	cd $(CPPDIR)/qt; make -f Makefile.gnu
	cd $(CPPDIR)/app; make -f Makefile.gnu
	cd $(CPPDIR)
clean:
	cd $(CPPDIR)/app; make -f Makefile.gnu clean
	cd $(CPPDIR)/qt; make -f Makefile.gnu clean
	cd $(CPPDIR)/txtcalculator; make -f Makefile.gnu clean
	cd $(CPPDIR)/calculator; make -f Makefile.gnu clean
	cd $(CPPDIR)/spreadsheet; make -f Makefile.gnu clean
	cd $(CPPDIR)/graphic/charts/simpleplot; make -f Makefile.gnu clean
	cd $(CPPDIR)/graphic/shapes; make -f Makefile.gnu clean
	cd $(CPPDIR)/osix; make -f Makefile.gnu clean
	cd $(CPPDIR)/numeric; make -f Makefile.gnu clean
	cd $(CPPDIR)/auxx; make -f Makefile.gnu clean
	cd $(CPPDIR)


