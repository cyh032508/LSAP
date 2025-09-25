---
title: HW1 Report
fontsize: 10pt        # 9pt、10pt、11pt、12pt 皆可
geometry: margin=1cm  # 可選：設定邊界
mainfont: Heiti TC   # 如果要中文字型
---
# HW1 Report

## 1. 原始碼與檔案

幾個主要檔案的位置與功能：

| 檔案                                                              | 功能                                                                                                                                |
| ----------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------- |
| `~/bin`/`srm`                                                 | 將指定檔案/目錄打包成 `tar.gz`，移到 `~/.trash/files/`，並在 `~/.trash/meta/` 建立 `.meta.csv` 紀錄原路徑、刪除時間與大小。 |
| `~/bin`/`srm-restore`                                         | 讀取 `.meta.csv` 列出清單並還原指定索引的檔案或資料夾。                                                                           |
| `~/bin/srm-empty`                                               | 刪除超過 `TRASH_MAX_AGE_DAYS`（預設 7 天）的壓縮檔與中繼資料。                                                                    |
| `~/.config/systemd/user/srm-empty.service`、`srm-empty.timer` | 定義 systemd 使用者服務與計時器，每日自動清理。                                                                                     |

所有腳本開頭均為 `#!/usr/bin/env bash`，且設為可執行 (`chmod +x`)，就可直接以 `srm file.txt` 形式呼叫，不需 `bash srm`。

---

## 2. 系統設定步驟

1. **建立目錄**

   ```bash
   mkdir -p ~/bin ~/.trash/{files,meta} ~/.config/systemd/user
   ```
2. **安裝指令**
   將三個腳本存入 `~/bin/`，並賦予執行權限：

   ```bash
   chmod +x ~/bin/srm ~/bin/srm-restore ~/bin/srm-empty
   ```
3. **加入 PATH**
   在 `~/.bashrc` 末尾加入：

   ```bash
   export PATH="$HOME/bin:$PATH"
   ```

   重新載入：

   ```bash
   source ~/.bashrc
   ```
4. **設定 systemd 計時器**
   建立 `~/.config/systemd/user/srm-empty.service`：

   ```ini
   [Unit]
   Description=Smart Trash Can - purge old items

   [Service]
   Type=oneshot
   ExecStart=%h/bin/srm-empty
   Environment=TRASH_MAX_AGE_DAYS=7
   ```

   建立 `~/.config/systemd/user/srm-empty.timer`：

   ```ini
   [Unit]
   Description=Run srm-empty daily

   [Timer]
   OnCalendar=*-*-* 00:00:00
   Persistent=true

   [Install]
   WantedBy=timers.target
   ```

   啟用與立即啟動：

   ```bash
   systemctl --user daemon-reload
   systemctl --user enable --now srm-empty.timer
   systemctl --user list-timers srm-empty.timer
   ```

---

## 3. 功能示範

### 3.1 刪除檔案

```bash
$ srm ~/lab/a.txt
```

![My Picture](./images/1.png)

垃圾桶內會生成：

![My Picture](./images/2.png)

```bash
$ srm ~/lab
```

系統會提示要加上 flag：

![My Picture](./images/3.png)

```bash
$ srm -r ~/lab
```

![My Picture](./images/4.png)

### 3.2 還原檔案

```bash
$ srm-restore
```

![My Picture](./images/5.png)

### 3.3 自動/手動清空

手動立即清空：

```bash
$ TRASH_MAX_AGE_DAYS=0 srm-empty
```

![My Picture](./images/6.png)

檢查 systemd 計時器：

```bash
$ systemctl --user list-timers srm-empty.timer
```

![My Picture](./images/7.png)
