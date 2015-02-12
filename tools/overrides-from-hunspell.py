#!/usr/bin/python3

import hunspell
import sys

if len(sys.argv) != 3:
    print("Usage: ./overrides-from-hunspell /usr/share/hunspell/en_US.dic /usr/share/hunspell/en_US.aff")
    sys.exit(1)

hun = hunspell.HunSpell(sys.argv[1], sys.argv[2])

f = open(sys.argv[1], "r", errors='ignore')
lines = f.readlines()
for line in lines:
    word = line.strip().split("/")[0]
    asInput = word.lower().replace("'", "")
    if "'" in word and not hun.spell(asInput):
        print(asInput + "," + word)
