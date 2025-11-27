# 1. 選擇一個基礎鏡像
# [cite_start]Lab 說可以用 'node' [cite: 8]，我們用 node 18 的 alpine (輕量) 版本
FROM node:18-alpine

# 2. 在 container 內部設定一個工作目錄
WORKDIR /app

# 3. 複製 package.json 和 lock 檔案
#    (只先複製這兩個檔案可以利用 Docker 的快取)
COPY package*.json ./

# 4. 安裝專案相依套件
RUN npm install

# 5. 複製所有專案檔案到工作目錄
COPY . .

# 6. 執行 'build' 指令，產生靜態檔案
#    (請檢查你的 package.json，確認 'build' 指令存在)
RUN npm run build --legacy-peer-deps

# 7. 安裝一個簡單的 HTTP 伺服器來提供 build 好的檔案
RUN npm install -g serve

# 8. 暴露(Expose) container 的 port (這裡假設是 3000)
#    'serve' 預設會跑在 3000 port
EXPOSE 3000

# 9. 啟動 container 時要執行的指令：
#    用 'serve' 來伺服 'build' 資料夾的內容
CMD ["serve", "-s", "dist", "-l", "3000"]
