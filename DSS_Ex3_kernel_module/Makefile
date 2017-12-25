LK_SRCDIR       ?= /lib/modules/$(shell uname -r)/build
#LK_SRCDIR       ?= /usr/src/kernels/$(shell uname -r)
CURDIR          ?= $(pwd)

.PHONY: all clean
all :
	echo $(ARCH)
	$(MAKE) -C $(LK_SRCDIR) M=$(CURDIR) modules

clean :
	$(MAKE) -C $(LK_SRCDIR) M=$(CURDIR) clean
