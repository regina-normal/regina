#!/usr/bin/python
from rank import *
from manifold import *
import cmath
from krawczyk import *
import snappy
import sys
import interval
import complex

pi = interval.interval.pi()
version = 'v1.0.0'

# Returns the minimum imaginary part of all tetrahedra shapes

def min_imaginary_shapes(M):
	imaginary_shapes = []
	for i in M.tetrahedra_shapes(part = 'rect'):
		imaginary_shapes.append(i.imag)
	return min(imaginary_shapes)

def iarg(z):
	temp = atan2(z.imag.inf, z.imag.sup, z.real.inf, z.real.sup)
	return interval.interval(temp[0],temp[1])


def get_data(M):
	app_sol = [] #approximated solution, [z1.real, z1.imag, z2.real, z2.imag, ....]
	ind_mat = [] #store matrix of gluing equations
	# store equations of form 'rect'
	rect = []
	equations_rect = M.gluing_equations(form = 'rect')
	chosen_equation = [] # the i_th line we chose is chosen_equation[i]     
        num_tet = M.num_tetrahedra()
        num_cusps = M.num_cusps()
        # if a cusp is complete, we do not need consistency equation for longitude.
	ignore = 0
	for n in range(num_cusps):
                rect.append(equations_rect[num_tet + n + ignore])
		chosen_equation.append(num_tet + n + ignore)
		if M.cusp_info('complete?')[n]:
                        ignore += 1
        for n in range(num_tet):
                rect.append(equations_rect[n])
                chosen_equation.append(n)
	
	for sol in M.tetrahedra_shapes('rect'):
		app_sol.append(sol.real)
		app_sol.append(sol.imag)
	
	for i in chosen_equation:
		line = equations_rect[i]
		temp = []
		for a in range(len(line)):
			if a%3<2:
				for b in line[a]:
					temp.append(float(b))
			else:
				temp.append(float(line[a]))
		ind_mat.append(temp[:])
	return app_sol[:], ind_mat[:], chosen_equation[:]

def verify_hyperbolicity(M, print_data = False, save_data = False):
	# first check if M has positive solution.
	hikmot_data = '' # TODO: hex only
	hikmot_data = 'WARNING: Floating-point numbers are represented in computer as binary numbers. The binary floating-point numbers of output are approximated by the decimal numbers. This means the output decimal numbers are NOT correct rigorously. To avoid this warning, you can use the verify_hyperbolicity_hex(M, print_data = False, save_data = False) function alternatively. The function returns every output by the hexadecimal numbers. There is no error between the binary numbers and the hexadecimal numbers.\n' # TODO: non-hex only
	hikmot_data += 'Manifold name: ' + str(M) + '\n'
	if min_imaginary_shapes(M) < 0.0000000001:
		hikmot_data += "Approximated solution is not positive enough. Verification fails"
		if save_data:
			data_file = open(str(M) + '_hikmot_False.txt', "w")
			data_file.write(hikmot_data)
		if print_data:
			print hikmot_data					
		return False, []
	# Start main part
	# app_sol is the approximated solution by SnapPea
	# ind_mat is the index matrix
	# We chose n equations, i-th equation of our n equations is chosen_equation_all[i].
	app_sol, ind_mat, chosen_equation_all  = get_data(M)
	row_num = M.num_cusps()
	length_c = M.num_tetrahedra()

	x0 = app_sol
	row_ind = range(row_num)
	rowend = len(ind_mat)

	# Index selection by calculating matrix rank
	for i in range(row_num,rowend+1):
		if RankMat(ind_mat[:i+1]) > row_num:
			row_ind.append(i)
			row_num += 1
		if row_num == length_c:
			break
	
	chosen_equation = []
	for i in range(len(row_ind)):
		chosen_equation.append(chosen_equation_all[i])
	a = []
	b = []
	c = []
	for i in range(len(row_ind)):
		tmp = ind_mat[row_ind[i]]
		# Index matrix a, b and vector c
		a = a + tmp[:length_c]
		b = b + tmp[length_c:2*length_c]
		c.append(tmp[-1])
	hikmot_data += 'Matrix of chosen equations. See SnapPy\'s documentaion of gluing equation for the convension.\n'
	hikmot_data += str(ind_mat) + '\n'
	hikmot_data += 'Approximate solution by SnapPea:\n'
	for n in range(len(x0)):
		if n % 2 == 0:
			hikmot_data += "%+07.16f" % x0[n]         # TODO: non-hex only
			hikmot_data += '('                        # TODO: hex only
			hikmot_data += x0[n].hex()                # TODO: hex only
			hikmot_data += ')+('                      # TODO: hex only
		else:
			hikmot_data += "%+07.16f" % x0[n] + 'i\n' # TODO: non-hex only
			hikmot_data += x0[n].hex()                # TODO: hex only
			hikmot_data += ')i\n'                     # TODO: hex only
	
	list_abc = a+b+c+x0
	# Krawczyk test is here.
	x = krawczyk(length_c,list_abc)

	flag_verified = False
	ans = [None]*length_c

	if x[-1] == 1:
		n=0
		hikmot_data += 'A polished approximate solution:\n'
		for i in range(length_c):
			hikmot_data += '('                         # TODO: hex only
			hikmot_data += x[n].hex()                  # TODO: hex only
			hikmot_data += ')+('                       # TODO: hex only
			hikmot_data += x[n+1].hex()                # TODO: hex only
			hikmot_data += ')i\n'                      # TODO: hex only
			hikmot_data += "%+07.16f" % x[n]           # TODO: non-hex only
			hikmot_data += "%+07.16f" % x[n+1] + 'i\n' # TODO: non-hex only
			n += 2

		hikmot_data += 'Initial data:\n'
		for i in range(length_c):
			hikmot_data += '['
			hikmot_data += x[n].hex()          # TODO: hex only
			hikmot_data += "%+07.16f" % x[n]   # TODO: non-hex only
			hikmot_data += ', '
			hikmot_data += x[n+1].hex()        # TODO: hex only
			hikmot_data += "%+07.16f" % x[n+1] # TODO: non-hex only
			hikmot_data += ']+['
			hikmot_data += x[n+2].hex()        # TODO: hex only
			hikmot_data += "%+07.16f" % x[n+2] # TODO: non-hex only
			hikmot_data += ', '
			hikmot_data += x[n+3].hex()        # TODO: hex only
			hikmot_data += "%+07.16f" % x[n+3] # TODO: non-hex only
			hikmot_data += ']i\n'
			n += 4

		hikmot_data += 'Approximated inverse of Jacobian:\n['
		for i in range(2*length_c):
			hikmot_data += '['
			for j in range(2*length_c):
				hikmot_data += x[n].hex()        # TODO: hex only
				hikmot_data += "%+07.16f" % x[n] # TODO: non-hex only
				if j!=2*length_c-1:
					hikmot_data += ', '
				n += 1
			if i!=2*length_c-1:
				hikmot_data += '],\n'
			else:
				hikmot_data += ']]\n'

		ai = n #Index of answer
		hikmot_data += 'Verified solution:\n'
		for i in range(length_c):
			hikmot_data += '['
			hikmot_data += x[n].hex()          # TODO: hex only
			hikmot_data += "%+07.16f" % x[n]   # TODO: non-hex only
			hikmot_data += ', '
			hikmot_data += x[n+1].hex()        # TODO: hex only
			hikmot_data += "%+07.16f" % x[n+1] # TODO: non-hex only
			hikmot_data += ']+['
			hikmot_data += x[n+2].hex()        # TODO: hex only
			hikmot_data += "%+07.16f" % x[n+2] # TODO: non-hex only
			hikmot_data += ', '
			hikmot_data += x[n+3].hex()        # TODO: hex only
			hikmot_data += "%+07.16f" % x[n+3] # TODO: non-hex only
			hikmot_data += ']i\n'
			n += 4
	else:
		hikmot_data += 'Verification fails\n'
		if save_data:
			data_file = open(str(M) + '_hikmot_False.txt', "w")
			data_file.write(hikmot_data)
		if print_data:
			print hikmot_data					
		return False, ans

	for a in range(length_c):
		tmp_real = interval.interval(x[4*a+ai],x[4*a+1+ai])
		tmp_imag = interval.interval(x[4*a+2+ai],x[4*a+3+ai])
		ans[a] = complex.complex(tmp_real,tmp_imag)
	# check if the solutions are all positive
	for z in ans:
		if z.imag.inf<0 or (1/(1-z)).imag.inf<0 or ((z-1)/z).imag.inf<0:
			hikmot_data += "contains (possibly) negatively oriented tetrahedra\n Verification fails"
			if save_data:
				data_file = open(str(M) + '_hikmot_False.txt', "w")
				data_file.write(hikmot_data)
			if print_data:
				print hikmot_data

			return False, ans
	
	hikmot_data += "all positively oriented tetrahedra, rigorously ensured\n"

	# compute the sum of arguments of the solution
	result = 0
	
	# check the argument of each gluing equation.
	#
	hikmot_data += "Check argument condition \n"
	log_eq = M.gluing_equations('log')
	log2_eq = M.gluing_equations('rect')
	n = len(ans)
	ans_arg = [ans[:],ans[:],ans[:]]
	arg_verified = True
	for j in range(n):
			#z = ans[j].real.sup + ans[j].imag.sup*1j
			z = ans[j]
			ans_arg[0][j] = iarg(z)
			ans_arg[1][j] = iarg(1/(1-z))
			ans_arg[2][j] = iarg((z-1)/z)
	for i in range(len(chosen_equation)):
		result = 0 # initialize
		for j in range(n):
			result = result + log_eq[chosen_equation[i],3*j] * ans_arg[0][j] + log_eq[chosen_equation[i],3*j+1] * ans_arg[1][j] + log_eq[chosen_equation[i],3*j +2] * ans_arg[2][j]
		result = result/pi 
		# result should be 
		# 1) 0 if the equation corresponds to a cusp equation (equation for the completeness of a cusp)
		# 2) 2 otherwise (equation for a Dehn filling or an edge)	
		if i < M.num_cusps() and M.cusp_info('complete?')[i]: #see print_Moser_data for our way to choose equations.
			if result.inf > -0.01 and result.sup < 0.01: # if 0 is in result
				hikmot_data += "True, equation for " + str(i) + "th cusp(complete). The sum of arguments is in " '[' + "%+07.16f" % result.inf + ',' + "%+07.16f" % result.sup + ']' + '\n'
			else:
				hikmot_data += "False, equation for " + str(i) + "th cusp(complete). The sum of arguments is in " '[' + "%+07.16f" % result.inf + ',' + "%+07.16f" % result.sup + ']' + '\n'

				arg_verified = False
		else:
			if result.inf > 1.99 and result.sup < 2.01: # if 2.0 is in result
				if i < M.num_cusps():
					hikmot_data += "True, equation for " + str(i) + "th cusp(filled). The sum of arguments is in " '[' + "%+07.16f" % result.inf + ',' + "%+07.16f" % result.sup + ']' + '\n'
				else: 
					hikmot_data += "True, " + str(i) + "th equation for an edge. The sum of arguments is in " '[' + "%+07.16f" % result.inf + ',' + "%+07.16f" % result.sup + ']' + '\n'
			else:
				if i < M.num_cusps(): 
					hikmot_data += "False, equation for " + str(i) + "th cusp(filled). The sum of arguments is in" '[' + "%+07.16f" % result.inf + ',' + "%+07.16f" % result.sup + ']' + '\n'
				else:
					hikmot_data += "False, " + str(i) + "th equation for an edge. The sum of arguments is in " '[' + "%+07.16f" % result.inf + ',' + "%+07.16f" % result.sup + ']' + '\n'
					
				arg_verified = False
	#"""
	if arg_verified == False:
		hikmot_data += 'argument condition is not satisfied.'
		if save_data:
                        data_file = open(str(M) + '_hikmot_False.txt', "w")
                        data_file.write(hikmot_data)
                if print_data:
                        print hikmot_data

	if arg_verified and (x[-1] == 1):
		hikmot_data += 'verification suceeds'
		if save_data:
                        data_file = open(str(M) + '_hikmot_True.txt', "w")
                        data_file.write(hikmot_data)
                if print_data:
                        print hikmot_data
	return (x[-1] == 1) and arg_verified, ans[:]


