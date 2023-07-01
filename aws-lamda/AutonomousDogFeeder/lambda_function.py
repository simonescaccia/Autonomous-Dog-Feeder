import datetime
import json
import boto3
import logging

logger = logging.getLogger()
logger.setLevel(logging.INFO)

stage = 'prod'
resource = 'autonomous-dog-feeder'

DATE_FORMAT = '%d-%m-%Y'
DATE_TIME_FORMAT = '%d-%m-%Y %H:%M:%S'

dynamodb = boto3.resource('dynamodb')
food_table = dynamodb.Table('Food')
water_table = dynamodb.Table('Water')
settings_table = dynamodb.Table('Settings')

DEVICE_ID = '1'

html_home_page = """<!DOCTYPE html>
<html>
<head>
    <title>Autonomous Dog Feeder</title>
    <script src="https://www.gstatic.com/charts/loader.js"></script>
    <style>
        th   {{width: 50%}}
        td   {{width: 50%}}
    </style>
</head>
<body>
    <h1>Autonomous Dog Feeder</h1>
    <a href="autonomous-dog-feeder?date=28-06-2023"><h3>Demo data</h3></a>
    <h3>Date: {start_date} - {end_date}, Number of days: {number_of_days}</h3><br>
    <table style="width:100%">
        <tr>
            <th>Food</th>
            <th>Water</th>
        </tr>
        <tr>
            <td>
                <div id="foodChart" style="width:100%; height:500px;"></div>
                <script>
                google.charts.load('current',{{packages:['corechart']}});
                google.charts.setOnLoadCallback(drawChart);

                function drawChart() {{

                    // Set Data
                    const data = new google.visualization.DataTable();
                    
                    data.addColumn('datetime', 'Time');
                    data.addColumn('number', 'Food grams');

                    data.addRows({food_data});

                    // Set Options
                    const options = {{
                        title: 'Food grams vs. Time',
                        vAxis: {{title: 'Food grams'}},
                        hAxis: {{
                            title: 'Time',
                            viewWindow: {{
                                min: {min_date},
                                max: {max_date}
                            }},
                            gridlines: {{
                                count: -1,
                                units: {{
                                    days: {{format: ['dd MMM']}}
                                }}
                            }},
                            minorGridlines: {{
                                units: {{
                                    hours: {{format: ['HH:mm:ss', 'HH']}}
                                }}
                            }}    
                        }},
                        legend: 'none'
                    }};

                    // Draw
                    const chart = new google.visualization.ScatterChart(document.getElementById('foodChart'));
                    chart.draw(data, options);

                }}
                </script>
            </td>
            <td>
                <div id="waterChart" style="width:100%; height:500px;"></div>
                <script>
                google.charts.load('current',{{packages:['corechart']}});
                google.charts.setOnLoadCallback(drawChart);

                function drawChart() {{

                    // Set Data
                    const data = new google.visualization.DataTable();
                    
                    data.addColumn('datetime', 'Time');
                    data.addColumn('number', 'Water milliliters');

                    data.addRows({water_data});

                    // Set Options
                    const options = {{
                        title: 'Water milliliters vs. Time',
                        vAxis: {{title: 'Water milliliters'}},
                        hAxis: {{
                            title: 'Time',
                            viewWindow: {{
                                min: {min_date},
                                max: {max_date}
                            }},
                            gridlines: {{
                                count: -1,
                                units: {{
                                    days: {{format: ['dd MMM']}}
                                }}
                            }},
                            minorGridlines: {{
                                units: {{
                                    hours: {{format: ['HH:mm:ss', 'HH']}}
                                }}
                            }}
                        }},
                        legend: 'none'
                    }};

                    // Draw
                    const chart = new google.visualization.ScatterChart(document.getElementById('waterChart'));
                    chart.draw(data, options);

                }}
                </script>
            </td>
        </tr>
        <tr>
            <td>
                Food per day: {food_per_day} g<br>
                Number of meals per day: {number_of_meals}<br>
                Food consumed: {food_consumed} g
            </td>
            <td>
                Water bowl capacity: {water_bowl_capacity} ml<br>
                Water consumed: {water_consumed} ml
            </td>
        </tr>
    </table><br><br>
    <h4>Choose a date or a date range to display the amount of food and water consumed</h5><br>
    <!-- Add a form to select the date or the date range: start date, end date -->
    <form action="/{path}">
        <label for="date">Date (dd-mm-yyyy):</label>
        <input type="text" id="date" name="date">
        <h4>Or</h5>
        <label for="start_date">Start date (dd-mm-yyyy):</label>
        <input type="text" id="start_date" name="start_date"><br><br>
        <label for="end_date">End date (dd-mm-yyyy):</label>
        <input type="text" id="end_date" name="end_date"><br><br>
        <input type="submit" value="Submit">
    </form>

</body>
</html>
"""

# Query the food table for the last 30 days
def get_data_interval(table_str: str, start_timestamp: int, end_timestamp: int):

    # Check which table to query
    if(table_str == 'food'):
        table = food_table
    else:
        table = water_table    
            
    return table.query(
        KeyConditionExpression='DeviceId = :id AND #time BETWEEN :start AND :end',
        ExpressionAttributeNames={
            '#time': 'Time'
        },
        ExpressionAttributeValues={
            ':id': DEVICE_ID,
            ':start': start_timestamp,
            ':end': end_timestamp
        }
    )

def get_settings():

    return settings_table.query(
        KeyConditionExpression='DeviceId = :id',
        ExpressionAttributeValues={
            ':id': DEVICE_ID
        }
    )

def format_dict(check: bool, message="", start_str="", end_str="", start_timestamp=0, end_timestamp=0):
    if check:
        return {
            "check": check,
            "params": {
                "start": start_str,
                "end": end_str,
                "start_timestamp": start_timestamp,
                "end_timestamp": end_timestamp
            }
        }
    else:
        return {
            "check": check,
            "message": message
        }

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
            start_datetime = datetime.datetime.strptime(date, DATE_FORMAT)
            start = start_datetime.strftime(DATE_TIME_FORMAT)
            # Set the end date to be the same as the start date plus one day
            end_datetime = start_datetime + datetime.timedelta(days=1)
            end = end_datetime.strftime(DATE_TIME_FORMAT)
        except ValueError:
            return format_dict(False, "Invalid date format. Must be dd-mm-yyyy")
        else:
            return format_dict(
                True, 
                start_str=start, 
                end_str=end,
                # Convert datetime to timestamp in milliseconds
                start_timestamp=int(start_datetime.timestamp()),
                end_timestamp=int(end_datetime.timestamp())
            )
    
    if(start_date == ''):
        # No date and start date specified, then compute today's date in the format dd-mm-yyyy setting the hour to 00:00:00
        start_datetime = datetime.datetime.now().replace(hour=0, minute=0, second=0, microsecond=0)
        start = start_datetime.strftime(DATE_TIME_FORMAT)
        end_datetime = start_datetime + datetime.timedelta(days=1)
        end = end_datetime.strftime(DATE_TIME_FORMAT)
        return format_dict(
            True, 
            start_str=start, 
            end_str=end, 
            start_timestamp=int(start_datetime.timestamp()), 
            end_timestamp=int(end_datetime.timestamp())
        )
    else:
        try:
            start_datetime = datetime.datetime.strptime(start_date, DATE_FORMAT)
            start = start_datetime.strftime(DATE_TIME_FORMAT)
        except ValueError:
            return format_dict(False, "Invalid start date format. Must be dd-mm-yyyy")

    # Check if end date is not empty then check if it follows the format dd-mm-yyyy
    if(end_date != ''):
        try:
            end_datetime = datetime.datetime.strptime(end_date, DATE_FORMAT) + datetime.timedelta(days=1)
            end = end_datetime.strftime(DATE_TIME_FORMAT)
        except ValueError:
            return format_dict(False, "Invalid end date format. Must be dd-mm-yyyy")

    # Check if the end date is not empty and if it is before the start date
    if(end_datetime <= start_datetime):
        return format_dict(False, "End date must be after start date")
    
    # Check if the start date is not in the future
    if(start_datetime > datetime.datetime.now()):
        return format_dict(False, "Start date must be before today")
    
    # Check if the end date is not in the future
    if(end_datetime > datetime.datetime.now() + datetime.timedelta(days=1)):
        return format_dict(False, "End date must be before today")
    
    return format_dict(
        True, 
        start_str=start, 
        end_str=end, 
        start_timestamp=int(start_datetime.timestamp()), 
        end_timestamp=int(end_datetime.timestamp())
    )

def format_data_to_plot(data):
    # Build a list of lists containing the time and the value
    data_formatted = "["

    items = data['Items']
    # For each item, get the Time and the Value
    for item in items:
        # From the timestamp, get the datetime object
        datetime_obj = datetime.datetime.fromtimestamp(int(item['Time']))
        # Get the year, month, day, hour, minute and second
        year = datetime_obj.year
        month = datetime_obj.month
        day = datetime_obj.day
        hour = datetime_obj.hour
        minute = datetime_obj.minute
        second = datetime_obj.second
        # Append the time and the value to the list string
        data_formatted += "[new Date({},{},{},{},{},{}), {}],".format(year, month, day, hour, minute, second, item['Value']['Value'])

    # Remove the last comma
    data_formatted = data_formatted[:-1]
    # Close the list
    data_formatted += "]"

    return data_formatted

def compute_consumption(data):
    items = data['Items']
    # Compute the total water consumed
    total_consumption = 0
    # For each item, sum the Value
    for item in items:
        total_consumption += int(item['Value']['Value'])
    return total_consumption

def lambda_handler(event, context):

    # Sanity check
    params = sanity_check(event)
    if(params['check'] == False):
        return {
            'statusCode': 400,
            'body': json.dumps(params['message'])
        }
    

    # Get settings
    settings = get_settings()

    # Log the settings
    logger.info("Settings: {}\n".format(settings))
    

    # Get timestamp from params
    start_timestamp = params['params']['start_timestamp']
    end_timestamp = params['params']['end_timestamp']

    # Compute the difference in days between the start and the end timestamp
    days = int((end_timestamp - start_timestamp) / (24 * 60 * 60))

    # Print timestamps to log
    logger.info("Start timestamp: {}\n".format(start_timestamp))
    logger.info("End timestamp: {}\n".format(end_timestamp))
    logger.info("Days: {}\n".format(days))
    

    # Get the data from the food table and the water table
    food_data = get_data_interval('food', start_timestamp, end_timestamp)
    water_data = get_data_interval('water', start_timestamp, end_timestamp)

    # Log the data
    logger.info("Food data: {}\n".format(food_data))
    logger.info("Water data: {}\n".format(water_data))


    # Compute the amount of water consumed
    food_consumed = compute_consumption(food_data)
    water_consumed = compute_consumption(water_data)


    # Create the data to be plotted
    food_data_formatted = format_data_to_plot(food_data)
    water_data_formatted = format_data_to_plot(water_data)

    # Log the formatted data
    logger.info("Food data formatted: {}\n".format(food_data_formatted))
    logger.info("Water data formatted: {}\n".format(water_data_formatted))


    # Get the year, month, day, hour, minute and second from the start timestamp and the end timestamp
    start_datetime = datetime.datetime.fromtimestamp(start_timestamp)
    end_datetime = datetime.datetime.fromtimestamp(end_timestamp)

    html_home_page_formatted = html_home_page.format(
            start_date =            params['params']['start'], 
            end_date =              params['params']['end'],
            number_of_days =        days,
            food_data =             food_data_formatted,
            water_data =            water_data_formatted,
            min_date =              'new Date({},{},{},{},{},{})'.format(start_datetime.year, start_datetime.month, start_datetime.day, start_datetime.hour, start_datetime.minute, start_datetime.second),
            max_date =              'new Date({},{},{},{},{},{})'.format(end_datetime.year, end_datetime.month, end_datetime.day, end_datetime.hour, end_datetime.minute, end_datetime.second),
            food_per_day =          settings['Items'][0]['DailyFoodGrams'],
            number_of_meals =       settings['Items'][0]['NumberOfMealsPerDay'],
            water_bowl_capacity =   settings['Items'][0]['WaterBowlCapacityMilliliters'],
            water_consumed =        water_consumed,
            food_consumed =         food_consumed,
            path='{}/{}'.format(stage, resource)
        )
    
    # Log the html page
    logger.info("HTML page: {}\n".format(html_home_page_formatted))

    return {
        'statusCode': 200,
        'headers': {"Content-Type": "text/html"},
        'body': html_home_page_formatted
    }