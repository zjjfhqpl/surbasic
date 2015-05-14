EXTRA_DIST = borZoi.dsp borZoi.plg borZoi.vcproj borZoi_test.sln borZoi_test.vcproj  borZoi_test.dsw borZoi_test.dsp borZoi_test.ncb borZoi_test.plg

SUBDIRS = docs src

clean:
	rm -rf src/borzoi.exe

release:
	rm -rf .deps */.deps
	$(MAKE) distcheck

sanity:
	./sanity_check $(VERSION)

snapshot:
	$(MAKE) dist distdir=$(PACKAGE)`date +"%y%m%d"`

zip: distdir
	zip -r $(distdir).zip $(distdir)
	-rm -rf $(distdir)
