
all:
	cd $(SWDIR)/c/xt/xcb/tst11 ; make -f Makefile.gnu
	cd $(SWDIR)/c/xt/xm3 ; make -f Makefile.gnu
	cd $(SWDIR)/c/xt/gl/tst11 ; make -f Makefile.gnu
	cd $(SWDIR)/c
clean:
	cd $(SWDIR)/c/xt/gl/tst11 ; make -f Makefile.gnu clean
	cd $(SWDIR)/c/xt/xm3 ; make -f Makefile.gnu clean
	cd $(SWDIR)/c/xt/xcb/tst11 ; make -f Makefile.gnu clean
	cd $(SWDIR)/c


