#!/bin/sh
cd /home/ctf
exec socat TCP-LISTEN:2333,reuseaddr,fork EXEC:./LockBox
