Q=@

.SUFFIXES:

all:
	$(Q)make -s cutest && make -s check && make -s blcheck

cutest:
	$(Q)git clone "https://github.com/aiobofh/cutest.git" -q

-include cutest/src/cutest.mk
-include cutest/src/coverage.mk

blcheck: blcheck.c story.c
	$(CC) -o $@ $^ -D"call(func)=func" -O2

clean::
	$(Q)$(RM) -rf *~ blcheck cutest
