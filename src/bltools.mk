Q=@

blsum: bltools
	${Q}${MAKE} -s -C bltools/src/ blsum; ln -s bltools/src/blsum

blcheck: bltools
	${Q}${MAKE} -s -C bltools/src/ blcheck; ln -s bltools/src/blcheck

blsprints: bltools
	${Q}${MAKE} -s -C bltools/src/ blsprints; ln -s bltools/src/blsprints

blburn: bltools
	${Q}${MAKE} -s -C bltools/src/ blburn; ln -s bltools/src/blburn

clean::
	${Q}rm -rf blcheck blsum blsprints blburn
