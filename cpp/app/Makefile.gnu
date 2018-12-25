all:
	cd $(SWDIR)/cpp/app/tst; make -f Makefile.gnu
ifdef QTDIR
	cd $(SWDIR)/cpp/app/qt; make -f Makefile.gnu
endif
	ls -ltr $(BINDIR)
	cd $(SWDIR)/cpp/app
clean:
ifdef QTDIR
	cd $(SWDIR)/cpp/app/qt; make -f Makefile.gnu clean
endif
	cd $(SWDIR)/cpp/app/tst; make -f Makefile.gnu clean
	cd $(SWDIR)/cpp/app


