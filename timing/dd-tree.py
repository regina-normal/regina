import math
import time

sigs = [ 'tLAwLvLLMvQQQkcbbdfkonmlnroqprqqsshqnhjpiapvsvnjrgfohk',
         'uLAMLvvLzLQMQQcbcbdegmlopoqnrprsssttxxjxxjlldarjggsckofxr',
         'CLAMzLvvPwzALQwvQQQkbcbdefhkmnorpvvtsrywzwzBABBAAhhjhhhkfldtgwxapcbwvwswihlili',
         'xvLvvvwMvQPPQQQQcehpjtqksntrtvoupwpsuwsvwcgacalvucahatbhapaggjgfk' ]

def bits(t):
    m = makeMatchingEquations(t, NS_QUAD)
    norm = []
    for i in range(m.columns()):
        n = 0
        for j in range(m.rows()):
            e = m.entry(j, i).longValue()
            n = n + e * e
        norm.append(n)
    norm.sort()
    prod = 1
    for i in range(m.columns() - m.rows(), m.columns()):
        prod = prod * norm[i]
    return math.log(prod, 2) / 2

for sig in sigs:
    print sig
    t = NTriangulation.fromIsoSig(sig)
    print 'Bits:', bits(t)

    start = time.clock()
    s = NNormalSurfaceList.enumerate(t, NS_QUAD, NS_VERTEX)
    print s
    print time.clock() - start

    start = time.clock()
    u = NNormalSurfaceList.enumerate(t, NS_QUAD, NS_VERTEX, NS_VERTEX_DD)
    print u
    print time.clock() - start
