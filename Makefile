#	$OpenBSD: Makefile,v 1.16 2017/07/10 21:30:37 espie Exp $
#
# C Shell with process control; VM/UNIX VAX Makefile
# Bill Joy UC Berkeley; Jim Kulp IIASA, Austria

PROG=	csh
CPPFLAGS=-I. -D_GNU_SOURCE -DBUILTIN -DSHORT_STRINGS -DEDIT $(shell pkg-config --cflags libbsd-overlay)
SRCS=	alloc.c char.c const.c csh.c dir.c dol.c error.c exec.c exp.c file.c \
	func.c glob.c hist.c init.c lex.c misc.c parse.c printf.c proc.c \
	sem.c set.c str.c time.c

CLEANFILES+=error.h const.h

csh: $(SRCS) $(CLEANFILES)
	$(CC) $(SRCS) -o csh $(CPPFLAGS) $(shell pkg-config --libs libbsd-overlay libedit) $(CFLAGS)

clean:
	$(RM) $(PROG) $(CLEANFILES)

error.h: $(SRCS)
	rm -f $@
	@echo '/* Do not edit this file, make creates it. */' > $@
	@echo '#ifndef _h_sh_err' >> $@
	@echo '#define _h_sh_err' >> $@
	egrep -h 'ERR_' $^ | egrep '^#define' >> $@
	@echo '#endif /* _h_sh_err */' >> $@

const.h: const.c
	@rm -f $@
	@echo '/* Do not edit this file, make creates it. */' > $@
	${CC} -E $(CPPFLAGS) $^ | egrep 'Char STR' | \
	    sed -e 's/Char \([a-zA-Z0-9_]*\)\(.*\)/extern Char \1[];/' | \
	    sort >> $@
