proc = open("og_proclamation.txt", 'r').read()
encryption = open("emancipation_proclamation.txt", 'r').read()

key = []
for i in range(500):
    key.append(ord(proc[i]) ^ ord(encryption[i]))

encryption = open("Pidgeons.txt", 'r').read()
k = 0
decrypt = ''
for c in encryption:
    decrypt += chr(ord(c) ^ key[k])
    k = (k+1)%500
open("decrypt.txt",'w').write(decrypt)
