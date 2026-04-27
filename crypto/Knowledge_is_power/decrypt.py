encryption = "2f2d0c2c1e193b72593f3a267913066a3a3c7a353232550b16255c2f0b0a1672153b001d3d1e136a4403"
#find key
encryption_bytes = bytes.fromhex(encryption)
known = 'flag{'
key = ''
for i in range(len(known)):
    key = key + chr((encryption_bytes[i] ^ ord(known[i])))
key = key + chr((ord('}') ^ encryption_bytes[len(encryption_bytes) -1]))
print(f"key: {key}")
#decrypt
flag = ''
k = 0
for b in encryption_bytes:
    flag = flag + chr((b ^ ord(key[k])))
    k = (k+1)%len(key)
print(f"Flag: {flag}")