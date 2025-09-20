## 1. Service 檔案

儲存路徑：`~/.config/systemd/user/srm-empty.service`

```ini
[Unit]
Description=Smart Trash Can - purge old items

[Service]
Type=oneshot
ExecStart=%h/bin/srm-empty
Environment=TRASH_MAX_AGE_DAYS=7
```

---

## 2. Timer 檔案

儲存路徑：`~/.config/systemd/user/srm-empty.timer`

```ini
[Unit]
Description=Run srm-empty daily

[Timer]
OnCalendar=daily
Persistent=true

[Install]
WantedBy=timers.target
```

---

## 3. 建立與啟用步驟

```bash
mkdir -p ~/.config/systemd/user
nano ~/.config/systemd/user/srm-empty.service   # 貼上 Service 內容
nano ~/.config/systemd/user/srm-empty.timer     # 貼上 Timer 內容

systemctl --user daemon-reload
systemctl --user enable --now srm-empty.timer
```

啟用後可用以下指令確認計時器狀態：

```bash
systemctl --user list-timers srm-empty.timer
```

此設定會在每天凌晨 00:00 自動執行 `srm-empty` 指令，清除超過設定天數的垃圾桶內容。
