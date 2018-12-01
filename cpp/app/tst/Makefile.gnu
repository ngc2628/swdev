all:
	cd $(SWDIR)/cpp/app/tst/bcucof; make -f Makefile.gnu
	ls -ltr $(BINDIR)
	cd $(SWDIR)/cpp/app/tst
clean:
	cd $(SWDIR)/cpp/app/tst/bcucof; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/app/tst


