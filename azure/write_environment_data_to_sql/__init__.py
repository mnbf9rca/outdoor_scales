import logging
from collections.abc import Iterable
from json import dumps, loads
from os import environ
from typing import List, Any

import azure.functions as func
import pyodbc
from dateutil import parser

logger = logging.getLogger(f"azure.func.{__name__}")

def main(event: func.EventHubEvent):

    # for some reason, when executing in azure, we dont always get an iterable, even with cardinality=many
    # so test for it
    event_data = _get_event_data(event)
    logger.info(f'Received trigger for {len(event_data)} items')
    logger.debug(f'payload: {dumps(event_data)}')

    return _write_to_database(event_data)

def _get_event_data(event: func.EventHubEvent) -> List[Any]:
    if isinstance(event, Iterable):
        logger.debug('iterable')
        return [loads(s) for s in [e.get_body().decode('utf-8') for e in event]]
    else:
        logger.debug('not iterable')
        return loads(event.get_body().decode('utf-8'))

def _write_to_database(event_data: dict) -> int:
    try:
        cnxn = pyodbc.connect(environ.get('cloud_sql_conn_string'))
    except Exception as e:
            logger.error(e)
            return -1
    try:
        with cnxn:
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

            '''
            cursor.executemany("""INSERT INTO cloud_scales_source_data
                            (       [source],   [type],   [name],    [event],    [data],    [device_id],                  [published_at])
                            VALUES (?,?,?,?,?,?,?)    
                            """,
                            [(s["source"], s["type"], s["name"], s["event"], s["data"], s["device_id"], parser.parse(s["published_at"]))
                                for s in event_data])
            '''
            cursor.executemany("{CALL insert_cloud_measurement (?,?,?,?,?,?,?)}", 
                                    [(s["source"], s["type"], s["name"], s["event"], float(s["data"]), s["device_id"], parser.parse(s["published_at"]))
                                    for s in event_data])
    except Exception as e:
        logger.error(e)
        return -1
    # shouldnt need to close, but seems to improve throughput
    cnxn.close()
    logger.info(f"Successfully stored {len(event_data)} in DB")

    return None
