#!/usr/bin/env zsh
set -euo pipefail

# Open a MicroPython REPL using mpremote
# Usage:
#   tools/repl.sh [PORT]

PORT_ARG=${1:-}

err() { echo "[repl] $*" >&2; }
info() { echo "[repl] $*"; }

if ! command -v mpremote >/dev/null 2>&1; then
  err "mpremote not found. Install with: python3 -m pip install --user mpremote"
  exit 1
fi

if [[ -n "$PORT_ARG" ]]; then
  PORT="$PORT_ARG"
else
  PORT=$(ls -1 /dev/tty.usb* /dev/tty.SLAB* /dev/tty.wchusb* 2>/dev/null | head -n1 || true)
fi

if [[ -z "${PORT:-}" ]]; then
  err "Could not detect serial port automatically. Please specify it explicitly."
  exit 1
fi

info "Connecting REPL on $PORT (Ctrl-] to exit)"
mpremote connect "$PORT" repl
