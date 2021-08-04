import logging
import logging.handlers
from datetime import datetime
import os

from Utilities.root_dir import ROOT_DIR

now = datetime.now()
now_str = now.strftime("%d_%m_%Y_%H_%M_%S")

formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")

file_handler = logging.handlers.WatchedFileHandler(
    os.environ.get("LOGFILE", f"{ROOT_DIR}/Logs/aurora_server_{now_str}.log"))
file_handler.setFormatter(formatter)

console_handler = logging.StreamHandler()
console_handler.setFormatter(formatter)

logger = logging.getLogger("Aurora")
logger.setLevel(os.environ.get("LOGLEVEL", "INFO"))

for handler in logger.handlers:
    logger.removeHandler(handler)

logger.addHandler(file_handler)
logger.addHandler(console_handler)