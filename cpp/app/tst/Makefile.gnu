all:
	cd $(SWDIR)/cpp/app/tst/list; make -f Makefile.gnu
	cd $(SWDIR)/cpp/app/tst/mat; make -f Makefile.gnu
	cd $(SWDIR)/cpp/app/tst/polyinter; make -f Makefile.gnu
	cd $(SWDIR)/cpp/app/tst/thinkcell; make -f Makefile.gnu 
	ls -ltr $(BINDIR)
	cd $(SWDIR)/cpp/app/tst
clean:
	cd $(SWDIR)/cpp/app/tst/polyinter; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/app/tst/mat; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/app/tst/list; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/app/tst/thinkcell; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/app/tst


