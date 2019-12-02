IDIR = .
CC = gcc
CFLAGS = -I$(IDIR)

PLATFORM = linux

ODIR=/cygdrive/c/temp
LDIR =

LIBS=

_DEPS = devapi.h dm_types.h ospl.h devapi_packer.h dev_agent_log.h dev_agent_conf.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#_OBJ = dm.o dm_$(DM).o ospl_$(PLATFORM).o dev_agent_backend.o dm_test_main.o

_AGT_OBJ = dm.o ospl_$(PLATFORM).o devapi_packer.o dev_agent_backend.o dev_agent.o
AGT_OBJ = $(patsubst %,$(ODIR)/%,$(_AGT_OBJ))

_ADM10_OBJ = dm_adm10.o dm_link_adm10.o
ADM10_OBJ = $(patsubst %,$(ODIR)/%,$(_ADM10_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $(CFLAGS) $<

agent_adm10: $(AGT_OBJ) $(ADM10_OBJ)
	$(CC) -g -o $(ODIR)/$@.exe $(CFLAGS) $(LIBS) $^

.PHONY: clean

clean:
	rm $(AGT_OBJ) $(ADM10_OBJ)
