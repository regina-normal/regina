# Thanks to Matthias Goerner for writing these helper functions to make
# the Python experience more pleasant in Regina.

from .engine import Triangulation3

# C'tor for Triangulation3 to take a SnapPy Triangulation or Manifold

_original_trig_ctor = Triangulation3.__init__

def _trig_ctor(self, *args):
    if len(args) == 1 and hasattr(args[0], '_to_string'):
        # Convert from a SnapPea manifold
        _original_trig_ctor(self, args[0]._to_string())
    else:
        # Call original constructor
        _original_trig_ctor(self, *args)

Triangulation3.__init__ = _trig_ctor

