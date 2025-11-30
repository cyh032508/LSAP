#!/bin/bash

# Start backend
cd /app/backend
poetry run uvicorn server:app --host 0.0.0.0 --port 8000 &

# Start frontend
cd /app/frontend
poetry run streamlit run chat.py \
    --server.headless=true \
    --server.address=0.0.0.0 \
    --server.port=8501 \
    --server.enableWebsocketCompression=false \
    --server.enableCORS=false
