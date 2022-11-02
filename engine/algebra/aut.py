
# The code below generates the hard-coded minimalAutGroup arrays in covers.cpp.
#
# It was designed to run under Regina 6.0.
#
# - Ben Burton, 24 July 2021

def genAut(cls, code2 = False):
    results = []
    maxAut = 0
    for p in cls.Sn:
        aut = []    # all automorphisms
        idemp = []  # all automorphisms that are their own inverse
        other = []  # all automorphisms that are not their own inverse

        minimal = True
        for j in cls.Sn:
            conj = j * p * j.inverse()
            if conj.SnIndex() < p.SnIndex():
                minimal = False
                break
            elif conj == p:
                idx = j.SnIndex()
                aut.append(idx)
                if j == j.inverse():
                    idemp.append(idx)
                else:
                    other.append(idx)
        if minimal:
            if len(aut) == cls.nPerms:
                aut = []
                idemp = []
                other = []
            elif len(aut) > maxAut:
                maxAut = len(aut)
            results.append((p.SnIndex(), aut, idemp, other))

    print('Max #aut:', maxAut)
    for line in range(len(results)):
        r = results[line]
        code = '/* ' + str(r[0]) + ' */ {'
        if len(r[1]) == cls.nPerms:
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

genAut(Perm2)
print()
genAut(Perm3)
print()
genAut(Perm4, True)
print()
genAut(Perm5, True)
print()
genAut(Perm6, True)
print()
genAut(Perm7, True)
