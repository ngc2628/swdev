
all:
	cd $(SWDIR)/c/mkbase; make -f Makefile.gnu
	cd $(SWDIR)/c/txtcalc; make -f Makefile.gnu
	cd $(SWDIR)/c/misc; make -f Makefile.gnu
	cd $(SWDIR)/c/xt; make -f Makefile.gnu
	cd $(SWDIR)/c
clean:
	cd $(SWDIR)/c/xt; make -f Makefile.gnu clean
	cd $(SWDIR)/c/misc; make -f Makefile.gnu clean
	cd $(SWDIR)/c/txtcalc; make -f Makefile.gnu clean
	cd $(SWDIR)/c/mkbase; make -f Makefile.gnu clean
	cd $(SWDIR)/c


