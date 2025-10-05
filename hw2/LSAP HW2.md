# LSAP HW2

---

## 1. 謎樣的硬碟

### 1. 下載完檔案之後掛載為 loop

```bash
for i in {1..4}; do
  sudo losetup -fP disk${i}.img
done
```

```bash
$ losetup -a
# 輸出：
# /dev/loop1: []: (/home/classuser/disk2.img)
# /dev/loop2: []: (/home/classuser/disk3.img)
# /dev/loop0: []: (/home/classuser/disk1.img)
# /dev/loop3: []: (/home/classuser/disk4.img)
```

### 2. 檢查檔案資訊看看哪些是 raid 的成員

```bash
sudo mdadm --examine /dev/loop* | egrep 'loop|Array UUID|Device Role|State'
```

![截圖 2025-10-03 凌晨12.01.21](LSAP%20HW2%2028159f80387a80b085eaf179c83a2f0d/upload_d5e5ca4221ac5d9b612349eeb4e2515d.png)

可以看到 disk1, disk3, disk4 是 raid 的成員，然後 disk2 是壞掉的那個硬碟。

### 3. 確認完之後就把它們組成 raid

```bash
sudo mdadm --assemble /dev/md0 /dev/loop0 /dev/loop2 /dev/loop3
cat /proc/mdstat
```

結果：

```bash
mdadm: /dev/md0 has been started with 3 drives.
Personalities : [raid0] [raid1] [raid6] [raid5] [raid4] [raid10]
md0 : active raid5 loop0[0] loop3[3] loop2[1]
      405504 blocks super 1.2 level 5, 512k chunk, algorithm 2 [3/3] [UUU]

unused devices: <none>
```

### 4. 掛載 raid 然後查看裡面的內容

```bash
sudo mkdir -p /mnt/raid
sudo mount /dev/md0 /mnt/raid
ls /mnt/raid
```

![截圖 2025-10-03 凌晨12.15.09](LSAP%20HW2%2028159f80387a80b085eaf179c83a2f0d/upload_90489db505a9b2e5f68f65076bbd90fc.png)

看到裡面有 treasure.txt 這個檔案接著把它複製出來。

![截圖 2025-10-03 凌晨12.11.51](LSAP%20HW2%2028159f80387a80b085eaf179c83a2f0d/upload_b65cb6ac33f4b874cbcaa6a0fe22ecf1.png)

最後就得到了 secret code 是 `<IL0V3L5AP_HAPPYHOLIDAY9/28>`。

## 2. NFS auto mounting （autofs）

### 1. 建立目錄與資料夾

```bash
sudo mkdir -p /srv/nfsroot/shared/public
sudo mkdir -p /srv/nfsroot/shared/projects/group12
sudo mkdir -p /srv/nfsroot/shared/projects/HW2
sudo mkdir -p /srv/nfsroot/users/alice
sudo mkdir -p /srv/nfsroot/users/bob
sudo mkdir -p /srv/nfsroot/users/charlie
sudo mkdir -p /srv/nfsroot/services/webdata
sudo mkdir -p /srv/nfsroot/services/backups

echo "This is README" | sudo tee /srv/nfsroot/shared/public/README.txt
echo "Task for group12" | sudo tee /srv/nfsroot/shared/projects/group12/task.md
echo "To do for HW2" | sudo tee /srv/nfsroot/shared/projects/HW2/todo.txt
echo "Alice's note" | sudo tee /srv/nfsroot/users/alice/notes.txt
echo "Bob's plan" | sudo tee /srv/nfsroot/users/bob/plan.txt
echo "Charlie's secret" | sudo tee /srv/nfsroot/users/charlie/secret.txt
echo "<html>Hello</html>" | sudo tee /srv/nfsroot/services/webdata/index.html
echo "Backup on $(date)" | sudo tee /srv/nfsroot/services/backups/snapshot.txt
```

### 2. 設定權限

```bash
sudo chown -R nobody:nogroup /srv/nfsroot
sudo chmod -R 755 /srv/nfsroot
```

### 3. **設定 pseudo root 並 export**

```bash
sudo nano /etc/exports
```

```
/srv/nfsroot  140.112.106.87(rw,fsid=0,insecure,no_subtree_check)
```

套用設定：

```bash
sudo exportfs -ra
sudo systemctl restart nfs-server
```

### 4. client 端建立 map

![image.png](LSAP%20HW2%2028159f80387a80b085eaf179c83a2f0d/image.png)

### 5. 啟用 autofs

```bash
sudo systemctl restart autofs
```

### 6. 驗證自動掛載生效

`public` 目錄在使用前是未掛載的，但 `ls /mnt/nfs/public` 之後再查看掛載狀態，結果顯示 `public` 已經被自動掛載。

![image.png](LSAP%20HW2%2028159f80387a80b085eaf179c83a2f0d/image%201.png)