#!/bin/bash  

#8KB ALL FF
head -c 8192 /dev/zero | tr '\0' '\377'