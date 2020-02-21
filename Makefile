all:
	@${MAKE} --no-print-directory -C src && \
	${MAKE} --no-print-directory -C test

clean:
	@${MAKE} --no-print-directory -C src clean && \
	${MAKE} --no-print-directory -C test clean
