# CS2107 GDB in GitHub Codespaces

GDB currently doesn't work on the SoC Compute Cluster (`kernel.yama.ptrace_scope = 2`). This repo gives you an x86-64 Ubuntu machine in your browser with `gdb`, Pwndbg, pwntools and `nc` already installed. It works on any laptop, including Apple Silicon Macs.

## 1. Start a codespace

1. Sign in to GitHub. A free account is fine.
2. On this repo's page, click **Code → Codespaces → Create codespace on main**.
   Or open https://codespaces.new/hermes-jro/cs2107-gdb-codespace
3. Wait 2–4 minutes the first time while it installs the tools. You'll get VS Code in the browser with a terminal at the bottom.
4. In the terminal, check that GDB works:

   ```
   ./scripts/check-gdb.sh
   ```

   All four checks should say `PASS`.

## 2. Use GDB as normal

Everything in the GDB Bootcamp slides works as written, including `run`, `start`, breakpoints and `pwndbg`.

```
cd demo
pwndbg ./login           # or plain: gdb ./login
pwndbg> break check_login
pwndbg> run
pwndbg> disas main
pwndbg> x/16xg $rsp
```

GDB turns off address randomisation, so addresses start with `0x5555...` and stay the same between runs, just like in the slides.

To try the bootcamp's own `vuln.c`: in the left sidebar, right-click → **Upload…** (or drag the file in), then

```
gcc -Og -o vuln vuln.c
pwndbg vuln
```

## 3. Assignment 2 challenge files

`challenges/` already contains the player files for the two pwn challenges, the same files you download from CTFd:

| Folder | Challenge | Binary |
|---|---|---|
| `challenges/e3/` | E.3 *Too Much to Say* | `overflow` (no PIE, no canary) |
| `challenges/m3/` | M.3 *Addressed to You* | `addressed-to-you` (PIE) |

Each `flag.txt` is a local placeholder, so get the real flag from the hosted service. To try one:

```
cd challenges/e3
pwndbg ./overflow
```

M.3 is PIE. GDB turns off address randomisation, so it loads at the same address every run, but the hosted service doesn't. Don't hard-code addresses you see in GDB.


For other challenges:

- **Downloadable challenges (e.g. E.1):** drag the file from your computer into the file explorer on the left. To download results, right-click a file → **Download…**
- **Uploaded binaries lose their execute permission.** Run `chmod +x ./<binary>` before `./<binary>` or `gdb ./<binary>`.
- **pwntools with GDB:** `gdb.debug()` and `gdb.attach()` open GDB in a split pane, so run your script inside `tmux` and add this line near the top:

  ```python
  context.terminal = ['tmux', 'splitw', '-h']
  ```

  Then run `tmux` in the terminal, followed by `python3 solve.py`. Plain `process('./binary')` doesn't need any of this.
- **Network challenges:** the CTFd tunnel still runs **on your own laptop**, as in the Assignment 2 PDF. The codespace is on GitHub's servers, so it can't reach `127.0.0.1:35xxx` on your laptop. Debug the binary locally in the codespace, then send your final exploit from your laptop through the tunnel.
- **Do not upload `ctfd_tunnel_key` to the codespace or to GitHub.** It's a shared course credential. `.gitignore` blocks it from commits, but don't copy it in at all.
- If you fork this repo to save your work, **make the fork private**. Don't push challenge files, solutions or flags to a public repo.

## 4. Free usage

| Account | Free per month | On the default 2-core machine |
|---|---|---|
| GitHub Free | 120 core-hours | about 60 hours |
| GitHub Pro (free with the Student Developer Pack) | 180 core-hours | about 90 hours |

- A codespace stops itself after 30 minutes of inactivity. You can also stop it at https://github.com/codespaces → **⋯ → Stop codespace**.
- With no payment method on your account, it simply stops when the free quota runs out. You won't be charged.
- Delete codespaces you're done with to free storage (15 GB free).

## 5. Troubleshooting

| Problem | Fix |
|---|---|
| `pwndbg: command not found` | Open a new terminal, or run `source ~/.bashrc`. If it's still missing: `curl -LsSf https://install.pwndbg.re \| sh -s -- -t pwndbg-gdb -u` |
| `ptrace: Operation not permitted` | Codespace was made from an old copy of the config. Delete it and create a new one. |
| `warning: Error disabling address space randomization` | Same as above. Make sure `.devcontainer/devcontainer.json` contains `"seccomp=unconfined"`. |
| Setup failed or is slow | Command palette (Ctrl/Cmd+Shift+P) → **Codespaces: Rebuild Container** |

## How it works

`.devcontainer/devcontainer.json` starts an Ubuntu 24.04 container with two settings that GDB needs:

- `"capAdd": ["SYS_PTRACE"]` lets GDB trace programs, including attaching with `gdb -p`.
- `"securityOpt": ["seccomp=unconfined"]` lets GDB turn off address randomisation, so addresses match between runs.

`.devcontainer/setup.sh` installs gcc, gdb, gdbserver, Pwndbg (`pwndbg` command), pwntools, tmux and netcat, then builds `demo/login`.

### Alternative: run on your own machine

The same config works locally with Docker Desktop, OrbStack or Colima and the VS Code **Dev Containers** extension (or `devcontainer up` from the CLI). On Apple Silicon Macs the x86-64 container runs under emulation, where GDB usually can't trace programs. Use Codespaces instead.
