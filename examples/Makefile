#
# This is an example make file for manageing the bltools on a daily basis with ease
#

Q=@

TODO_MIN=$(shell ../src/blsum -t -n combined.org)
TODO_MAX=$(shell ../src/blsum -t -x combined.org)
DONE=$(shell ../src/blsum -d combined.org)
VELO=$(shell tail -1 velocity.dat | cut -d' ' -f2)
MEAN=$(shell tail -1 velocity.dat | cut -d' ' -f3)
LASTSPRINT=$(shell ../src/blsprints sprint.list | tail -1)
MIN=$(shell tail -1 sprint.dat | cut -d' ' -f2)
MAX=$(shell head -1 sprint.dat | cut -d' ' -f2)
MINF=$(shell if [ ${MIN} -lt 0 ]; then echo ${MIN}; else echo 0; fi)
MAXF=$(shell echo "${MAX}+1" | bc)
DAYSx2=$(shell tail -1 sprint.list | cut -d' ' -f3 | wc -c)
LEN=$(shell echo "${DAYSx2}/2" | bc)
LEFT_MIN=$(shell echo "${TODO_MIN}/${MEAN}/5" | bc)
LEFT_MAX=$(shell echo "${TODO_MAX}/${MEAN}/5" | bc)

all:
	${Q}${MAKE} -C ../src -s blsum; \
	${MAKE} -C ../src -s blvelocity; \
	${MAKE} -C ../src -s blsprints; \
	${MAKE} -C ../src -s blburn; \
	${MAKE} -s summary

sprint.html: sprint.org
	$(Q)./sprint2html.sh > $@

sprint.pdf: sprint.html
	$(Q)wkhtmltopdf $< $@

combined.org: backlog.org sprint.org done.org
	${Q}cat $^ | grep -v '^* NEXT' | grep -v '^* Sprint' > $@

sprint.dat: combined.org sprint.list
	${Q}../src/blburn combined.org sprint.list ${LASTSPRINT} > $@

sprint.gpt: sprint.dat sprint.list
	${Q}echo "set title \"Tools team burn-down ${LASTSPRINT} of ${LEN} days" > $@; \
	echo "set xlabel \"Days\"" >> $@; \
	echo "set ylabel \"Points\"" >> $@; \
	echo "set xzeroaxis" >> $@; \
	echo "set term dumb" >> $@; \
	echo "set nokey" >> $@; \
	echo "set yrange[${MINF}:${MAXF}]" >> $@; \
	echo "plot [0:${LEN}] \"sprint.dat\" using 2:xtic(1) with lines" >> $@


velocity.dat: combined.org sprint.list
	${Q}../src/blvelocity combined.org sprint.list | cut -d'-' -f2 > $@

velocity.gpt: velocity.dat
	${Q}echo "set title \"Mean velocity per day, over time" > $@; \
	echo "set xlabel \"Sprint\"" >> $@; \
	echo "set ylabel \"Points\"" >> $@; \
	echo "set xzeroaxis" >> $@; \
	echo "set term dumb" >> $@; \
	echo "set nokey" >> $@; \
	echo "set yrange[0:2]" >> $@; \
	echo "plot \"velocity.dat\" using 2:xtic(1) with lines" >> $@

summary: combined.org sprint.list velocity.dat sprint.dat velocity.asc sprint.asc
	${Q}echo "TODO: ${TODO_MIN}-${TODO_MAX} poins in backlog"; \
	echo "DONE: ${DONE} points"; \
	echo "VELO: ${VELO} points/day last sprint ${MEAN} mean points/day";\
	echo "LEFT: ${LEFT_MIN} to ${LEFT_MAX} weeks of work left"; \
	cat velocity.asc; \
	cat sprint.asc; \
	echo "You can produce grapical versions by using:"; \
	echo ""; \
	echo " $$ make sprint.png"; \
	echo " $$ make velocity.png"; \
	echo ""; \
	echo "You can also produce a printable PDF version of the current sprint using:"; \
	echo ""; \
	echo " $$ make sprint.pdf"; \
	echo ""

# Modify a .gnuplot file to output a PNG file instead of dumb ASCII art
%-png.gpt: %.gpt
	${Q}sed -e "s/dumb/pngcairo size 1280,720 enhanced font 'Verdana,20'/" $^ > $@

# Modify a .gnuplot file to output a SVG file instead of dumb ASCII art
%-svg.gpt: %.gpt
	${Q}sed -e "s/dumb/svg size 1280,720 fname 'Verdana' fsize 20/" $^ > $@

# Generate a PNG file form a *-png.gpt file
%.png: %-png.gpt
	${Q}gnuplot $^ > $@

# Generate a SVG file form a *-svg.gpt file
%.svg: %-svg.gpt
	${Q}gnuplot $^ > $@

# Generate a ASCII art file form a *.gpt file
%.asc: %.gpt %.dat
	${Q}gnuplot $< > $@

top_of_backlog.cnv: backlog.org Makefile sprint.org
	${Q}echo -n 'text 0,15 "' > $@
	${Q}grep "^* " sprint.org | sed -e 's/ *:.*:$$//g;s/"//g' >> $@
	${Q}echo "--------------------------" >> $@
	${Q}grep "^* " $< | head -10 | sed -e 's/ *:.*:$$//g;s/"//g' >> $@
	${Q}echo '"' >> $@

top_of_backlog.png: top_of_backlog.cnv
	${Q}convert -size 520x195 xc:white -pointsize 16 -fill black -draw @$^ $@

clean:
	${Q}rm -f *~ combined.org sprint.dat velocity.dat *.gpt *.png *.svg *.asc sprint.html sprint.pdf top_of_backlog.*; \
	${MAKE} -C ../src clean
