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
TTMPROOT=/tmp/${USER}/tarsio/
TSRCROOT=../src/

check::
	@${MAKE} --no-print-directory -C ../src/

#
# Depend on the Tarsio make system
#
-include $(shell pkg-config --variable=includedir tarsio)/tarsio.mk
-include $(shell pkg-config --variable=includedir tarsio)/coverage.mk
