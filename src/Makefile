Q=@

.SUFFIXES:

all: cutest check

cutest:
	git clone https://github.com/aiobofh/cutest.git

-include cutest/src/cutest.mk

blcheck: blcheck.c story.c
	$(CC) -o $@ $^ -D"call(func)=func" -O2

clean::
	$(Q)$(RM) -f *~ blcheck
