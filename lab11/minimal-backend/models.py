from typing import Optional
from sqlmodel import SQLModel, Field
from datetime import datetime


class VehicleBase(SQLModel):
    supervisor: str
    longitude: float
    latitude: float


class Vehicle(VehicleBase, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    last_updated: datetime = Field(default_factory=datetime.utcnow)


class VehicleCreate(VehicleBase):
    pass


class VehicleUpdate(SQLModel):
    supervisor: Optional[str] = None
    longitude: Optional[float] = None
    latitude: Optional[float] = None
    last_updated: Optional[datetime] = None
