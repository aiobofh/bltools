all:
	@${MAKE} --no-print-directory -C src && \
	${MAKE} --no-print-directory -C test && \
	${MAKE} --no-print-directory -C test xml

clean:
	@${RM} *~ && \
	${MAKE} --no-print-directory -C src clean && \
	${MAKE} --no-print-directory -C test clean
