import socket, subprocess, re, io, contextlib, ast
from _thread import *


INITIAL = '''Python 3.10.18 (main, Jun 18 2025, 10:10:04) [GCC 8.5.0 20210514 (Red Hat 8.5.0-26)] on linux
Type "help", "copyright", "credits" or "license" for more information.
'''


def start_server(HOST, PORT):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()
        print("Server started...")
        while True:
            conn, addr = s.accept()
            start_new_thread(threading, (conn,))


def threading(conn):
    conn.sendall(INITIAL.encode())
    
    while True:
        conn.sendall('>>> '.encode())
        input = conn.recv(2048).decode()
        result = safe_exec(input)
        conn.sendall(f'{result}\n'.encode())
    
    conn.close()


DANGEROUS_ATTRS = {'__globals__', '__import__', 'open', 'popen', 'read'}
def is_safe(code):
    try:
        tree = ast.parse(code)
        for node in ast.walk(tree):
            if isinstance(node, ast.Attribute):
                if node.attr in DANGEROUS_ATTRS:
                    return False
            if isinstance(node, ast.Name):
                if node.id in DANGEROUS_ATTRS:
                    return False
        return True
    except Exception:
        return False


def get_sandbox_builtins():
    import builtins

    # Start with all built-ins
    safe_builtins = dict(vars(builtins))

    # List of functions to blacklist
    dangerous = [
        '__import__',
        'open',
        'popen',
        'read',
        'eval',
        'exec',
        'compile',
        'input',
        'help',
        'globals',
        'locals',
        'vars',
        'dir',
        'delattr',
        'setattr',
        # 'getattr',
        'exit', 'quit',
        '__build_class__',
        '__loader__',
    ]

    for key in dangerous:
        safe_builtins.pop(key, None)

    return safe_builtins


def des():
    import yaml
    def inner():
        return yaml
    return inner


def safe_exec(expr):
    if not is_safe(expr):
        return "Error: Forbidden code pattern detected."
    
    sandbox_globals = {
        "__builtins__": get_sandbox_builtins(),

        # Obfuscating standard libs
        "math": __import__("math"),
        "random": __import__("random"),
        "datetime": __import__("datetime"),
        "re": __import__("re"),
        "collections": __import__("collections"),
        "functools": __import__("functools"),
        "itertools": __import__("itertools"),
        "string": __import__("string"),
        "heapq": __import__("heapq"),
        "operator": __import__("operator"),

        # The actual target (hidden in noise)
        "des": des,
        
        # More obscure libs for noise
        "decimal": __import__("decimal"),
        "fractions": __import__("fractions"),
        "bisect": __import__("bisect"),
        "uuid": __import__("uuid"),
        "pprint": __import__("pprint"),
        "base64": __import__("base64"),
        "pickletools": __import__("pickletools"),
    }
    sandbox_locals = {}

    output = io.StringIO()
    try:
        with contextlib.redirect_stdout(output):
            try:
                result = eval(expr, sandbox_globals, sandbox_locals)
                if result is not None:
                    return str(result)
            except SyntaxError:
                exec(expr, sandbox_globals, sandbox_locals)
    except Exception as e:
        return f"Error: {e}"
    return output.getvalue().strip() or "None"

if __name__ == "__main__":

    start_server('0.0.0.0', 4444)
