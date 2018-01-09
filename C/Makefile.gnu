
all:
	cd $(CVER)/auxx; make -f Makefile.gnu
	cd $(CVER)/numeric; make -f Makefile.gnu
	cd $(CVER)/osix; make -f Makefile.gnu
	cd $(CVER)/graphic/shapes; make -f Makefile.gnu
	cd $(CVER)/graphic/charts/simpleplot; make -f Makefile.gnu
	cd $(CVER)/spreadsheet; make -f Makefile.gnu
	cd $(CVER)/calculator; make -f Makefile.gnu
	cd $(CVER)/txtcalculator; make -f Makefile.gnu
	cd $(CVER)/qt; make -f Makefile.gnu
	cd $(CVER)/app; make -f Makefile.gnu
	cd $(CVER)
clean:
	cd $(CVER)/app; make -f Makefile.gnu clean
	cd $(CVER)/qt; make -f Makefile.gnu clean
	cd $(CVER)/txtcalculator; make -f Makefile.gnu clean
	cd $(CVER)/calculator; make -f Makefile.gnu clean
	cd $(CVER)/spreadsheet; make -f Makefile.gnu clean
	cd $(CVER)/graphic/charts/simpleplot; make -f Makefile.gnu clean
	cd $(CVER)/graphic/shapes; make -f Makefile.gnu clean
	cd $(CVER)/osix; make -f Makefile.gnu clean
	cd $(CVER)/numeric; make -f Makefile.gnu clean
	cd $(CVER)/auxx; make -f Makefile.gnu clean
	cd $(CVER)


