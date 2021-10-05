import os
import sqlite3
import sys

from Utilities.logger import logger

# Relative to project root.
DB_PATH_FILE = "./Database/aurora.db"


class Database:
    current_table = ""

    conn = None

    def __init__(self):

        try:
            self.conn = sqlite3.connect(os.path.abspath(DB_PATH_FILE))
        except:
            logger.error(f"Error while connecting to the database")
            return None

    def closeConnection(self):
        self.conn.close()
        logger.info(f"Closing connection to the database")

    def cursor(self):
        return self.conn.cursor()

    def commit(self):
        self.conn.commit()

    def prepare(self, table_name):
        self.current_table = table_name

    def cleanUp(self):
        self.current_table = ""

    def do(self, query, custom_cursor=False):
        try:
            c = custom_cursor or self.conn.cursor()

            # Logging the query
            logger.debug(f"Running query: {query}")
            c.execute(query)

            if not custom_cursor:
                self.conn.commit()

            return c
        except Exception as e:
            logger.warn(f"SQLite: {e}")
            pass

    def _getAll(self, query):
        return self.do(query).fetchall()

    def _getOne(self, query):
        return self.do(query).fetchone()

    # High level functions:

    def tableExists(self, table_name):
        self.prepare(table_name)

        item_to_return = self.do(f"SELECT name FROM sqlite_master WHERE type='table' AND name='{table_name}';").fetchone()

        self.cleanUp()
        return item_to_return

    def insertMany(self, table_name, values):
        self.prepare(table_name)

        value_queries = []

        for value in values:
            mapped_values = map(lambda e: f"'{e}'", value.values())
            value_str = f"({', '.join(mapped_values)})"
            value_queries.append(value_str)

        item_to_return = self.do(
            f"INSERT INTO {table_name} ({', '.join(values[0].keys())}) VALUES {', '.join(value_queries)}")

        self.cleanUp()
        return item_to_return

    def insert(self, table_name, item):
        self.insertMany(table_name, [item])

    def first(self, table_name, where, select="*"):
        self.prepare(table_name)

        query = f"SELECT {select} FROM {table_name}"
        where_query = f" WHERE {where}" if where else ""
        query += where_query
        item_to_return = self._getOne(query)

        self.cleanUp()
        return item_to_return

    def createTable(self, table_name, columns):
        self.prepare(table_name)

        query_column_arr = []

        for column in columns.keys():
            query_column_arr.append(f"{column} {columns[column]}")

        query_column_str = ", ".join(query_column_arr)

        item_to_return = self.do(f"""
            CREATE TABLE IF NOT EXISTS {table_name} ( {query_column_str} )
        """)

        self.cleanUp()
        return item_to_return

    def dropTable(self, table_name):
        self.prepare(table_name)

        self.do(f"""
            DROP TABLE {table_name}
        """)

        self.cleanUp()
        return None;

    def makeUnique(self, table_name, column):
        self.prepare(table_name)

        self.do(f"CREATE UNIQUE INDEX {table_name}_{column}_uindex ON {table_name} ({column})")

        self.cleanUp()

    # Column types:

    def text(self):
        return f'TEXT'

    def datetime(self):
        return f"DATETIME DEFAULT CURRENT_TIMESTAMP"

    def int(self):
        return "INTEGER"

    def id(self, table_name):
        return f'INTEGER NOT NULL CONSTRAINT {table_name}_pk PRIMARY KEY AUTOINCREMENT'


database = Database()
