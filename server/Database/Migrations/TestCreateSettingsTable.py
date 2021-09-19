def run(database):
    database.createTable('settings', {
        'id': 'id',
        'label': 'text',
        'value': 'text'
    })