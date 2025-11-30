# Minimal EV Backend

Small FastAPI backend using SQLite to store EV vehicle records.

Features:
- SQLite database (file: `vehicles.db`)
- Table `Vehicle` with columns: id, supervisor, longitude, latitude, last_updated
- Endpoints (no auth): list, get, insert, update, delete

Quick start

1. Create a virtual environment and install dependencies:

```bash
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

2. Run the server:

```bash
uvicorn main:app --reload
```

3. API endpoints available at `http://127.0.0.1:8000`:

- GET /vehicles — list all vehicles
- GET /vehicles/{id} — get one vehicle
- POST /vehicles — insert a vehicle
- PUT /vehicles/{id} — update a vehicle
- DELETE /vehicles/{id} — delete a vehicle

The OpenAPI docs are at `http://127.0.0.1:8000/docs`.
