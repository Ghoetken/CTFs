#!/bin/sh
cd /home/ctf
exec socat TCP-LISTEN:2773,reuseaddr,fork EXEC:./SeedSurfing