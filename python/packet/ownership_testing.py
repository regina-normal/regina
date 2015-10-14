from regina import *

for i in range(100):

    N = NContainer()

    for i in range(5):
        N.insertChildFirst(NContainer())

    C = NContainer()
    N.insertChildAfter(C, N.getFirstTreeChild())

    print C.getTreeParent().getNumberOfChildren()
    
    N.insertChildAfter(NContainer(), N.getFirstTreeChild().getNextTreeSibling().getNextTreeSibling())
        
    C = NContainer()
    N.insertChildFirst(C)

    print N.getNumberOfChildren()
    print C.getNumberOfChildren()

    C = None
    N.insertChildFirst(NContainer())
    C = NContainer()
    N.insertChildFirst(C)
    
    for i in range(5):
        N.getLastTreeChild().insertChildFirst(NContainer())

    C2 = NContainer()
    N.getLastTreeChild().insertChildFirst(C2)

    print C2.getNumberOfChildren()
    
    print N.getNumberOfChildren()

    N = None

    r = False

    try:
        print C.getNumberOfChildren()
    except:
        r = True

    assert r

    r = False

    try:
        print C2.getNumberOfChildren()
    except:
        r = True
        
    assert r
