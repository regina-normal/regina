# Additional initialisation that takes place only when running Regina
# from within Sage:

from . import engine
import sage.all

# -------------------------------------------------------------------------
# Conversion from Regina objects to Sage objects
# -------------------------------------------------------------------------

from .engine import GroupPresentation

# Additional methods for GroupPresentation

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
