all:
	cd $(SWDIR)/cpp/app/tst; make -f Makefile.gnu
	cd $(SWDIR)/cpp/app/qt; make -f Makefile.gnu
	ls -ltr $(BINDIR)
	cd $(SWDIR)/cpp/app
clean:
	cd $(SWDIR)/cpp/app/qt; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/app/tst; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/app


