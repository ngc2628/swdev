
all:
	cd $(SWDIR)/c/tst/polyinter; make -f Makefile.gnu
	cd $(SWDIR)/c/tst/croutdoolittle.2d; make -f Makefile.gnu
	cd $(SWDIR)/c/tst/croutdoolittle.1d; make -f Makefile.gnu
	cd $(SWDIR)/c/tst/intersect; make -f Makefile.gnu
	cd $(SWDIR)/c/tst/vertices; make -f Makefile.gnu
	cd $(SWDIR)/c/tst/list; make -f Makefile.gnu
	cd $(SWDIR)/c/tst/arr; make -f Makefile.gnu
	cd $(SWDIR)/c/tst/string; make -f Makefile.gnu
	cd $(SWDIR)/c/tst
clean:
	cd $(SWDIR)/c/tst/string; make -f Makefile.gnu clean
	cd $(SWDIR)/c/tst/arr; make -f Makefile.gnu clean
	cd $(SWDIR)/c/tst/list; make -f Makefile.gnu clean
	cd $(SWDIR)/c/tst/vertices; make -f Makefile.gnu clean
	cd $(SWDIR)/c/tst/intersect; make -f Makefile.gnu clean
	cd $(SWDIR)/c/tst/croutdoolittle.2d; make -f Makefile.gnu clean
	cd $(SWDIR)/c/tst/croutdoolittle.1d; make -f Makefile.gnu clean
	cd $(SWDIR)/c/tst/polyinter; make -f Makefile.gnu clean
	cd $(SWDIR)/c/tst


