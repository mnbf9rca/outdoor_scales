import logging
from collections.abc import Iterable
from json import dumps, loads
from os import environ

import azure.functions as func
import pyodbc
from dateutil import parser


def main(event: func.EventHubEvent):
    if isinstance(event, Iterable):
        event_data = [loads(s) for s in [e.get_body().decode('utf-8') for e in event]]
    else:
        event_data = [loads(event.get_body().decode('utf-8'))]
    logging.info('Python EventHub trigger received an event set of %s items: %s',
                 len(event_data),
                 event_data)

    cnxn = pyodbc.connect(environ.get('cloud_sql_conn_string'))
    cursor = cnxn.cursor()
    cursor.fast_executemany = True
    ''' 
    table: cloud_scales_source_data
    [source] varchar(64),
    [type] varchar(64),
    [name] varchar(64),
    [event] varchar(64),
    [data] decimal(18,0),
    [device_id] varchar(24),
    [published_at] datetime,
    '''

    cursor.executemany("""
    INSERT INTO cloud_scales_source_data
    ( [source], 
      [type], 
      [name], 
      [event], 
      [data], 
      [device_id], 
      [published_at])
    VALUES (?,?,?,?,?,?,?)    
    """, [(s["source"],
           s["type"],
           s["name"],
           s["event"],
           s["data"],
           s["device_id"],
           parser.parse(s["published_at"]))
          for s in event_data])
    cursor.commit()

    return
