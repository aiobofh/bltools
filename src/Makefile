Q=@

.SUFFIXES:

all:
	$(Q)${MAKE} -s cutest && ${MAKE} -s check && ${MAKE} -s blcheck

cutest:
	$(Q)git clone "https://github.com/aiobofh/cutest.git" -q

-include cutest/src/cutest.mk
-include cutest/src/coverage.mk

blcheck: blcheck.c story.c backlog.c
	$(CC) -o $@ $^ -D"call(func)=func" -O2

blsum: blsum.c story.c backlog.c
	$(CC) -o $@ $^ -D"call(func)=func" -O2

clean::
	$(Q)$(RM) -rf *~ blcheck cutest
