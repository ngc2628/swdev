
all:
	cd $(SWDIR)/c/tst/polyinter; make -f Makefile.gnu
	cd $(SWDIR)/c/tst/croutdoolittle; make -f Makefile.gnu
	cd $(SWDIR)/c/tst/intersect; make -f Makefile.gnu
	cd $(SWDIR)/c/tst
clean:
	cd $(SWDIR)/c/tst/intersect; make -f Makefile.gnu clean
	cd $(SWDIR)/c/tst/croutdoolittle; make -f Makefile.gnu clean
	cd $(SWDIR)/c/tst/polyinter; make -f Makefile.gnu clean
	cd $(SWDIR)/c/tst


