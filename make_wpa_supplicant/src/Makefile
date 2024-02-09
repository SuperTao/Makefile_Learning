SUBDIRS=common utils

all:
	for d in $(SUBDIRS); do [ -d $$d ] && $(MAKE) -C $$d; done

clean:
	$(Q)for d in $(SUBDIRS); do [ -d $$d ] && $(MAKE) -C $$d clean; done
	$(Q)rm -f *~

install:
	for d in $(SUBDIRS); do [ -d $$d ] && $(MAKE) -C $$d install; done
