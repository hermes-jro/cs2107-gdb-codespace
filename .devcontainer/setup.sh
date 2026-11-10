#!/usr/bin/env bash
# Runs once when the codespace is created.
set -euo pipefail

echo "==> Installing gcc, gdb, netcat, python3"
sudo apt-get update -qq
sudo DEBIAN_FRONTEND=noninteractive apt-get install -y -qq \
  build-essential gdb gdbserver tmux netcat-openbsd python3 python3-pip python3-venv file \
  >/dev/null

echo "==> Installing pwntools"
python3 -m pip install --user --quiet --break-system-packages pwntools

echo "==> Installing Pwndbg (user install, provides the 'pwndbg' command)"
curl -LsSf 'https://install.pwndbg.re' | sh -s -- -t pwndbg-gdb -u

# Avoid debuginfod download prompts on every run
grep -qs 'debuginfod' ~/.gdbinit || echo 'set debuginfod enabled off' >> ~/.gdbinit

echo "==> Building demo"
make -C demo

echo
echo "Done. Run: ./scripts/check-gdb.sh"
