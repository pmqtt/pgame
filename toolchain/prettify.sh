#!/usr/bin/env bash
find . -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' -exec clang-format -i {} \;
