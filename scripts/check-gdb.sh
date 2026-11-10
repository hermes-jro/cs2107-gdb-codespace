#!/usr/bin/env bash
# Checks that GDB works in this environment. Prints PASS/FAIL for each check.
set -u
cd "$(dirname "$0")/../demo"
[ -x login ] || make -s

pass=0; fail=0
ok()  { echo "PASS  $1"; pass=$((pass+1)); }
bad() { echo "FAIL  $1"; echo "      $2"; fail=$((fail+1)); }

echo "Arch: $(uname -m)   ptrace_scope: $(cat /proc/sys/kernel/yama/ptrace_scope 2>/dev/null || echo n/a)"
echo

# 1. gdb run + breakpoint
out=$(echo AAAA | gdb -q -batch -ex 'break check_login' -ex run -ex 'info registers rip' ./login 2>&1)
if grep -q 'Breakpoint 1, .*check_login' <<<"$out"; then ok "gdb run stops at a breakpoint"
else bad "gdb run stops at a breakpoint" "$(grep -m1 -iE 'ptrace|error|warning' <<<"$out")"; fi

# 2. ASLR disabled by gdb (addresses start with 0x5555...)
if grep -qE 'rip +0x5555' <<<"$out"; then ok "gdb disables ASLR (rip = $(grep -oE '0x5555[0-9a-f]+' <<<"$out" | head -1))"
else bad "gdb disables ASLR" "$(grep -m1 -E 'rip|randomization' <<<"$out")"; fi

# 3. attach to a running process
sleep 30 & pid=$!
out=$(gdb -q -batch -p "$pid" -ex 'info registers rip' 2>&1)
kill "$pid" 2>/dev/null
if grep -qE '^rip ' <<<"$out"; then ok "gdb -p attaches to a running process"
else bad "gdb -p attaches to a running process" "$(grep -m1 -iE 'ptrace|error' <<<"$out")"; fi

# 4. pwndbg present
if command -v pwndbg >/dev/null; then ok "pwndbg installed ($(command -v pwndbg))"
else bad "pwndbg installed" "run: curl -LsSf https://install.pwndbg.re | sh -s -- -t pwndbg-gdb -u"; fi

echo
echo "$pass passed, $fail failed"
[ "$fail" -eq 0 ]
