import sys

from Scripts.make_migration import make_migration
from Scripts.make_seeder import make_seeder
from Scripts.run_migrations import run_migrations
from Scripts.run_seeder import run_seeder

HELP_MSG = "For help do: 'python3 run.py help'"

commands = {
    'migrate:make': {
        "function": make_migration,
        "arguments": "[NAME]",
        "description": "Creates a new migration."
    },
    'migrate:run': {
        "function": run_migrations,
        "description": "Runs all of the necessary migrations."
    },
    'seed:make': {
        "function": make_seeder,
        "arguments": "[NAME]",
        "description": "Creates a new seeder."
    },
    'seed:run': {
        "function": run_seeder,
        "arguments": "[NAME]",
        "description": "Runs the specified seeder."
    },
}


def _help(args):
    print('Aurora Server')
    print("\n\tUsage:\n\t> python3 run.py GROUP:COMMAND <command_args...>")
    print("\n\tExample:\n\t> python3 run.py migrate:make CreateSettingsTable")
    print('\n\tAvailable commands:')
    for _command in commands.keys():
        print(f"\t- {_command} {commands[_command]['arguments'] + ' ' if 'arguments' in commands[_command].keys() else ''}: {commands[_command]['description']}")
    print("")

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
