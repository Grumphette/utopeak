CFLAGS=-O3 -fPIC

LIBPFM_VER=4.3.0
LIBPFM_BIN=libpfm-$(LIBPFM_VER)/lib/libpfm.so
LIBPFM_INC=libpfm-$(LIBPFM_VER)/include/
LIBPFM_LIBPATH=libpfm-$(LIBPFM_VER)/lib/

all: pfmcounters2.so ccyclescounters.so

pfmcounters2.so: pfmcounters2.c pfmcounters2.h $(LIBPFM_BIN)
	$(CC) $(CFLAGS) -shared -Wall -Wextra -fPIC pfmcounters2.c inih/ini.c -I$(LIBPFM_INC) -Iinih -o $@  -L$(LIBPFM_LIBPATH) -lpfm
	
ccyclescounters.so: ccyclescounters.c ccyclescounters.h $(LIBPFM_BIN)
	$(CC) $(CFLAGS) -shared -Wall -Wextra -fPIC ccyclescounters.c inih/ini.c -I$(LIBPFM_INC) -Iinih -o $@  -L$(LIBPFM_LIBPATH) -lpfm

$(LIBPFM_BIN):
	$(MAKE) -C libpfm-$(LIBPFM_VER)

clean:
	rm -f pfmcounters2.so
	rm -f ccyclescounters.so
	$(MAKE) -C libpfm-$(LIBPFM_VER) clean

install: all
	cp pfmcounters2.so /usr/lib/libpfmcounters2.so
	cp pfmcounters2.h /usr/include/libpfmcounters2.h
	cp ccyclescounters.so /usr/lib/libccyclescounters.so
	cp ccyclescounters.h /usr/include/libccyclescounters.h
	
uninstall:
	rm -f /usr/lib/libpfmcounters2.so
	rl -f /usr/include/libpfmcounters2.h
	rm -f /usr/lib/libccyclescounters.so
	rl -f /usr/include/libccyclescounters.h
