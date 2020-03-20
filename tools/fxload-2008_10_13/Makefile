# $Id: Makefile,v 1.6 2002/04/12 00:31:00 dbrownell Exp $
#
# Builds "fxload" binaries, distribution tarball, and RPMs.
#

prefix =		/
exec_prefix =		${prefix}
sbindir =		${exec_prefix}/sbin
mandir =		${prefix}/usr/share/man

INSTALL =		/usr/bin/install -cD
INSTALL_PROGRAM =	${INSTALL}

PROG = 			fxload

CFLAGS =		-O -Wall $(RPM_OPT_FLAGS)

FILES_SRC_C =		ezusb.c main.c
FILES_SRC_H =		ezusb.h
FILES_SRC_OTHER =	README.txt COPYING Makefile fxload.8 a3load.hex
FILES_SRC =		$(FILES_SRC_OTHER) $(FILES_SRC_H) $(FILES_SRC_C)

FILES_OBJ =		$(FILES_SRC_C:%.c=%.o)

REV =			$(shell date "+%Y_%m_%d"| awk '{print $$1}')
RELEASE_NAME =		$(PROG)-$(REV)



# the interesting targets
# NOTE:  the default build ("make all") labels itself as a
# development build ("fxload -V" output)
all: $(PROG)

release:	rpms
	@echo FILES FOR RELEASE $(RELEASE_NAME)
	@find * -name '*.rpm' -o -name '*.gz' | grep $(RELEASE_NAME)


# object files
$(PROG): $(FILES_OBJ)
	$(CC) -o $(PROG) $(FILES_OBJ)

%.o: %.c
	$(CC) -c $(CFLAGS)  $< -o $@
main.o: main.c ezusb.h
ezusb.o: ezusb.c ezusb.h


# different degrees of clean ...
#	FIXME:  shouldn't assume only x86 RPMs get built
mrproper:	clean
	rm -f $(PROG)-*.tar.gz
	rm -f  $(PROG)-*.spec $(PROG)-*.src.rpm
	rm -rf i386 $(PROG)-* build
clean:
	rm -f Log *.o *~ $(PROG)


# install, from tarball or for binary RPM
install: $(PROG)
	$(INSTALL_PROGRAM) $(PROG) $(sbindir)/$(PROG)
	$(INSTALL_PROGRAM) -m 0644 $(PROG).8 $(mandir)/man8/$(PROG).8
	$(INSTALL_PROGRAM) -m 0644 a3load.hex $(prefix)/usr/share/usb/a3load.hex


# make a source tarball
tarball: $(RELEASE_NAME).tar.gz

$(RELEASE_NAME).tar.gz: clean
	@rm -rf $(RELEASE_NAME)
	@mkdir $(RELEASE_NAME)
	@for file in $(FILES_SRC); do			\
		cp $$file $(RELEASE_NAME)/$$file;	\
		chmod 0644 $(RELEASE_NAME)/$$file;	\
		done
	@tar cfz $(RELEASE_NAME).tar.gz $(RELEASE_NAME)
	@rm -rf $(RELEASE_NAME)
	@echo "TARBALL:  $(RELEASE_NAME).tar.gz"

# make source and binary RPMs
# you don't need to be root to do this, just to install
rpms:	tarball
	@rm -rf $(RELEASE_NAME).spec build $(RELEASE_NAME)
	@sed "s/VERCODE/$(REV)/" < $(PROG).spec > $(RELEASE_NAME).spec
	@mkdir build
	rpm --define "_srcrpmdir $(PWD)" --define "_rpmdir $(PWD)" \
		--define "_sourcedir $(PWD)" \
		--define "_builddir $(PWD)/build" \
		-ba --clean --buildroot=$(PWD)/$(RELEASE_NAME) \
		$(RELEASE_NAME).spec
	@rm -rf $(RELEASE_NAME).spec build $(RELEASE_NAME)
	@echo ''

