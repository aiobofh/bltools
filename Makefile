all:
	@${MAKE} --no-print-directory -C src && \
	${MAKE} --no-print-directory -C test && \
	${MAKE} --no-print-directory -C test xml

clean:
	@${MAKE} --no-print-directory -C src clean && \
	${MAKE} --no-print-directory -C test clean
