Q?=@

MAKEFLAGS += --no-builtin-rules
CFLAGS=-Wall -pedantic -std=c99 -I.

export Q

.SUFFIXES:

all:
	$(Q)export Q="${Q}"; \
	${MAKE} -s blcheck && \
	${MAKE} -s blsum && \
	${MAKE} -s blsprints && \
	${MAKE} -s blburn && \
	${MAKE} -s blvelocity

.PRECIOUS: %.o
%.o: %.c
	${Q}${CC} ${CFLAGS} -o $@ -c $<

blcheck: blcheck.o date.o story.o backlog.o
	$(CC) -o $@ $^

blsum: blsum.o date.o story.o backlog.o
	$(CC) -o $@ $^

blsprints: blsprints.o date.o sprint.o
	$(CC) -o $@ $^

blburn: blburn.o date.o sprint.o story.o backlog.o
	$(CC) -o $@ $^

blvelocity: blvelocity.o date.o sprint.o story.o backlog.o
	$(CC) -o $@ $^

clean::
	$(Q)$(RM) -rf *~ blcheck blsum blsprints blburn
