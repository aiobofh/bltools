bltools
=======

Emacs Org-Mode text-file Scrum backlog tools. This project will provide a few
simple and quick tools to summarize backlog points, done points manage team
velocity.

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

And by just letting the bltools parse this file you can get a lot of
information::

  $ blsum -d example.org  # Get all DONE points
  5

  $ blsum -t -n example.org  # Get a sum of the minimum number of points TODO
  4

  $ blsum -t -x example.org  # Get a sum of the maximum number of points TODO
  23

There is also a tool called blcheck which only validates the formatting of
the org-files. So that they look as bltools expect them to look.

Burn-down generator
-------------------

It's possible to feed some more information into the blburn command to get a
plotable burn-down for a specific sprint. This requires you to declare the
sprint start/stop, weekdays within the sprint and the commitment.

For example a one-week sprint in the beginning of 2017 with a commitment of
10 points should look something like this::

  2017-01-02 2017-01-06 MoTuWeThFr 10 Sprint-001

Stored in a file. What this means is that a sprint is defined between the two
days and all the days (Mo, Tu, We, Th and Fr) are working days. The commitment
level for the sprint is 10 points and it's called "Sprint-001".

Then the blburn command will be able to generate a series of points for use
with for example GNU Plot to create a burn-down chart plot for the specified
sprint::

  $ blburn example.org sprints.list Sprint-001
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

Note
----

Most of the executable tools build with this toolkit are still very
experimental. However the code library underneath is fairly well tested
using CUTest https://github.com/aiobofh/cutest .
