# --- Dockerfile (後端) ---

# 1. 選擇一個 Python 基礎鏡像
FROM python:3.9-slim

# 2. 設定工作目錄
WORKDIR /app

# 3. 複製 requirements.txt 檔案
COPY requirements.txt .

# 4. 安裝 Python 相依套件
RUN pip install --no-cache-dir -r requirements.txt

# 5. 複製所有專案檔案
COPY . .

# 6. 暴露(Expose) container 的 port
EXPOSE 8000

# 7. 設定啟動指令 (使用 uvicorn 啟動 main.py 裡的 app)
CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]
