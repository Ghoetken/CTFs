#!/bin/sh
cd /home/ctf
exec socat TCP-LISTEN:2223,reuseaddr,fork EXEC:./high_score