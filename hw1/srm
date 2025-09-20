#!/usr/bin/env bash
set -euo pipefail

usage() { echo "Usage: srm [-r] <path...>"; exit 1; }

RECURSE=0
if [[ "${1-}" == "-r" ]]; then RECURSE=1; shift; fi
(( $# >= 1 )) || usage

# 當用 sudo 執行時，仍要把垃圾桶放在原使用者家目錄（不是 /root）
if [[ -n "${SUDO_USER-}" && "$SUDO_USER" != "root" ]]; then
  HOME_DIR="$(eval echo "~$SUDO_USER")"
else
  HOME_DIR="$HOME"
fi

TRASH_ROOT="$HOME_DIR/.trash"
FILES_DIR="$TRASH_ROOT/files"
META_DIR="$TRASH_ROOT/meta"
mkdir -p "$FILES_DIR" "$META_DIR"

iso_now() { date "+%Y-%m-%dT%H:%M:%S%:z"; }         # ISO8601 帶時區
today()  { date "+%F"; }                             # YYYY-MM-DD

# 取得大小（檔案用 stat，資料夾用 du -sb）
size_bytes() {
  local p="$1"
  if [[ -d "$p" ]]; then du -sb -- "$p" | cut -f1; else stat -c%s -- "$p"; fi
}

for target in "$@"; do
  if [[ ! -e "$target" ]]; then
    echo "srm: $target: No such file or directory" >&2; continue
  fi
  if [[ -d "$target" && $RECURSE -ne 1 ]]; then
    echo "srm: cannot remove $target, is a directory (use -r to trash recursively)" >&2
    continue
  fi

  base="$(basename -- "$target")"
  datepart="$(today)"
  pid="$$"
  counter=0
  while :; do
    key="${base}.${datepart}.${pid}.${counter}"
    if [[ ! -e "$FILES_DIR/$key.tar.gz" && ! -e "$META_DIR/$key.meta.csv" ]]; then
      break
    fi
    ((counter++))
  done

  sz="$(size_bytes "$target")"
  iso="$(iso_now)"

  # 打包：僅收錄 basename，避免存入絕對路徑
  archive="$FILES_DIR/$key.tar.gz"
  ( cd "$(dirname -- "$target")"
    tar -czf "$archive" -- "$(basename -- "$target")"
  )

  # 成功才移除原檔/夾
  if [[ -d "$target" ]]; then rm -rf -- "$target"; else rm -f -- "$target"; fi || true

  # 寫入中繼資料（三欄）
  echo "$target,$iso,$sz" > "$META_DIR/$key.meta.csv"

  echo "Trashed: $target"
done