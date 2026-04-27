from pwn import *

libc_path = './libc.so.6'  # path to your libc to patch

libc = ELF(libc_path)

# 1. Overwrite /bin/sh string
# binsh_addr = next(libc.search(b'/bin/sh\x00'))
# print(f"[*] /bin/sh found at {hex(binsh_addr)}")

# with open(libc_path, 'r+b') as f:
#     f.seek(binsh_addr)
#     f.write(b'XXXXXXX')  # Replace with nonsense bytes

# 2. Patch 'system' function start to ret (opcode 0xc3)
if 'system' in libc.symbols:
    system_addr = libc.symbols['system']
    print(f"[*] system found at {hex(system_addr)}")
    with open(libc_path, 'r+b') as f:
        f.seek(system_addr)
        f.write(b'\xc3')  # ret instruction

# 3. Patch 'execve' function start to ret (opcode 0xc3)
if 'execve' in libc.symbols:
    execve_addr = libc.symbols['execve']
    print(f"[*] execve found at {hex(execve_addr)}")
    with open(libc_path, 'r+b') as f:
        f.seek(execve_addr)
        f.write(b'\xc3')  # ret instruction
        
# 3. Patch 'execve' function start to ret (opcode 0xc3)
# if 'read' in libc.symbols:
#     read_addr = libc.symbols['read']
#     print(f"[*] read found at {hex(read_addr)}")
#     with open(libc_path, 'r+b') as f:
#         f.seek(read_addr)
#         f.write(b'\xc3')  # ret instruction

print("[*] Patching complete.")