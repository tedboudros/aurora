import importlib
from pathlib import Path
import os.path

from Utilities.logger import logger


class Migration:
    migration = None

    old_path = ""

    def backupAndChangePath(self):
        self.old_path = os.getcwd()
        path = Path(__file__)
        os.chdir(path.parent)

    def restorePath(self):
        os.chdir(self.old_path)

    def __init__(self, database, migration_file_name):
        self.backupAndChangePath()
        filepath = os.path.abspath(f"./Migrations/{migration_file_name}.py")
        import_name = f"Database.Migrations.{migration_file_name}"
        self.restorePath()

        file_exists = os.path.isfile(filepath)

        if not file_exists:
            logger.error(f'Cannot find specified migration ("{migration_file_name}")')
            return None

        self.migration = importlib.import_module(import_name)

        self.migration.run(database)


# def run(self):


def run_migrations(database):
    old_path = ""

    def getOldPathAndChangeToNew():
        temp_old_path = os.getcwd()
        path = Path(__file__)
        os.chdir(path.parent)
        return temp_old_path

    def restorePath():
        os.chdir(old_path)

    old_path = getOldPathAndChangeToNew()
    directory = os.path.abspath(f"./Migrations")
    restorePath()

    from os import listdir
    from os.path import isfile, join
    migration_files = [f for f in listdir(directory) if isfile(join(directory, f))]

    for migration_file in migration_files:
        Migration(database, os.path.basename(migration_file.replace('.py', '')))
