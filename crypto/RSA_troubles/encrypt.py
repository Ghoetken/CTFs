import random
from sympy import isprime, mod_inverse

class RSAKeyPair:
    def __init__(self, e, d, n, p, q):
        self.public = (e, n)
        self.private = (d, n)
        self.p = p
        self.q = q

    def encrypt(self, m):
        e, n = self.public
        return pow(m, e, n)

    def decrypt(self, c):
        d, n = self.private
        return pow(c, d, n)


def generate_large_primes(bits):
    while True:
        p = random.getrandbits(bits)
        p |= ((1 << bits - 1) | 1) 
        if isprime(p):
            break
    q = p + 1
    while True:
        if isprime(q):
            return p,q
        q += 1

def generate_rsa_keypair(bits):
    e = 65537
    half_bits = bits // 2
    while True:
        p, q = generate_large_primes(half_bits)
        if p != q:
            break
    n = p * q
    phi = (p - 1) * (q - 1)

    try:
        d = mod_inverse(e, phi)
    except ValueError:
        return generate_rsa_keypair(bits)
    return RSAKeyPair(e,d,n,p,q)

rsakey = generate_rsa_keypair(2048)
flag = b"flag{f01l3d_by_f3rm4t}"
m = int.from_bytes(flag, "big")
public_key = str(rsakey.public[0]) + ',' + str(rsakey.public[1]) 
keyfd = open("public_key.txt", 'w')
keyfd.write(public_key)
cipherfd = open("ciphertext.txt", 'w')
cipherfd.write(str(rsakey.encrypt(m)))

