from sqlalchemy import Column, Integer, String, DateTime
from sqlalchemy.ext.declarative import declarative_base

Base = declarative_base()


class Migration(Base):
    __tablename__ = "migrations"
    id = Column(Integer, primary_key=True)
    timestamp = Column(DateTime)
    name = Column(String)
    batch = Column(Integer)


class Seeder(Base):
    __tablename__ = "seeders"
    id = Column(Integer, primary_key=True)
    first_name = Column(String)
    last_name = Column(String)