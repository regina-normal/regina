/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::ProgressTracker
static const char *ProgressTracker =
R"doc(Manages percentage-based progress tracking and cancellation polling
for long operations.

See the ProgressTrackerBase documentation for detailed information on
how to use a progress tracker.

This class represents a progress tracker that measures progress using
percentages. Specifically, each stage has a percentage progress that
rises from 0 to 100 as the stage progresses. Each stage also has a
fractional weight (between 0 and 1 inclusive), and the percentage
progress of the entire calculation is taken to be the weighted sum of
the progress of the individual stages. The weights of all stages
should sum to 1.)doc";

// Docstring regina::python::doc::ProgressTrackerBase
static const char *ProgressTrackerBase =
R"doc(The base class for Regina's progress tracking classes.

These classes manage progress tracking and cancellation polling for
long operations. A typical progress tracker is simultaneously used by
a *writing* thread, which is performing the long calculations, and a
*reading* thread, which displays progress updates to the user and/or
takes cancellation requests from the user.

Progress works through a series of *stages*. Each stage has a text
description, as well as a numerical progress indicator. For the class
ProgressTracker, this is a percentage that rises from 0 to 100 as the
stage progresses; for ProgressTrackerOpen, this is an integer that
starts at 0 and rises (with no particular upper bound).

The life cycle of a progress tracker is as follows.

* The reading thread creates a progress tracker (of type
  ProgressTracker or ProgressTrackerOpen), and passes it to the
  writing thread when the calculation begins.

* The writing thread:

  * creates the first stage by calling newStage();

  * as the stage progresses, repeatedly updates the progress of this
    stage by calling routines such as ProgressTracker::setPercent() or
    ProgressTrackerOpen::incSteps(), and also polls for cancellation
    by calling isCancelled();

  * moves through any additional stages in a similar fashion, by
    calling newStage() and then repeatedly calling routines such as
    ProgressTracker::setPercent(), ProgressTrackerOpen::incSteps(),
    and isCancelled();

  * calls setFinished() once all processing is complete (regardless of
    whether the operation finished naturally or was cancelled by the
    user);

  * never touches the progress tracker again.

* The reading thread:

  * passes the progress tracker to the writing thread;

  * repeatedly polls the state of the tracker by calling routines such
    as ProgressTracker::percentChanged(),
    ProgressTrackerOpen::stepsChanged(), descriptionChanged() and/or
    isFinished();

  * if percentChanged() or stepsChanged() returns ``True``, collects
    the total progress by calling percent() or steps() respectively
    and displays this to the user;

  * if descriptionChanged() returns ``True``, collects the description
    of the current stage by calling description() and displays this to
    the user;

  * if the user ever chooses to cancel the operation, calls cancel()
    but continues polling the state of the tracker as above;

  * once isFinished() returns ``True``, displays any final information
    to the user and destroys the progress tracker.

It is imperative that the writing thread does not access the tracker
after calling setFinished(), and it is imperative that the reading
thread does not destroy the tracker until after isFinished() returns
``True``. In particular, even if the reading thread has called
cancel(), it must still wait upon isFinished() before destroying the
tracker. Until isFinished() returns ``True``, there is no guarantee
that the writing thread has detected and honoured the cancellation
request.

Progress trackers rely on multiple threads accessing the same
underlying object, and so they cannot be copied, moved or swapped.

.. note::
    This class implements common functionality for ProgressTracker and
    ProgressTrackerOpen, and should not be used on its own. Instead,
    you should always use either ProgressTracker or
    ProgressTrackerOpen (according to whether you need percentage-
    based or open-ended progress tracking respectively).)doc";

// Docstring regina::python::doc::ProgressTrackerOpen
static const char *ProgressTrackerOpen =
R"doc(Manages percentage-based progress tracking and cancellation polling
for open-ended operations.

See the ProgressTrackerBase documentation for detailed information on
how to use a progress tracker.

This class represents a progress tracker that measures progress using
a non-negative integer, which typically indicates the number of steps
completed so far during the operation. This integer begin at 0 and
rises as the operation progresses. There is no particular "end point"
or upper bound on the number of steps, and indeed the end point is
often unknown until the operation has finished.)doc";

namespace ProgressTrackerBase_ {

// Docstring regina::python::doc::ProgressTrackerBase_::cancel
static const char *cancel =
R"doc(Indicates to the writing thread that the user wishes to cancel the
operation. The writing thread might not detect and/or respond to this
request immediately (or indeed ever), and so the reading thread should
continue to wait until isFinished() returns ``True`` before it cleans
up and destroys this progress tracker.

This is typically called by the reading thread.)doc";

// Docstring regina::python::doc::ProgressTrackerBase_::description
static const char *description =
R"doc(Returns the human-readable description of the current stage.

This is typically called by the reading thread.

Returns:
    ``the`` current stage description.)doc";

// Docstring regina::python::doc::ProgressTrackerBase_::descriptionChanged
static const char *descriptionChanged =
R"doc(Queries whether the stage description has changed since the last call
to descriptionChanged(). If this is the first time
descriptionChanged() is called, the result will be ``True``.

This is typically called by the reading thread.

Returns:
    ``True`` if and only if the stage description has changed.)doc";

// Docstring regina::python::doc::ProgressTrackerBase_::isCancelled
static const char *isCancelled =
R"doc(Queries whether the reading thread has made a request for the writing
thread to cancel the operation; in other words, whether cancel() has
been called.

This is typically called by the writing thread.

Returns:
    ``True`` if and only if a cancellation request has been made.)doc";

// Docstring regina::python::doc::ProgressTrackerBase_::isFinished
static const char *isFinished =
R"doc(Queries whether the writing thread has finished all processing. This
will eventually return ``True`` regardless of whether the processing
finished naturally or was cancelled by the reading thread.

This is typically called by the reading thread.

Returns:
    ``True`` if and only if the writing thread has finished all
    processing.)doc";

// Docstring regina::python::doc::ProgressTrackerBase_::setFinished
static const char *setFinished =
R"doc(Used by the writing thread to indicate that it has finished all
processing. The stage description will be updated to indicate that the
operation is finished.

This is typically called by the writing thread.

.. warning::
    The base class implementation of this routine should never be
    called (and indeed, it is declared here but not implemented). You
    should always call either ProgressTracker::setFinished() or
    ProgressTrackerOpen::setFinished().)doc";

}

namespace ProgressTrackerOpen_ {

// Docstring regina::python::doc::ProgressTrackerOpen_::__default
static const char *__default =
R"doc(Creates a new progress tracker. This sets a sensible state description
(which declares that the operation is initialising), and marks the
current progress as zero steps completed.

This is typically called by the reading thread.)doc";

// Docstring regina::python::doc::ProgressTrackerOpen_::incSteps
static const char *incSteps =
R"doc(Used by the writing thread to indicate that one more step has been
completed.

This is typically called by the writing thread.

Returns:
    ``True`` if there has been no cancellation request, or ``False``
    if cancel() has been called (typically by the reading thread).)doc";

// Docstring regina::python::doc::ProgressTrackerOpen_::incSteps_2
static const char *incSteps_2 =
R"doc(Used by the writing thread to indicate that some number of additional
steps have been completed.

This is typically called by the writing thread.

Parameter ``add``:
    the number of additional steps that have been completed. The value
    returned by steps() will increase by this amount.

Returns:
    ``True`` if there has been no cancellation request, or ``False``
    if cancel() has been called (typically by the reading thread).)doc";

// Docstring regina::python::doc::ProgressTrackerOpen_::newStage
static const char *newStage =
R"doc(Used by the writing thread to indicate that it has moved on to a new
stage of processing. The number of steps completed will be left
unchanged.

This is typically called by the writing thread.

Parameter ``desc``:
    a human-readable description of the new stage. Typically this
    begins with a capital and does not include a final period (full
    stop).)doc";

// Docstring regina::python::doc::ProgressTrackerOpen_::setFinished
static const char *setFinished =
R"doc(Used by the writing thread to indicate that it has finished all
processing. The total number of steps completed will be left
unchanged, but the stage description will be updated to indicate that
the operation is finished.

This is typically called by the writing thread.)doc";

// Docstring regina::python::doc::ProgressTrackerOpen_::steps
static const char *steps =
R"doc(Returns the number of steps completed throughout the entire operation.
This counts the progress across all stages (both current and
previous).

This is typically called by the reading thread.

Returns:
    the current number of steps completed.)doc";

// Docstring regina::python::doc::ProgressTrackerOpen_::stepsChanged
static const char *stepsChanged =
R"doc(Queries whether the number of steps completed has changed since the
last call to stepsChanged(). If this is the first time stepsChanged()
is called, the result will be ``True``.

This is typically called by the reading thread.

Returns:
    ``True`` if and only if the number of steps completed has changed.)doc";

}

namespace ProgressTracker_ {

// Docstring regina::python::doc::ProgressTracker_::__default
static const char *__default =
R"doc(Creates a new progress tracker. This sets a sensible state description
(which declares that the operation is initialising), and marks the
current progress as zero percent complete.

This is typically called by the reading thread.)doc";

// Docstring regina::python::doc::ProgressTracker_::newStage
static const char *newStage =
R"doc(Used by the writing thread to indicate that it has moved on to a new
stage of processing. The percentage progress through the current stage
will automatically be set to 100.

This is typically called by the writing thread.

Parameter ``desc``:
    a human-readable description of the new stage. Typically this
    begins with a capital and does not include a final period (full
    stop).

Parameter ``weight``:
    the relative weight of this stage as a fraction of the entire
    operation. This weight must be between 0 and 1 inclusive, and the
    weights of *all* stages must sum to 1 in total.)doc";

// Docstring regina::python::doc::ProgressTracker_::percent
static const char *percent =
R"doc(Returns the percentage progress through the entire operation. This
combines the progress through the current stage with all previous
stages, taking into account the relative weights that the writing
thread has passed to newStage().

This is typically called by the reading thread.

Returns:
    the current percentage progress.)doc";

// Docstring regina::python::doc::ProgressTracker_::percentChanged
static const char *percentChanged =
R"doc(Queries whether the percentage progress has changed since the last
call to percentChanged(). If this is the first time percentChanged()
is called, the result will be ``True``.

This is typically called by the reading thread.

Returns:
    ``True`` if and only if the percentage progress has changed.)doc";

// Docstring regina::python::doc::ProgressTracker_::setFinished
static const char *setFinished =
R"doc(Used by the writing thread to indicate that it has finished all
processing. The percentage progress through both the current stage and
the entire operation will automatically be set to 100, and the stage
description will be updated to indicate that the operation is
finished.

This is typically called by the writing thread.)doc";

// Docstring regina::python::doc::ProgressTracker_::setPercent
static const char *setPercent =
R"doc(Used by the writing thread to indicate the level of progress through
the current stage.

Unlike percent(), which measures progress in the context of the entire
operation, this routine takes a percentage that is strictly relative
to the current stage (i.e., the stage most recently declared through a
call to newStage()). When the stage begins, setPercent() would
typically be given a figure close to 0; when the stage is finished,
setPercent() would typically be given a figure close to 100.

There is no actual need to call setPercent(0) at the beginning of the
stage or setPercent(100) at the end of the stage, since other routines
(such as the constructor, newStage() and setFinished()) will take care
of this for you.

This is typically called by the writing thread.

Parameter ``percent``:
    the percentage progress through this stage, as a number between 0
    and 100 inclusive.

Returns:
    ``True`` if there has been no cancellation request, or ``False``
    if cancel() has been called (typically by the reading thread).)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

