import datetime
import json
import boto3
import logging

logger = logging.getLogger()
logger.setLevel(logging.INFO)

stage = 'prod'
resource = 'autonomous-dog-feeder'

DATE_FORMAT = '%d-%m-%Y'

dynamodb = boto3.resource('dynamodb')
food_table = dynamodb.Table('Food')
water_table = dynamodb.Table('Water')

html_home_page = """<!DOCTYPE html>
<html>
<head>
    <title>Autonomous Dog Feeder</title>
</head>
<body>
    <h1>Autonomous Dog Feeder</h1>
    <!-- 
    Add a table with two columns
    First row is the header: Food, Water
    Second row contains graphs
    Third row contains the amount of food and water consumed 
    -->
    <h3>Date: {start_date} - {end_date}</h2><br>
    <table>
        <tr>
            <th>Food</th>
            <th>Water</th>
        </tr>
        <tr>
            <td>Graph</td>
            <td>Graph</td>
        </tr>
        <tr>
            <td>Amount</td>
            <td>Amount</td>
        </tr>
    </table><br><br>
    <h4>Choose a date or a date range to display the amount of food and water consumed</h5><br>
    <!-- Add a form to select the date or the date range: start date, end date -->
    <form action="/{path}">
        <label for="date">Date (DD-MM-YYYY):</label>
        <input type="text" id="date" name="date">
        <h4>Or</h5>
        <label for="start_date">Start date (DD-MM-YYYY):</label>
        <input type="text" id="start_date" name="start_date"><br><br>
        <label for="end_date">End date (DD-MM-YYYY):</label>
        <input type="text" id="end_date" name="end_date"><br><br>
        <input type="submit" value="Submit">
    </form>

</body>
</html>
"""

# Query the food table for the last 30 days
def get_data_interval(table_str: str, start: datetime, end: datetime):
    # Compute the start and end timestamps
    start_timestamp = int(start.timestamp())
    end_timestamp = int(end.timestamp())

    # Check which table to query
    if(table_str == 'food'):
        table = food_table
    else:
        table = water_table    
            
    return table.query(
        KeyConditionExpression='Timestamp > :start and Timestamp < :end',
        ExpressionAttributeValues={
            ':start': start_timestamp,
            ':end': end_timestamp
        }
    )
    
def sanity_check(event):
    logger.info("Event: {}\n".format(event))

    # Get the event parameters
    query_string_parameters = event.get('queryStringParameters')
    # Log the query string parameters
    logger.info("Query string parameters: {}\n".format(query_string_parameters))

    if query_string_parameters is None:
        query_string_parameters = {}

    date = query_string_parameters.get('date', '')
    start_date = query_string_parameters.get('start_date', '')
    end_date = query_string_parameters.get('end_date', '')

    # Print to log
    logger.info("Date: {}\n".format(date))
    logger.info("Start date: {}\n".format(start_date))
    logger.info("End date: {}\n".format(end_date))
    
    if(date != ''):
        # Single date not specified
        try:
            date_datetime = datetime.datetime.strptime(date, DATE_FORMAT)
            start = date_datetime.strftime(DATE_FORMAT)
            # Set the end date to be the same as the start date plus one day
            end = (date_datetime + datetime.timedelta(days=1)).strftime(DATE_FORMAT)
        except ValueError:
            return {"check": False, "message": "Invalid date format. Must be DD-MM-YYYY"}
        else:
            return {"check": True, "params": {"start": start, "end": end}}
    
    if(start_date == ''):
        # No date and start date specified, then compute today's date in the format DD-MM-YYYY
        now = datetime.datetime.now()
        start = now.strftime(DATE_FORMAT)
        end = (now + datetime.timedelta(days=1)).strftime(DATE_FORMAT)
        return {"check": True, "params": {"start": start, "end": end}}
    else:
        try:
            start_datetime = datetime.datetime.strptime(start_date, DATE_FORMAT)
            start = start_datetime.strftime(DATE_FORMAT)
        except ValueError:
            return {"check": False, "message": "Invalid start date format. Must be DD-MM-YYYY"}

    # Check if end date is not empty then check if it follows the format DD-MM-YYYY
    if(end_date != ''):
        try:
            end_datetime = datetime.datetime.strptime(end_date, DATE_FORMAT) + datetime.timedelta(days=1)
            end = end_datetime.strftime(DATE_FORMAT)
        except ValueError:
            return {"check": False, "message": "Invalid end date format. Must be DD-MM-YYYY"}

    # Check if the end date is not empty and if it is before the start date
    if(end_datetime <= start_datetime):
        return {"check": False, "message": "End date must be after start date"}
    
    # Check if the start date is not in the future
    if(start_datetime > datetime.datetime.now()):
        return {"check": False, "message": "Start date must be before today"}
    
    # Check if the end date is not in the future
    if(end_datetime > datetime.datetime.now() + datetime.timedelta(days=1)):
        return {"check": False, "message": "End date must be before today"}
    
    return {"check": True, "params": {"start": start, "end": end}}


def lambda_handler(event, context):

    # Sanity check
    params = sanity_check(event)
    if(params['check'] == False):
        return {
            'statusCode': 400,
            'body': json.dumps(params['message'])
        }

    return {
        'statusCode': 200,
        "headers": {"Content-Type": "text/html"},
        'body': html_home_page.format(
            start_date=params['params']['start'], 
            end_date=params['params']['end'],
            path='{}/{}'.format(stage, resource)
        )
    }