import logging
import pyodbc
from os import environ

import azure.functions as func


def main(event: func.EventHubEvent):
    logging.info('Python EventHub trigger processed an event: %s',
                 event.get_body().decode('utf-8'))

    ''' expand input messages
    [source] varchar(64),
    [type] varchar(64),
    [name] varchar(64),
    [event] varchar(64),
    [data] decimal(18,0),
    [device_id] varchar(24),
    [published_at] datetime,
    '''

    cnxn = pyodbc.connect(environ.get('cloud_sql_conn_string'))
    cursor = cnxn.cursor()
    cursor.fast_executemany = True
    
    