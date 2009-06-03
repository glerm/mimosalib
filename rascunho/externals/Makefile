current:
	echo make pd_linux, pd_win, or pd_darwin

clean: ; rm -f *.pd_linux *.o

install: ; cp *.pd_linux /usr/lib/pd/extra

# ----------------------- WINDOWS -----------------------

pd_win: roughness.dll 

.SUFFIXES: .dll

WINCFLAGS = -Wall -W -Wshadow -Wstrict-prototypes -DPD -DNT -W3 -WX -Wno-unused -mms-bitfields -Wno-parentheses -Wno-switch -O6 -funroll-loops -fomit-frame-pointer

#coloque aqui abaixo pasta do seu src do pd - windows only
WINPDPATH= 

WININCLUDE =  -I.. -I../include -I$(WINPDPATH)/src

LDFLAGS = -shared

.c.dll:
	gcc -mms-bitfields $(WINCFLAGS) $(WININCLUDE) -o $*.o -c $*.c
	gcc $(LDFLAGS) -o $*.dll $*.o $(WINPDPATH)/bin/pd.dll 	
	strip --strip-unneeded $*.dll
	rm -f $*.o
	


# ----------------------- LINUX i386 -----------------------

pd_linux: roughness.pd_linux helloworld.pd_linux

.SUFFIXES: .pd_linux

#pasta onde esta eu src do pd - serve tb pra mac
PDPATH = /home/glerm/puredata/

LINUXCFLAGS = -DPD -O2 -funroll-loops -fomit-frame-pointer \
    -Wall -W -Wshadow -Wstrict-prototypes  \
    -Wno-unused -Wno-parentheses -Wno-switch

LINUXINCLUDE =  -I$(PDPATH)/src

.c.pd_linux:
	cc $(LINUXCFLAGS) $(LINUXINCLUDE) -o $*.o -c $*.c
	ld -export_dynamic  -shared -o $*.pd_linux $*.o -lc -lm
	strip --strip-unneeded $*.pd_linux
	rm $*.o



# ----------------------- Mac OSX -----------------------

pd_darwin: roughness.pd_darwin 

.SUFFIXES: .pd_darwin

DARWINCFLAGS = -DPD -O2 -Wall -W -Wshadow -Wstrict-prototypes \
    -Wno-unused -Wno-parentheses -Wno-switch

.c.pd_darwin:
	cc $(DARWINCFLAGS) $(LINUXINCLUDE) -o $*.o -c $*.c
	cc -bundle -undefined suppress -flat_namespace -o $*.pd_darwin $*.o 
	rm -f $*.o

