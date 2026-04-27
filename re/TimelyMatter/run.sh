#!/bin/sh
cd /home/ctf
exec socat TCP-LISTEN:2993,reuseaddr,fork EXEC:./timely

