Q=@

.SUFFIXES:

all:
	$(Q)${MAKE} -s cutest && \
	${MAKE} -s check && \
	${MAKE} -s blcheck && \
	${MAKE} -s blsum && \
	${MAKE} -s blsprints && \
	${MAKE} -s blburn

cutest:
	$(Q)git clone "https://github.com/aiobofh/cutest.git" -q

-include cutest/src/cutest.mk
-include cutest/src/coverage.mk

blcheck: blcheck.c date.c story.c backlog.c
	$(CC) -o $@ $^ -O2

blsum: blsum.c date.c story.c backlog.c
	$(CC) -o $@ $^ -O2

blsprints: blsprints.c date.c sprint.c
	$(CC) -o $@ $^ -O2

blburn: blburn.c date.c sprint.c story.c backlog.c
	$(CC) -o $@ $^ -O2

clean::
	$(Q)$(RM) -rf *~ blcheck blsum blsprints blburn
