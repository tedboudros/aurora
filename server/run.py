import sys

from Scripts.make_migration import make_migration
from Scripts.run_migrations import run_migrations

HELP_MSG = "For help do: 'python3 run.py help'"

commands = {
    'migrate:make': {
        "function": make_migration,
        "description": "Creates a new migration with the given name."
    },
    'migrate:run': {
        "function": run_migrations,
        "description": "Runs all of the migrations (only new ones)."
    },
}


def _help(args):
    print('Aurora Server Helper! \n')
    for _command in commands.keys():
        print(f"\t'{_command}' : {commands[_command]['description']}\n")


all_commands = {
    **commands,
    'help': {
        "function": _help
    }
}

if __name__ == '__main__':
    try:
        length_of_args_to_pass = len(sys.argv) - 2

        if length_of_args_to_pass < 0:
            raise IndexError(HELP_MSG)

        command = sys.argv[1]

        if command not in all_commands.keys():
            raise IndexError(f"Command not found... {HELP_MSG}")

        args_to_pass = sys.argv[2:len(sys.argv)]
        all_commands[command]['function'](args_to_pass)
    except IndexError as err:
        print(err)
