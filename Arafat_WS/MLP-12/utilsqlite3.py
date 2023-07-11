# Utilities for handling sqlite3 databases


import sqlite3
import os


# Create the sqlite3 database file, overwrite if exist
# Check if the database file already exists
def createDB(db_path, overwrite=False):
    # Check overwrite conditions
    file_exist = os.path.exists(db_path)
    if file_exist and overwrite==False:
        print(f'ERROR: {db_path} exist, try overwrite=True')
        return False
    # remove old file if exist
    if file_exist:
        print('WARN: Overwriting existing file', db_path)
        os.remove(db_path)
    # Connect to the SQLite database file with overwrite option
    conn = sqlite3.connect(db_path, isolation_level=None)
    conn.close()
    return True



# Returns a list of table names in the database file
def getTableNames(db_path):
    # Connect to the SQLite database
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()

    # Fetch the table names
    cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
    table_names = cursor.fetchall()
    table_names = [name[0] for name in table_names]  # make a list to return
    conn.close()
    return table_names


# Returns a list of column names of the specified table
def getColNames(database_filename, table_name):
    # Connect to the SQLite database
    conn = sqlite3.connect(database_filename)
    cursor = conn.cursor()

    # Fetch the column names
    cursor.execute(f"PRAGMA table_info({table_name})")
    results = cursor.fetchall()
    column_names = [result[1] for result in results]  # Extract the column names from the query results

    # Close the connection and return
    conn.close()
    return column_names


# returns all records of a given table
def getRecords(db_path, table_name):
    # Connect to the SQLite database
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    # Fetch all records from the table
    cursor.execute(f"SELECT * FROM {table_name}")
    records = cursor.fetchall()
    return records


# Deletes a table from the database
def dropTable(db_path, table_name):
    # Connect to the SQLite database
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    # Drop the table if it exists
    cursor.execute(f"DROP TABLE IF EXISTS {table_name}")
    # Commit the changes and close the connection
    conn.commit()
    conn.close()


# Deletes the rows of the given table
def deleteRows(db_path, table_name):
    # Connect to the SQLite database
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    # Delete all rows from the table
    print(f'WARN: Deleting all rows in {table_name}')
    cursor.execute(f"DELETE FROM {table_name}")
    conn.commit()
    conn.close()


# Checks if a table exist
def existTable(db_path, table_name):
    # Connect to the SQLite database
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    # Check if the table exists
    cursor.execute(f"SELECT name FROM sqlite_master WHERE type='table' AND name='{table_name}'")
    result = cursor.fetchone()
    if result is None: exist = False
    else: exist = True
    # Commit the changes and close the connection and return result
    conn.commit()
    conn.close()
    return exist



# Inserts a list of records into the Header table
# recordFunc: A function that takes a cursor and a record and inserts into the table (insertHeaderRecord)
# record_list: list of tuples (key, target, description)
def insertRecordList(db_path, recordFunc, record_list):
    # Connect to the SQLite database
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    # Insert the records
    for record in record_list:
        recordFunc(cursor, record)
    # Commit the changes and close the connection
    conn.commit()
    conn.close()
