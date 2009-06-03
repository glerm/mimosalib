current:
	echo make pd_linux, pd_win, or pd_darwin

clean: ; rm -f *.pd_linux *.o

install: ; cp *.pd_linux /usr/lib/pd/extra


# ----------------------- LINUX i386 -----------------------

pd_linux: glermcounter.pd_linux

.SUFFIXES: .pd_linux

#pasta onde esta eu src do pd - serve tb pra mac
PDPATH = /home/glerm/puredata/

LINUXCFLAGS = -DPD -O2 -funroll-loops -fomit-frame-pointer \
    -Wall -W -Wshadow -Wstrict-prototypes  \
    -Wno-unused -Wno-parentheses -Wno-switch

LINUXINCLUDE =  -I$(PDPATH)/src

.c.pd_linux:
	gcc $(LINUXCFLAGS) $(LINUXINCLUDE) -o $*.o -c $*.c
	ld -export_dynamic  -shared -o $*.pd_linux $*.o -lc -lm
	strip --strip-unneeded $*.pd_linux
	rm $*.o



