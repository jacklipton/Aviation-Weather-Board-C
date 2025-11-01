#!/usr/bin/env zsh
set -euo pipefail

# Deploy project files to the MicroPython board using mpremote
# Usage:
#   tools/deploy.sh [PORT]
# If PORT is omitted, it will try to auto-detect.

PORT_ARG=${1:-}

err() { echo "[deploy] $*" >&2; }
info() { echo "[deploy] $*"; }

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

if [[ ! -f "config.py" ]]; then
  err "config.py not found in repo root. Run: tools/setup_config.py"
  exit 1
fi

info "Using port: $PORT"

FILES=(
  boot.py
  connect_to_wifi.py
  light_control.py
  server.py
  AviationWeatherServer.py
  config.py
)

for f in "${FILES[@]}"; do
  if [[ -f "$f" ]]; then
    info "Uploading $f"
    mpremote connect "$PORT" fs cp "$f" :"$f"
  else
    err "Missing file: $f"
  fi
done

info "Listing files on device:"
mpremote connect "$PORT" fs ls

info "Resetting device to run boot.py"
mpremote connect "$PORT" reset
