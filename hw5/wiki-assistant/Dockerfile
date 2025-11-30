FROM python:3.12-slim

# -----------------------------------------------------
# Install system dependencies
# -----------------------------------------------------
RUN apt-get update && apt-get install -y --no-install-recommends \
    curl build-essential \
    && rm -rf /var/lib/apt/lists/*

# -----------------------------------------------------
# Install Poetry
# -----------------------------------------------------
ENV POETRY_HOME="/opt/poetry"
ENV PATH="$POETRY_HOME/bin:$PATH"
RUN curl -sSL https://install.python-poetry.org | python3 -

ENV POETRY_VIRTUALENVS_IN_PROJECT=true
ENV POETRY_NO_INTERACTION=1
ENV POETRY_CACHE_DIR='/tmp/poetry-cache'

WORKDIR /app

# -----------------------------------------------------
# Copy pyproject + lock files (cache-friendly)
# -----------------------------------------------------
COPY backend/pyproject.toml backend/poetry.lock /app/backend/
COPY frontend/pyproject.toml frontend/poetry.lock /app/frontend/

# -----------------------------------------------------
# Install backend dependencies
# -----------------------------------------------------
WORKDIR /app/backend
RUN poetry install --no-root

# -----------------------------------------------------
# Install frontend dependencies
# -----------------------------------------------------
WORKDIR /app/frontend
RUN poetry install --no-root

# -----------------------------------------------------
# Copy actual source code
# -----------------------------------------------------
WORKDIR /app
COPY backend /app/backend
COPY frontend /app/frontend

# -----------------------------------------------------
# Entrypoint
# -----------------------------------------------------
COPY entrypoint.sh /app/entrypoint.sh
RUN chmod +x /app/entrypoint.sh

EXPOSE 8000
EXPOSE 8501

CMD ["/app/entrypoint.sh"]
