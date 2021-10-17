import logging
import logging.handlers
from datetime import datetime
import os

from Helpers.root_dir import ROOT_DIR
from pathlib import Path

existing_loggers = {};

def make_logger(name):

    if name in existing_loggers:
        return existing_loggers[name]

    now = datetime.now()
    now_str = now.strftime("%d_%m_%Y_%H_%M_%S")

    formatter = logging.Formatter("%(asctime)s | %(name)s | %(levelname)s - %(message)s")

    Path(f"{ROOT_DIR}/../logs").mkdir(parents=True, exist_ok=True)

    file_handler = logging.handlers.WatchedFileHandler(
        os.environ.get("LOGFILE", f"{ROOT_DIR}/../logs/aurora_launcher_client_{now_str}.log"))
    file_handler.setFormatter(formatter)

    console_handler = logging.StreamHandler()
    console_handler.setFormatter(formatter)

    logger = logging.getLogger(name)
    logger.setLevel(os.environ.get("LOGLEVEL", "INFO"))

    for handler in logger.handlers:
        logger.removeHandler(handler)

    logger.addHandler(file_handler)
    logger.addHandler(console_handler)

    existing_loggers[name] = logger

    return logger