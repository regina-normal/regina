################################
#
#  Sample Python Script
#
#  Illustrates progress reporting during long operations.
#
#  See the file "progress.session" for the results of running this script.
#
################################

import time

# Create a new triangulation of the lens space L(49,12).
tri = regina.NTriangulation()
tri.insertLayeredLensSpace(49,12)
print tri.getNumberOfTetrahedra(), 'tetrahedra'

# Create a progress manager to use during the lengthy surface enumeration.
# This will be responsible for reporting the state of progress while the
# enumeration runs in the background.
manager = regina.NProgressManager()

# Start the normal surface enumeration.
# Because we are passing a progress manager to enumerate(), the
# enumeration will be started in the background and control will be
# returned immediately to the python console.
surfaces = regina.NNormalSurfaceList.enumerate(tri,
    regina.NNormalSurfaceList.STANDARD, 1, manager)

# Wait for the surface enumeration to fully start up.
while not manager.isStarted():
    time.sleep(1)


# At this point the surface enumeration is now running.
# Output a progress report every second until it's finished.
prog = manager.getProgress()
while not manager.isFinished():
    print 'Progress:', prog.getDescription()
    time.sleep(1)


# The surface enumeration is now complete.
print surfaces.getNumberOfSurfaces(), 'normal surfaces'

# Output the total time spent during the surface enumeration.
print 'Total real time:', prog.getRealTime(), 'seconds'
print 'Total cpu time:', prog.totalCPUTime(), 'seconds'
