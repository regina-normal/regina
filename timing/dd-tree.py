import time

progress = 1

sigs = [ 'tLAwLvLLMvQQQkcbbdfkonmlnroqprqqsshqnhjpiapvsvnjrgfohk',
         'uLAMLvvLzLQMQQcbcbdegmlopoqnrprsssttxxjxxjlldarjggsckofxr',
         'CLAMzLvvPwzALQwvQQQkbcbdefhkmnorpvvtsrywzwzBABBAAhhjhhhkfldtgwxapcbwvwswihlili',
         'xvLvvvwMvQPPQQQQcehpjtqksntrtvoupwpsuwsvwcgacalvucahatbhapaggjgfk' ]

def enum(t, coords, which, alg):
    start = time.clock()
    if progress:
        m = ProgressTracker()
        s = NormalSurfaces.enumerate(t, coords, which, alg, m)
        while not m.isFinished():
            print m.percent(), '%'
            time.sleep(0.3)
    else:
        s = NormalSurfaces.enumerate(t, coords, which, alg)
    print s
    print time.clock() - start

for sig in sigs:
    print sig
    t = Triangulation3.fromIsoSig(sig)

    enum(t, NS_QUAD, NS_VERTEX, NS_ALG_DEFAULT)
    enum(t, NS_QUAD, NS_VERTEX, NS_VERTEX_DD)
