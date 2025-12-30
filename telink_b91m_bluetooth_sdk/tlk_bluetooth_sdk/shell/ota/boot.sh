#!/bin/bash  

binFile="$1"

if [ ! -f "$binFile" ]; then
    echo "Error: bin File not found." >&2
    exit 1
fi

oriBinSize=$(stat -c %s "$binFile")
trueBinSize=65536

if [ $oriBinSize -gt $trueBinSize ]; then
    echo "Error: The size of the boot bin exceeds 64k." >&2
    exit 1
fi

tempFile=$(mktemp)
cat "$binFile" > "$tempFile"

padding=$((trueBinSize - oriBinSize))
if [ $padding -gt 0 ]; then
    dd if=/dev/zero bs=1 count=$padding 2>/dev/null | tr "\000" "\377" >> "$tempFile"
fi

cat "$tempFile"

rm -f "$tempFile"