# For more info about db functions, see database.py
def up(database):
    database.createTable('settings', {
        'id': database.id(),
        'label': database.text(),
        'value': database.text(),
    })


def down(database):
    database.dropTable('settings')
