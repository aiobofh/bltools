#
# Speedup GNU Make a bit
#
MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

#
# Default clean builds
#
CFLAGS=-Wall -pedantic -std=c99 -I.

#
# Configure Tarsio
#
TTMPROOT=/tmp/tarsio/${USER}/
TSRCROOT=../src/

check::
	@${MAKE} --no-print-directory -C ../src/

#
# Depend on the Tarsio make system
#
-include $(shell pkg-config --variable=includedir tarsio)/tarsio.mk

#
# Additinal linking information (for module-tests real code is linked)
#
backlog_check: ${TSRCROOT}story.o ${TSRCROOT}date.o

blcheck_check: ${TSRCROOT}backlog.o ${TSRCROOT}story.o ${TSRCROOT}date.o

blsprints_check: ${TSRCROOT}sprint.o ${TSRCROOT}date.o

blsum_check: ${TSRCROOT}backlog.o ${TSRCROOT}story.o ${TSRCROOT}date.o

sprint_check: ${TSRCROOT}date.o

story_check: ${TSRCROOT}date.o
