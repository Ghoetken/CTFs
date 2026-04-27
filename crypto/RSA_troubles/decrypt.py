from sympy import mod_inverse
from math import isqrt

def is_perfect_sqr(n):
    x = isqrt(n)
    if (pow(x,2) == n):
        return True
    else:
        return False

def fermat_attack(n):
    a = (isqrt(n))
    while((pow(a,2) - n) < 0):
        a += 1
    while True:
        b_sq = pow(a,2) - n
        if(is_perfect_sqr(b_sq)):
            b = isqrt(b_sq)
            return a+b, a-b
        a += 1
    

ciphertext = open("ciphertext.txt", 'r').read()
c = int(ciphertext)
public_key = open("public_key.txt", 'r').read().strip().split(',')
e = int(public_key[0])
n = int(public_key[1])
p, q = fermat_attack(n)
phi = (p-1)*(q-1)
d = int(mod_inverse(e,phi))
m = pow(c,d,n)

byte_length = (m.bit_length() + 7) // 8
flag_recovered = m.to_bytes(byte_length, "big")
flag = flag_recovered.decode()
print(flag)

