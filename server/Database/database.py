import os
import sqlite3

from Utilities.logger import logger
from Database.migration import run_migrations

# Relative to project root.
DB_PATH_FILE = "./Database/aurora.db"


class Database:

    conn = None

    def __init__(self):

        try:
            self.conn = sqlite3.connect(os.path.abspath(DB_PATH_FILE))
            logger.info(f"Connected to the database")
        except:
            logger.error(f"Error while connecting to the database")
            return None

        run_migrations(self)

    def closeConnection(self):
        self.conn.close()
        logger.info(f"Closing connection to the database")

    def do(self, query, without_cursor = False):
        if not without_cursor:
            c = self.conn.cursor()

        logger.debug(f"Running query: {query}")
        c.execute(query)

        if not without_cursor:
            self.conn.commit()

    def createTable(self, table_name, columns):
        query_column_arr = []

        for column in columns.keys():
            query_column_arr.append(f"{column} {columns[column]}")

        query_column_str = ", ".join(query_column_arr)

        self.do(f"""
            CREATE TABLE IF NOT EXISTS {table_name} ( {query_column_str} )
        """)

    def cursor(self):
        return self.conn.cursor()

    def commit(self):
        self.conn.commit()



database = Database()