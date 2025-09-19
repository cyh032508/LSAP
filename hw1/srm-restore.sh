#!/usr/bin/env bash
set -euo pipefail

# --- 綁定到真實終端：避免 stdin 被管線/重導向影響 ---
if [[ -r /dev/tty ]]; then
  exec </dev/tty
fi

# 找到正確家目錄（即使用 sudo）
if [[ -n "${SUDO_USER-}" && "$SUDO_USER" != "root" ]]; then
  HOME_DIR="$(eval echo "~$SUDO_USER")"
else
  HOME_DIR="$HOME"
fi

TRASH_ROOT="$HOME_DIR/.trash"
FILES_DIR="$TRASH_ROOT/files"
META_DIR="$TRASH_ROOT/meta"

# 收集中繼資料
shopt -s nullglob
metas=( "$META_DIR"/*.meta.csv )
shopt -u nullglob
if (( ${#metas[@]} == 0 )); then
  echo "Trash is empty."
  exit 0
fi

# 表頭/分隔線（欄寬與資料列一致）
printf -- "%5s | %-25s | %9s | %-4s | %-30s | %s\n" \
  "Index" "Deleted ISO8601" "Size" "Type" "Key" "Original Path"
printf -- "----- | ------------------------- | --------- | ---- | ------------------------------ | ------------------------------\n"

# 建索引
declare -a KEYS ORIGS TIMES SIZES TYPES ARCHS
idx=0
for meta in "${metas[@]}"; do
  key="$(basename -- "$meta")"; key="${key%.meta.csv}"
  IFS=, read -r orig ts sz < "$meta" || true

  arch="$FILES_DIR/$key.tar.gz"
  first="$(tar -tzf "$arch" 2>/dev/null | head -n1 || true)"
  if [[ "$first" == */ ]]; then typ="DIR"; else typ="FILE"; fi

  KEYS[idx]="$key"; ORIGS[idx]="$orig"; TIMES[idx]="$ts"; SIZES[idx]="$sz"; TYPES[idx]="$typ"; ARCHS[idx]="$arch"

  printf -- "%5d | %-25s | %9s | %-4s | %-30s | %s\n" \
    "$idx" "${TIMES[idx]}" "${SIZES[idx]}" "${TYPES[idx]}" "${KEYS[idx]}" "${ORIGS[idx]}"
  ((idx++))
done

# --- 互動 / 非互動兩用 ---
pick=""
if (( $# >= 1 )); then
  pick="$1"
else
  if ! read -r -p $'Enter Index to restore: ' pick; then
    echo "No input received. You can run:  srm-restore <INDEX>" >&2
    exit 1
  fi
fi

# 驗證索引
if ! [[ "$pick" =~ ^[0-9]+$ ]]; then
  echo "Invalid index." >&2; exit 1
fi
if (( pick < 0 || pick >= idx )); then
  echo "Invalid index." >&2; exit 1
fi

# 取值
key="${KEYS[pick]}"; orig="${ORIGS[pick]}"; arch="${ARCHS[pick]}"
name="$(basename -- "$orig")"; parent="$(dirname -- "$orig")"

mkdir -p -- "$parent"
dest="$parent/$name"
if [[ -e "$dest" ]]; then
  suffix="$(date +%Y%m%d-%H:%M)"
  dest="$parent/${name}.restored.$suffix"
  echo "Original exists; restoring to: $dest"
fi

tmpdir="$(mktemp -d)"
trap 'rm -rf "$tmpdir"' EXIT
tar -xzf "$arch" -C "$tmpdir"

# 搬運
if [[ -e "$tmpdir/$name" ]]; then
  mv -- "$tmpdir/$name" "$dest"
else
  first="$(ls -1A "$tmpdir" | head -n1 || true)"
  mv -- "$tmpdir/$first" "$dest"
fi

# 清垃圾桶
rm -f -- "$arch" "$META_DIR/$key.meta.csv"

echo "Restored to: $dest"