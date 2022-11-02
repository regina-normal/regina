
# The code below generates the hard-coded minimalAutGroup arrays in covers.cpp.
#
# It was designed to run under Regina 7.3.
#
# - Ben Burton, 2 November 2022

def genAut(permClass, conjClass, code2 = False):
    results = []
    maxAut = 0

    c = conjClass()
    while c:
        p = c.rep()
        c.inc()

        aut = []    # all automorphisms

        for j in permClass.Sn:
            conj = j * p * j.inverse()
            if conj.SnIndex() < p.SnIndex():
                print('ERROR: Representative not minimal!')
                break
            elif conj == p:
                idx = j.SnIndex()
                aut.append(idx)
        if len(aut) == permClass.nPerms:
            aut = []
        elif len(aut) > maxAut:
            maxAut = len(aut)
        results.append((p.SnIndex(), aut))

    print('Max #aut:', maxAut)
    for line in range(len(results)):
        r = results[line]
        code = '/* ' + str(r[0]) + ' */ {'
        if len(r[1]) == permClass.nPerms:
            code = code + '-1'
        else:
            for i in range(maxAut + 1):
                if i > 0:
                    code = code + ', '
                else:
                    code = code + ' '
                if i < len(r[1]):
                    code = code + str(r[1][i])
                elif i == len(r[1]):
                    code = code + '-1'
                    break
        code = code + ' }'
        if line != len(results) - 1:
            code = code + ','
        print(code)

genAut(Perm2, PermClass2)
print()
genAut(Perm3, PermClass3)
print()
genAut(Perm4, PermClass4, True)
print()
genAut(Perm5, PermClass5, True)
print()
genAut(Perm6, PermClass6, True)
print()
genAut(Perm7, PermClass7, True)
print()
genAut(Perm8, PermClass8)
