# Once you've found the message encoded in the audio and converted it to a base 4 number you can use this to convert it to base10 and then
# back to the flag as a string 


def to_ascii(ctext):
    ptext = ''
    for i in range(0,len(ctext)-1,4):
        ptext += chr(int(ctext[i])*64 + int(ctext[i+1])*16 + int(ctext[i+2])*4 + int(ctext[i+3]))
    return ptext

#encoding found in spectrogram at a frequency to high to hear
base4 = "12121230120112131323132103001311113312030310123202131310113312200303120111021133123110111331"
ptext = to_ascii(base4)
print(ptext)