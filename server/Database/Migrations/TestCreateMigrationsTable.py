def run(database):
    database.createTable('migrations', {
        'id': 'id',
        'timestamp': 'datetime',
        'name': 'text',
        'status': 'text'
    })