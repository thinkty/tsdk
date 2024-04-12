#!/bin/bash

cat $1
numbers=$(cat $1 | tr -d '\n')
for ((i=0; i<80; i+=2)); do
	printf "0x%s, " "${numbers:i:2}"
done
printf "0x%s0\n" "${numbers:80:1}"
