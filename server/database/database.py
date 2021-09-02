import os
import sqlite3

from Utilities.logger import logger

def init_database():
    conn = sqlite3.connect(os.path.abspath('./database/aurora.db'))
    c = conn.cursor()

    c.execute("""
            CREATE TABLE if not exists games (
                name text,
                image text
            )
        """)

    conn.commit()
    conn.close()
    logger.info(f"Connected to database")



