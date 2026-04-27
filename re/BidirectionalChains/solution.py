global_f = [
    0x1d, 0x63, 0xdb, 0x9b, 0xe3,
    0x11, 0x81, 0xcf, 0x2f, 0xfb, 
    0x63, 0x55, 0xc1, 0xdf
]

global_b = [
    0xeb, 0xe5, 0xfb, 0xc3, 0xcf,
    0xb7, 0x53, 0x83, 0x9d, 0x2b, 
    0xfb, 0x0d, 0x43, 0x87
]

def encodef(p, c):
    return ((c + p) * p + c * p) & 0xff

def encodeb(c, n):
    return (n + c * c * n - c) & 0xff

flag = ""

for i in range(14):
    for c in range(0x20, 0x7F):  
        if i == 0:
            is_match = (encodef(0xA3, c) == global_f[i]) and (encodeb(c, global_b[i+1]) == global_b[i])
        elif i == 13:
            is_match = (encodef(global_f[i-1], c) == global_f[i]) and (encodeb(c, 0x4B) == global_b[i])
        else:
            is_match = (encodef(global_f[i-1], c) == global_f[i]) and (encodeb(c, global_b[i+1]) == global_b[i])
        
        if is_match:
            flag += chr(c)
            break

flag = "flag{" + flag + "}"
print(flag)
