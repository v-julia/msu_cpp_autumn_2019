import subprocess

encoding = 'utf-8'

def check1(expr, result, rcode):
    cm = './calco ' + expr
    out =  subprocess.run(cm, stdout = subprocess.PIPE, shell=True)

    #print(out)
    if out.returncode == rcode and result == str(out.stdout, encoding):
        print('{} = {}; passed'.format(expr, result))
    else:
        print(out)
        print('error {0} != {1}, got {2}'.format(expr,result, str(out.stdout,encoding)))


check1('"2 + 2"', '4', 0)
check1('"2 + 2*2"', '6', 0)
check1('"2/2 + 2 - 650/34"', '-16', 0)
check1('"-1 * 2 + 646/34 - -1"', '18', 0)

check1('', '', 1)

check1('"24*3 + "', '', 2)

check1('"34 + * 2"', '', 3)
check1('"34 + 45/i"', '', 3)


check1('"11*2 + 35 ! 7"', '', 4)
check1('"2 ! -2"', '', 4)

check1('"34/0 + 34 -2*2"', '', 5)

