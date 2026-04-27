def to_digits(ctext):
    ptext = ''
    for c in ctext:
        if c == '(':
            ptext += '0'
        if c == '^':
            ptext += '1'
        if c == '*':
            ptext += '2'
        if c == '/':
            ptext += '3'
        if c == '+':
            ptext += '4'
        if c == '-':
            ptext += '5'
    return ptext



def to_ascii(ctext):
    ptext = ''
    for i in range(0,len(ctext)-1,3):
        ptext += chr(int(int(ctext[i])*36 + int(ctext[i+1])*6 + int(ctext[i+2])))
    return ptext


ctext = "*-()/()()*+^*-^/*/**-^*()/^/*/-*++^*()/()*/^**/-/^^/()+*+-^*+*--*/-^*()/()+^*//^()*+^/^**-//()//()**-//^^*-*^+//*-"
ptext = to_ascii(to_digits(ctext))
print(ptext)
