#!/bin/sh


./a.out $1 | rg '.{2,}' | head -n 5
