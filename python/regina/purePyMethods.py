from .engine import Packet, Triangulation3, GroupPresentation

try:
    import sage.all
    _within_sage = True
except:
    _within_sage = False

# Additional method for Packet

def _children(self):
    """
    Iterate through all children of this NPacket.
    """
    child = self.firstChild()
    while child:
        yield child
        child = child.nextSibling()

Packet.children = _children


def _descendents(self):
    """
    Iterate through all descendents of this NPacket.
    """

    for child in self.children():
        yield child
        for desc in child.descendents():
            yield desc

Packet.descendents = _descendents

# C'tor for Triangulation3 to take SnapPy Triangulation's and Manifold's

_original_trig_ctor = Triangulation3.__init__

def _trig_ctor(self, *args):
    if len(args) == 1 and hasattr(args[0], '_to_string'):
        # Convert from a SnapPea manifold
        _original_trig_ctor(self, args[0]._to_string())
    else:
        # Call original constructor
        _original_trig_ctor(self, *args)

Triangulation3.__init__ = _trig_ctor

# Additional methods for GroupPresentation

if _within_sage:
    # Follow convention that .sage() yields a representation of
    # a mathematical object native to sage

    def _convertRel(generators, rel):
        return sage.all.prod([
                generators[term.generator] ** term.exponent
                for term in rel.terms()])

    def _convertGroupPresentation(self):
        """
        Returns a Sage version of this finitely presented group.
        """
        F = sage.all.FreeGroup(self.countGenerators())
        gens = F.generators()
        rels = [
            _convertRel(gens, self.relation(i))
            for i in range(self.countRelations())]
        return F/rels

    GroupPresentation.sage = _convertGroupPresentation
