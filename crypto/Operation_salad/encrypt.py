def to_base_6(ptext):
    ctext = ''
    for c in ptext:
        ctext += str((ord(c) // 36))
        ctext += str((ord(c) // 6) % 6)
        ctext += str(ord(c) % 6)
    return ctext

def convert_to_operationish(ptext): 
    ctext = ''
    for c in ptext:
        if c == '0': 
            ctext += '()'
        if c == '1':
            ctext += '^'
        if c == '2':
            ctext += '*'
        if c == '3':
            ctext += '/'
        if c == '4':
            ctext += '+'
        if c == '5':
            ctext += '-'
    return ctext

flag = "flag{Y0u_d0nt_spe4k_0p3rationish?}"
ctext = to_base_6(flag)
ctext = convert_to_operationish(ctext)
print(ctext)