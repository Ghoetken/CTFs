#!/bin/sh
cd /home/ctf
exec socat TCP-LISTEN:2113,reuseaddr,fork EXEC:./flag_on_sale
