def run(database):
    table = 'settings'

    database.createTable(table, {
        'id': database.id(table),
        'label': database.text(),
        'value': database.text()
    })

    database.makeUnique(table, 'id')
