bltools
=======

Emacs Org-Mode text-file Scrum backlog tools. This project will provide a few
simple and quick tools to summarize backlog points, done points manage team
velocity.

This set of tools came into existence because of the *super* easy-to-use
Emacs Org-Mode format for keeping track of task-lists. If you want to study
the Org-Mode in further detail than what this README describes please check
out the http://orgmode.org/ page. The bltools might break some of the standard
but if you use this carefully you will be a happy camper, using Emacs (or
other text editors) to keep your backlog in nice shape. It's super convenient
to move thins about in priority order and getting a structured format for
your backlog, without relying on a heavy "agile tool" that requires a
shit-load of administration. The ``bltools`` is an attempt to KISS principle.

The text-format is also *extremely* easy to keep in git. You can resolve
conflicts and collaborate very easily this way. I know many teams that keep
the backlog in an Excel sheet in the Share-point server or on a shared network
folder. But we all know how this will work with multiple users.... Not at all.

It is also very nice to be able to work in text-mode only consoles with your
backlog if you're a nerd like me.

Current state of this project is that the parser code and common code is in
pretty OK shape, but the executable tools are still in need of improvements.
Especially when it comes to documentation. In this README you can _briefly_
get an idea on how to use them. But since the project is pretty young you
might still need to refer to the source code to understand how to use the
tools in the best way.

You can find excessive test-code for the common code and the parser code in
the source-folder. This is also a nice example hon how to use the CUTest test
framework, which can be found here https://github.com/aiobofh/cutest.

Backlogs
--------

If you run a Scrum team you might have a huge backlog to take care of, this
is an example on how to organize your Emacs Org-Mode files:

* backlog.org
  This is the file where you have all your upcoming tasks stored, in estimated
  and prioritized fashion.
* sprint.org
  This is the contents of the current sprint, where you update stories as they
  get done.
* done.org
  The graveyard of all done stories.

But you can also keep everything in one file. The bltools usually take only
a concatinated file from all .org-files you treat as your backlog as argument.

This is an example of a few valid story slogan rows::

  * TODO 00 It shall be possible to store stories in Emacs Org-Mode format

    As a software developer I need my stories in a very easy to use format,
    like Emacs Org-Mode to make my agile mindset easier to realize.

  * TODO 01-20 It shall be possible to make fuzzy estimate ranges of a story

    As a product owner I want to be able to keep stories estimated even if
    they are not well defined yet.

  * TODO 03 It shall be possible to have accurate estimates of a story

    As a product owner I need to be able to have single (and accurate)
    estimates of stories in the backlog as the stories grow more defined.

  * DONE 05 It shall be possible to toggle stories to DONE to track progress
    SCHEDULED: <2017-01-03> DEADLINE: <2017-01-05>

    As a product owner I need to be able to track story progress and to be
    able to calculate velocity over time.

    * The SCHEDULED Emacs Org-Mode tag is the date when the story was started.
    * The DEADLINE Emacs Org-Mode tag is the date when the story was done.

    This is actually MANDATORY to be able to consider a story to be done.

There is a corner case with ``bltools`` that requires you to have ALL stories
estimated. But it can handle estimate ranges like this ``03-12`` meaning that
the story is somewhere between 3p and 12p hard to do. Or you can just put
``00`` on un-estimated stories until you have had the chance to groom your
backlog further.

Me (personally) prefer to write my stories like this::

  * TODO 00 Doc. of bltools shall explain how to write stories :need-estimate:

    As a Certified Scrum Master or Certified Product Owner i need to be able
    to understand how to keep my backlog in as good a shape as possible to
    be able to impress all my friends at work and have them awe my skills.

    * [ ] Write some kind of example-based documentation
    * [ ] Make sure there are as few spelling errors as possible
    * [ ] Try it out on my friends

    DoD: The bltools documentation shall explain how to write stories in a
         good, example-based way with as few spelling errors as possible.

... Then when the story is groomed and understood by the implementation team
the title row should look like this instead (with an estimate)::

  * TODO 01 Doc. of bltools shall explain how to write stories...

It should also be SHIFT-Up/Down positioned in the correct priority with all
the other stories in the backlog Org-Mode file.

When the story is done it should look like this::

  * DONE 01 Doc. of bltools shall explain how to write stories
    STARTDATE: <2017-03-23> DEADLINE: <2017-03-24>

    As a Certified Scrum Master or Certified Product Owner i need to be able
    to understand how to keep my backlog in as good a shape as possible to
    be able to impress all my friends at work and have them awe my skills.

    * [X] Write some kind of example-based documentation
    * [X] Make sure there are as few spelling errors as possible
    * [X] Try it out on my friends

    DoD: The bltools documentation shall explain how to write stories in a
         good, example-based way with as few spelling errors as possible.

As simple as that... :) The ``bltools`` only care about the TODO/DONE row and
the STARTDATE/DEADLINE-row to build all the information for you, regarding the
burn-down data and velocity calculations.

Statistics parser - blsum
-------------------------

By just letting the bltools parse this file you can get a lot of
information::

  $ blsum -d example.org  # Get all DONE points
  5

  $ blsum -t -n example.org  # Get a sum of the minimum number of points TODO
  4

  $ blsum -t -x example.org  # Get a sum of the maximum number of points TODO
  23

There is also a tool called ``blcheck`` which only validates the formatting
of the org-files. So that they look as bltools expect them to look.

Burn-down generator - blburn
----------------------------

It's possible to feed some more information into the ``blburn`` command to
get a plotable burn-down for a specific sprint. This requires you to declare
the sprint start/stop, weekdays within the sprint and the commitment.

For example a one-week sprint in the beginning of 2017 with a commitment of
10 points should look something like this::

  2017-01-02 2017-01-06 MoTuWeThFr 10 Sprint-001

Stored in a file. What this means is that a sprint is defined between the two
days and all the days (Mo, Tu, We, Th and Fr) are working days. The commitment
level for the sprint is 10 points and it's called "Sprint-001".

Then the ``blburn`` command will be able to generate a series of points for use
with for example GNU Plot to create a burn-down chart plot for the specified
sprint::

  $ blburn example.org sprints.list Sprint-001  # Get the burn-down of sprint
  Mo 10
  Tu 10
  We 10
  Th 5
  Fr 5

But if there for example is a holiday on Thursday it shall be removed from
the sprint definition line in the sprint-list::

  2017-01-02 2017-01-06 MoTuWeFr 10 Sprint-001

And the output will be adjusted accordingly::

  $ blburn example.org sprints.list Sprint-001
  Mo 10
  Tu 10
  We 10
  Fr 5

Velocity trend generator - blvelocity
-------------------------------------

Having the sprint-list file defined and the Emacs Org-Mode files in place with
the STARTDATE and DEADLINE tags set on DONE stories it's perfectly possible to
generate a plot of the velocity trend for your team::

  $ blvelocity example.org sprints.list
  Sprint-001 0.300000 0.300000
  Sprint-002 0.900000 0.600000
  Sprint-003 1.100000 0.766667
  ...

The sprint ID read from the sprint-list file, and the DEADLINE date is used to
determine which sprint a story was done and the first floating point number is
the mean velocity of the sprint in question, in points per day. The third and
last column is the mean velocity over time in points per day. This will help
you keep track of over-/under-commitment for future sprints.

Use in conjunction with Gnuplot
-------------------------------

It's pretty easy to parse the output from the bltools further to find min/max
and graph lengths and such things. A nice way to present them is with Gnuplot.

For example if you'd like a ASCII-art graph in your terminal or a SVG file for
your web-site to radiate your progress.

A Gnuplot template to generate a graphical burn-down chart
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you use a template somewhat looking like this::

  set title "The awesome team burn-down @SPRINTID@ of @DAYS@ days
  set xlabel "Days"
  set ylabel "Points"
  set xzeroaxis
  set term dumb
  set nokey
  set yrange[@MINPTS@:@MAXPTS@]
  plot [0:@DAYS@] "sprint.dat" using 2:xtic(1) with lines

... You will get a nice burn-down chart for the data from ``blburn`` stored in
the ``sprint.dat`` file. You need to ``sed`` the @KEYWORD@ stuff in this
template to your relevant information.

Sprint-ID
~~~~~~~~~

You can extract the latest sprint ID from your sprint-list using::

  $ SPRINTID = $(blsprints sprint.list | tail -1)

Sprint-duration
~~~~~~~~~~~~~~~

You can get the number of days in the sprint by doing some magic like::

  $ DAYS = echo "$(tail -1 sprint.list | cut -d' ' -f3 | wc -c)/2" | bc

Sprint burn-down points target
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The magic of have 0 (or less if you managed to finish some Next-storries)::

  $ MIN = $(blburn foo.org sprint.list $SPRINTID | tail -1 | cut -d' ' -f2)
  $ MINPTS = $(if [ ${MIN} -lt 0 ]; then echo ${MIN}; else echo 0; fi)

... And the commitment for the sprint should probably be the max value::

  $ MAXPTS = $(blburn foo.org sprint.list $SPRINTID | head -1 | cut -d' ' -f2)

A Gnuplot template to generate a graphical velocity trent chart
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can use the output from ``blvelocity`` straight away with Gnuplot as well::

  set title "Mean velocity per day, over time
  set xlabel "Sprint"
  set ylabel "Points"
  set xzeroaxis
  set term dumb
  set nokey
  set yrange[0:2]
  plot "velocity.dat" using 2:xtic(1) with lines

This Gnuplot configuration will read the output of ``blvelocity`` stored in
the ``velocity.dat`` file to generate a nice trend chart.

Note
----

Most of the executable tools build with this toolkit are still very
experimental. However the code library underneath is fairly well tested
using CUTest https://github.com/aiobofh/cutest.
