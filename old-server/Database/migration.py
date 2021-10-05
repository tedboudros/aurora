import importlib
from pathlib import Path
import os.path

from Utilities.logger import logger


class Migration:
    migration = None
    database = None

    migration_name = ""
    old_path = ""

    def backupAndChangePath(self):
        self.old_path = os.getcwd()
        path = Path(__file__)
        os.chdir(path.parent)

    def restorePath(self):
        os.chdir(self.old_path)

    def __init__(self, database, migration_name):
        self.backupAndChangePath()
        filepath = os.path.abspath(f"./Migrations/{migration_name}.py")
        import_name = f"Database.Migrations.{migration_name}"
        self.restorePath()

        file_exists = os.path.isfile(filepath)

        if not file_exists:
            logger.error(f'Cannot find specified migration ("{migration_name}")')
            return None

        self.migration = importlib.import_module(import_name)
        self.database = database
        self.migration_name = migration_name

    def run(self, batch):
        self.database.insert('migrations', {"name": self.migration_name})
        self.migration.up(self.database)

