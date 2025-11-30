from sqlmodel import create_engine, SQLModel, Session
from typing import Optional

DATABASE_URL = "sqlite:///./vehicles.db"

# Use a synchronous engine for simplicity
engine = create_engine(DATABASE_URL, echo=False)

def init_db():
    """Create database tables if they don't exist."""
    SQLModel.metadata.create_all(engine)

def get_session() -> Session:
    return Session(engine)
