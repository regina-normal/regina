import time

sigs = [ 'tLAwLvLLMvQQQkcbbdfkonmlnroqprqqsshqnhjpiapvsvnjrgfohk',
         'uLAMLvvLzLQMQQcbcbdegmlopoqnrprsssttxxjxxjlldarjggsckofxr',
         'CLAMzLvvPwzALQwvQQQkbcbdefhkmnorpvvtsrywzwzBABBAAhhjhhhkfldtgwxapcbwvwswihlili',
         'xvLvvvwMvQPPQQQQcehpjtqksntrtvoupwpsuwsvwcgacalvucahatbhapaggjgfk' ]

for sig in sigs:
    print sig
    t = NTriangulation.fromIsoSig(sig)

    start = time.clock()
    s = NNormalSurfaceList.enumerate(t, NS_QUAD, NS_VERTEX)
    print s
    print time.clock() - start

    start = time.clock()
    u = NNormalSurfaceList.enumerate(t, NS_QUAD, NS_VERTEX, NS_VERTEX_DD)
    print u
    print time.clock() - start
