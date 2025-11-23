#!/usr/bin/env zsh
set -euo pipefail

# Deploy project files to the MicroPython board using mpremote
# Usage:
#   tools/deploy.sh [PORT]
# If PORT is omitted, it will try to auto-detect.

PORT_ARG=${1:-}

err() { echo "[deploy] $*" >&2; }
info() { echo "[deploy] $*"; }

# Prefer the project's .venv Python if present, otherwise fall back to python3
PY_BIN=""
if [[ -x ".venv/bin/python3" ]]; then
  PY_BIN=".venv/bin/python3"
elif [[ -x ".venv/bin/python" ]]; then
  PY_BIN=".venv/bin/python"
else
  PY_BIN="$(command -v python3 || true)"
fi

if [[ -z "$PY_BIN" ]]; then
  err "No Python interpreter found (.venv/bin/python3 or python3)."
  exit 1
fi

# Use mpremote via the interpreter to ensure the correct environment
MPREMOTE=("$PY_BIN" -m mpremote)

# Quick check that mpremote is importable in the chosen interpreter
if ! "${MPREMOTE[@]}" --version >/dev/null 2>&1; then
  err "mpremote is not available in $PY_BIN. Install it into your environment:"
  err "  $PY_BIN -m pip install mpremote"
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

ensure_ready() {
  # Try to get the device into a clean state/raw REPL before uploading
  # Ignore failures here; the retry loop below will handle it per-file
  "${MPREMOTE[@]}" connect "$PORT" soft-reset || true
  sleep 0.5
}

upload_with_retry() {
  local f="$1"
  local attempts=0
  local max_attempts=3
  while (( attempts < max_attempts )); do
    ((attempts++))
    info "Uploading $f (attempt ${attempts}/${max_attempts})"
    if "${MPREMOTE[@]}" connect "$PORT" fs cp "$f" :"$f"; then
      return 0
    fi
    err "Upload failed for $f; attempting soft-reset and retry..."
    "${MPREMOTE[@]}" connect "$PORT" soft-reset || true
    sleep 0.7
  done
  return 1
}

ensure_ready

for f in "${FILES[@]}"; do
  if [[ -f "$f" ]]; then
    if ! upload_with_retry "$f"; then
      err "Giving up on $f after multiple attempts"
      exit 1
    fi
  else
    err "Missing file: $f"
  fi
done

info "Listing files on device:"
"${MPREMOTE[@]}" connect "$PORT" fs ls

info "Resetting device to run boot.py"
"${MPREMOTE[@]}" connect "$PORT" reset
