AUTOMAKE_OPTIONS = foreign subdir-objects
CFLAGS=-Wall -I/usr/local/include -DVERSION=\"$(VERSION)\" -DPROG="\"$(PACKAGE)\"" 
LDFLAGS=-L/usr/local/lib -L/usr/lib -lz -lstdc++ -lboost_filesystem -lboost_system
# LDFLAGS=-L/usr/local/lib -L/usr/lib -lz -lstdc++ -lboost_filesystem-mgw47-mt-1_57 -lboost_system-mgw47-mt-1_57
CPPFLAGS=-Wall -I/usr/local/include -DNDEBUG
bin_PROGRAMS = waunzip
waunzip_SOURCES = waunzip.cc zip/unzip.c zip/utf8.c zip/ioapi.c
