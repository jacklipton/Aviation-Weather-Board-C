#!/usr/bin/env zsh
set -euo pipefail

# Simple flasher for MicroPython using esptool
# Usage:
#   tools/flash.sh [esp32|esp8266] /path/to/firmware.bin [PORT]
# If PORT is omitted, it will try to auto-detect.

BOARD_TYPE=${1:-}
FW_PATH=${2:-}
PORT_ARG=${3:-}

err() { echo "[flash] $*" >&2; }
info() { echo "[flash] $*"; }

if ! command -v esptool.py >/dev/null 2>&1; then
  err "esptool.py not found. Install with: python3 -m pip install --user esptool"
  exit 1
fi

if [[ -z "$BOARD_TYPE" ]]; then
  read "?Enter board type (esp32/esp8266): " BOARD_TYPE
fi
if [[ "$BOARD_TYPE" != "esp32" && "$BOARD_TYPE" != "esp8266" ]]; then
  err "Unsupported board type: $BOARD_TYPE"
  exit 1
fi

if [[ -z "$FW_PATH" || ! -f "$FW_PATH" ]]; then
  err "Firmware path missing or not a file: $FW_PATH"
  err "Download from https://micropython.org/download/ (choose your board)"
  exit 1
fi

if [[ -n "$PORT_ARG" ]]; then
  PORT="$PORT_ARG"
else
  # Try to auto-detect a likely port on macOS
  PORT=$(ls -1 /dev/tty.usb* /dev/tty.SLAB* /dev/tty.wchusb* 2>/dev/null | head -n1 || true)
fi

if [[ -z "${PORT:-}" ]]; then
  err "Could not detect serial port automatically. Please specify it explicitly as the 3rd argument."
  exit 1
fi

info "Using port: $PORT"
info "Erasing flash..."

if [[ "$BOARD_TYPE" == "esp32" ]]; then
  esptool.py --chip esp32 --port "$PORT" erase_flash
  info "Writing firmware..."
  esptool.py --chip esp32 --port "$PORT" --baud 460800 write_flash -z 0x1000 "$FW_PATH"
else
  esptool.py --chip esp8266 --port "$PORT" erase_flash
  info "Writing firmware..."
  esptool.py --chip esp8266 --port "$PORT" --baud 460800 write_flash --flash_size=detect 0 "$FW_PATH"
fi

info "Done. Press reset (EN/RST) on the board."
