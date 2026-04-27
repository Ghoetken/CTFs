def XOR(ptext,key):
    ctext = []
    k = 0
    for c in ptext:
        xor_byte = (ord(c) ^ ord(key[k]))
        ctext.append(f"{xor_byte:02x}")
        k = (k + 1) % len(key)
    return ''.join(ctext)
    
flag = "flag{gr34t_X0Rk!_B3t_y0u_d1dnt_3xpect_~!!}"
key = "IAmKe~"
ctext = XOR(flag, key)
print(ctext)