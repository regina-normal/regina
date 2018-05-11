from .engine import Integer, LargeInteger

# The python wrapping for Integer and LargeInteger both register
# an __init__ which can take a long and a double.
#
# When boost::python notices that an integer does not fit into a long
# and thus is not convertible, it actually tries to convert it to a
# double and succeeds although it is loosing precision in the process.
#
# We override __init__ here to fix this.

def fixInteger(IntegerType):

    IntegerType.__init__raw = IntegerType.__init__

    def __init__(self, *args):
        """
        Constructs a regina.Integer, respectively, regina.LargeInteger
        from a python integer, string (with an optional base specified
        as second argument), float or a regina.Integer or regina.LargeInteger.
        """
        
        if len(args) > 2:
            IntegerType.__init__raw(self)
            raise TypeError("%s() takes at most 2 arguments" % IntegerType.__name__)
        
        if len(args) == 2:
            # Cover case where we have string and base
            string, base = args
            if not isinstance(string, str):
                IntegerType.__init__raw(self)
                raise TypeError(
                    "%s() expects first argument to be string when given a "
                    "base" % IntegerType.__name__)
            if not isinstance(base, int) or isinstance(base, long):
                IntegerType.__init__raw(self)
                raise TypeError(
                    "%s() expects second argument to be an integer used as " %
                    "base" % IntegerType.__name__)
            IntegerType.__init__raw(self, string, base)
            return

        if len(args) == 1:
            arg = args[0]
            if (isinstance(arg, float) or
                isinstance(arg, str) or
                isinstance(arg, Integer)):
                IntegerType.__init__raw(self, arg)
                return
            if isinstance(arg, LargeInteger):
                # Cover case where we try to convert a LargeInteger holding
                # inf to an Integer
                if IntegerType == Integer and arg.isInfinite():
                    IntegerType.__init__raw(self)
                    raise ValueError("Cannot convert infinity to Integer.")
                IntegerType.__init__raw(self, arg)
                return
            if isinstance(arg, int) or isinstance(arg, long):
                if (Integer._smallest_long <= arg and
                    arg <= Integer._largest_long):
                    IntegerType.__init__raw(self, arg)
                else:
                    # The crucial problem mentioned above:
                    # boost::python would use the double constructor
                    # and loose precision.
                    # Explicitly convert to a string.
                    IntegerType.__init__raw(self, '%d' % arg)
                return

            raise TypeError('%s() expected float, int, long, '
                            'str (with optional base), Integer, LongInteger.' %
                            IntegerType.__name__)
        
        IntegerType.__init__raw(self)

    IntegerType.__init__ = __init__

fixInteger(Integer)
fixInteger(LargeInteger)
