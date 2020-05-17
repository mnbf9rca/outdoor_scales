import logging
import pyodbc
from os import environ

import azure.functions as func


def main(event: func.EventHubEvent):
    logging.info('Python EventHub trigger processed an event: %s',
                 event.get_body().decode('utf-8'))
    cnxn = pyodbc.connect(environ.get('cloud_sql_conn_string'))
    cursor = cnxn.cursor()