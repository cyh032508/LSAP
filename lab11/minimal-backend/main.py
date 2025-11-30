from fastapi import FastAPI, HTTPException
from typing import List
from sqlmodel import select
from database import init_db, get_session
from models import Vehicle, VehicleCreate, VehicleUpdate
from datetime import datetime

app = FastAPI(title="Minimal EV Backend")


@app.on_event("startup")
def on_startup():
    init_db()


@app.get("/vehicles", response_model=List[Vehicle])
def list_vehicles():
    with get_session() as session:
        vehicles = session.exec(select(Vehicle)).all()
        return vehicles


@app.get("/vehicles/{vehicle_id}", response_model=Vehicle)
def get_vehicle(vehicle_id: int):
    with get_session() as session:
        vehicle = session.get(Vehicle, vehicle_id)
        if not vehicle:
            raise HTTPException(status_code=404, detail="Vehicle not found")
        return vehicle


@app.post("/vehicles", response_model=Vehicle, status_code=201)
def create_vehicle(v: VehicleCreate):
    vehicle = Vehicle.from_orm(v)
    vehicle.last_updated = datetime.utcnow()
    with get_session() as session:
        session.add(vehicle)
        session.commit()
        session.refresh(vehicle)
        return vehicle


@app.put("/vehicles/{vehicle_id}", response_model=Vehicle)
def update_vehicle(vehicle_id: int, v: VehicleUpdate):
    with get_session() as session:
        vehicle = session.get(Vehicle, vehicle_id)
        if not vehicle:
            raise HTTPException(status_code=404, detail="Vehicle not found")

        vehicle_data = v.dict(exclude_unset=True)
        for key, value in vehicle_data.items():
            setattr(vehicle, key, value)
        vehicle.last_updated = datetime.utcnow()

        session.add(vehicle)
        session.commit()
        session.refresh(vehicle)
        return vehicle


@app.delete("/vehicles/{vehicle_id}", status_code=204)
def delete_vehicle(vehicle_id: int):
    with get_session() as session:
        vehicle = session.get(Vehicle, vehicle_id)
        if not vehicle:
            raise HTTPException(status_code=404, detail="Vehicle not found")
        session.delete(vehicle)
        session.commit()
        return None
