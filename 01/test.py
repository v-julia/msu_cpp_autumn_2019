import subprocess

encoding = 'utf-8'

def check1(expr, result, rcode):
    cm = './calco ' + expr
    out =  subprocess.run(cm, stdout = subprocess.PIPE, shell=True)
    #if out.returncode == 0
    
    if out.returncode == rcode and result == str(out.stdout, encoding):
        print('{} = {}; passed'.format(expr, result))
    else:
        print(out)
        print('error {0} != {1}, got {2}'.format(expr,result, str(out.stdout,encoding)))

check1('"2 + 2"', '4', 0)
check1('"2 + 2*2"', '6', 0)
check1('"-1 * 2 + 646/34 - -1"', '18', 0)
check1('"34 + 45/i"', 'invalid expression!', 1)
check1('"!2 + -2"', 'invalid expression!', 1)
check1('"2/2 + 2 - 650/34"', '-16', 0)
