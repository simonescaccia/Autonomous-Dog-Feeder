# Autonomous-Dog-Feeder

![phisical_circuit](./images/phisical_circuit.jpg)

## 1. What is the problem and why do you need IoT?

### Problems we want to solve

#### Main problem

- During the summer, some families leave their dogs because they do not have the opportunity or the desire to move with them. (formal data not found but a lot of news reports every year in the summer are released, in particular, they talk about 500 pets abandoned every day in Italy).
Duration of the holidays:  
La maggior parte sceglie di andare via tra giugno, luglio e agosto per periodi anche abbastanza lunghi: il 59% ha infatti indicato dalle 4 alle 7 notti via. Da considerare che la media 2021-2022 delle vacanze degli italiani nel periodo estivo è stata di 4 notti ([source](https://www.nostrofiglio.it/bimbi-in-viaggio/turismo-family-in-italia#:~:text=La%20maggior%20parte%20sceglie%20di,%C3%A8%20stata%20di%204%20notti.)). Then, to help dogs and families with this problem, we want to provide an autonomous dog feeder to feed the pet for at least 7 days. This project aims to build a reliable alternative to the neglect of dogs during the summer.

#### Related problems

- Dogs have a daily amount of food they need to eat, according to their age, activity level, and health condition. This amount of food is suggested by the vet or written on the dog food package. We want to make sure that the dog gets the right amount of food every day.
- Dogs need to eat their food several meals throughout the day. Each meal should be served at a specific time, according to the dog's age and activity level or preference. We want to make sure that the dog gets the right amount of food at the right time.
- Water is essential for dogs. We want to make sure that the dog has access to water at all times.
- We want to monitor the trends of the dog's eating habits and water consumption, to support the medical diagnosis.

### How we can monitor the environment?

- Food:
  - We want to measure the amount of dry food to pour in the bowl, to be able to define the amount of food for each meal. We can use a load cell sensor to measure the weight of the food.
- Water:
  - We want to measure the amount of water left in the bowl.

### Sensors

- **Load cell** sensor to measure the weight of the food in the bowl.
Periodic sensing is required to measure over time the quantity of food in the bowl.
By doing this, we can learn the dog's eating habits, like building a confidence percentage of the amount of food the dog eats at each time, to increase the power efficiency of the system.\
![load_cell](./images/load_cell.png)
- **Load cell** sensor to measure the weight of the water in the bowl to quantify the liters of water in the bowl (the density of the water is about 1 Kg/L, considering the temperature of the water at room temperature and the pressure at sea level).
Periodic sensing is required to measure over time the quantity of water in the bowl.
By doing this, we can learn the dog's drinking habits, like building a confidence percentage of the amount of water the dog drinks at each time, to increase the power efficiency of the system.\
![load_cell](./images/load_cell.png)
- **Slide Switch** to start or stop the autonomous dog feeder.\
![slide_switch](./images/slide_switch.png)

### Actuators

- A **servo motor** to open and close the recipient of the food.
The activation of the servo motor will be triggered by the cloud-based service only the bowl can contain the required amount of food.\
![servo_motor](./images/servo.png)
- A **servo motor** to shake the recipient of the food. The activation of the servo motor will be triggered by the cloud-based service only the bowl can contain the required amount of food.\
![servo_motor](./images/servo.png)
- A **water pump** fills the bowl with the water.
The activation of the water pump will be triggered by the load cell sensor only when the bowl doesn't contain the required amount of water.\
![water_pump](./images/water_pump.png)

![circuit](./images/Circuit_bb.png)

## 2. What data are collected and by which sensors?

We want to collect the food and water consumption of the dog, during the day. By observing the sample data on load cells, we want to compute locally the amount of food and water consumed by the dog. Then, we want to send the computed data to the cloud-based service to store them and make them available to the dog owner.

### Sensors accuracy, the unit of measurement and periodicity

- The **load cell** sensor to measure the weight of the food in the bowl has a capacity of 5 Kg since the 1 Kg load cell is not suitable for large breeds of dogs.
Large breeds of dogs eat about 600/700 g of food per day divided into two or three meals(the amount of food is written on the dog food package). But in special cases, like pregnancy or lactation, the food quantity can be three times the normal amount. So, if a pregnant dog eats 2.1 Kg of food per day divided into two meals, a 1 Kg load cell can not be suitable for this case. More, the tare i.e. the weight of the bowl should be considered too.  
Also, we can support the fact that the resolution of the 5 Kg load cell is good enough for our purpose. The ESP32 is connected to the load cell via the HX711 amplifier, which has a resolution of 24 bits. This means that the load cell can measure the weight of the food in the bowl with a resolution of 5/2^24 = 0.0000003 Kg = 0.0003 g.
The chosen unit of measurement is grams (g) and is set using a calibration factor.
To choose the periodicity of the sensing we need to consider the fact that we are interested in monitoring the dog's eating habits, like at which time the dog starts eating considering that the food is scheduled to be served at a specific time, and the speed of eating the food in the bowl. So, to reconstruct the dog's eating velocity, we can consider that a large breeds dog finishes its meal in a maximum of 5 minutes.
Considering the sampling theorem, we need to sample every at least 2.5 minutes, but some dogs eat faster than others, in particular puppies can finish their meal in about 1 minute. So, we can choose a sampling period of 30 seconds.
This approach is a good start because we don't know a priori the eating habits of a specific dog, so we want to oversample at the beginning and then we can adjust the sampling period according to the dog's eating habits.
Finally, when the dog ends its meal we can stop the sensing until a new scheduled meal is served.
- The **load cell** sensor to measure the weight of the water in the bowl has a capacity of 5 Kg since the weight of the bowl can saturate the 1 Kg load cell.
The chosen unit of measurement is liters (L) and is set using a calibration factor since we can assume 1 Kg = 1 L.
To choose the periodicity of the sensing we need to consider the fact that we are interested in having water in the bowl at all times and monitoring the dog's water consumption habits, like how much water the dog drinks during the day.
So, to have water in the bowl at all times, we can consider that the dog drinks continuously for a maximum of 30 seconds and after that, the bowl can be empty, so we need to refill it.
Considering that we don't want that the water bowl refilled while the dog is drinking, we can choose a sampling period of 30 seconds.
  
### Collected data velocity, variability and variety

- Food data: the velocity of the data is one sample every 30 seconds, but only if the bowl is not empty and the dog has eaten something, otherwise the data is not collected (variability). The variety of data goes from the weight of a single meal, specified by the dog owner, to 0 g excluded. Peaks in the weight of the food can be caused by the dog pressing the bowl while eating.
- Water data: the velocity of the data is one sample every 30 seconds, but only if the dog has drunk something, otherwise the data is not collected (variability). The variety of data goes from the capacity of the bowl, defined by the vendor of the autonomous dog feeder, to 0 L excluded. Peaks in the weight of the water can be caused by the dog pressing the bowl while drinking.

### Data analysis

- Local data analysis: every sample is analyzed locally to check if something strange is happening, like:
  - If the weight of the food or the liters of water is negative, then we can assume there is an accuracy error in the sensor, so we can set the value to 0.
  - If the weight of the food is greater than the poured one, then we can assume that the dog is pressing the bowl while eating, so we can discard the sample.
  - If the water is greater than the poured one, then we can assume that the dog is pressing the bowl while drinking, so we can discard the sample.
  - If the servo motor is activated but the weight of the food is not increasing after a time defined by the distance between the recipient of the food and the bowl, then we can assume that the recipient of the food is empty, so we need to notify the dog owner.
  - If the water pump is activated but the weight of the water is not increasing after a time defined by the distance between the recipient of the water and the bowl, then we can assume that the recipient of the water is empty, so we need to notify the dog owner.
  - If we want to pour x grams of food, then we need to continuously monitor the weight of the food in the bowl and stop the servo motor when the weight of the food reaches x grams.
  - If we want to pour y liters of water, then we need to continuously monitor the weight of the water in the bowl and stop the water pump when the weight of the water reaches y liters.

- Collective data analysis: we expect to obtain similar habits for dogs with the same characteristics, like breed, age, weight, etc. So, we can use the data collected from all the dogs to predict the eating habits of a new dog with similar characteristics.

### Data prediction model and energy consumption

Without the data prediction model, as described above, we will monitor the water and food bowl every 30 seconds, and the energy consumption can be approximated to the following (considering that the servos and the water pump are activated only when needed, generally less than 10 times a day):

- water load cell: 0.0015 A * 5 V = 0.0075 W
- food load cell: 0.0015 A * 5 V = 0.0075 W
- water pump: 0.05 A * 5 V = 0.25 W
- servo motor: 0.19 A * 5 V = 0.95 W
- ESP32 in deep sleep: 0.0008 A * 5 V = 0.004 W

Considering that sampling using the load cell require 300 milliseconds, and the rest of the 30 seconds the ESP32 is in deep sleep, we can compute the percentage of the duty cycle for the sampling. So, using the proportion 30000:100=300:x, we can compute the duty cycle x=1%.
Finally, we can compute the energy consumption for the sampling part in a day: 
$$E_{SamplingDay} = 2 * 0.0075 W * 0.01 * 24 h = 0.0036 Wh$$
$$E_{SleepDay} = 0.004 W * 0.99 * 24 h = 0.094 Wh$$

Also, we can assume that in a day we will serve the dog 2 meals and 10 water refills, in which the single refill will require 5 seconds to complete,
so we can compute the energy consumption for the serving part in a day:
$$E_{ServingServoDay} = 2 * 0.95 W * 2 * 0.000057 h = 0.0022 Wh$$
$$E_{ServingPumpDay} = 0.25 W * 10 * 0.000057 h = 0.00014 Wh$$

Also, we need to consider the energy consumption of the ESP32 while sending data through the WiFi connection in a day. We can assume about 20 sendings in a day, 10 for the water refill and 10 for the food monitoring, 5 per meal. So, we can compute the energy consumption for the WiFi connection in a day, considering 1 second for each sending:
$$E_{WiFiDay} = 0.2 W * 20 * 0.000016 h = 0.000064 Wh$$

Finally, we can compute the total energy consumption in a day:
$$E_{TotalDay} = E_{SamplingDay} + E_{SleepDay} + E_{ServingServoDay} + E_{ServingPumpDay} + E_{WiFiDay} = 0.100004 Wh$$

Then in a week:
$$E_{TotalWeek} = 7 * E_{TotalDay} = 0.700028 Wh$$

Then in a month:
$$E_{TotalMonth} = 30 * E_{TotalDay} = 3.00012 Wh$$

Then in a year:
$$E_{TotalYear} = 365 * E_{TotalDay} = 36.50014 Wh$$

So, to choose the battery, we need 5V and then:
$$mAh = \frac{Wh}{V} = \frac{36.50014 Wh}{5 V} = 7.300028 Ah$$
Which is a reasonable battery capacity for a device like the ESP32, then we meet the requirements of at least 1 week.

With the data prediction model, we can avoid the sampling part in time in which we can predict that the dog will not eat or drink, like during the night. So, a reasonable assumption is that we avoid sampling in the 33% of the day (8 hours of sleep), and we can compute the energy consumption for the sampling part in a day:
$$E_{SamplingDayPrediction} = 2 * 0.0075 W * 0.01 * 16 h = 0.0024 Wh$$
$$E_{SleepDayDutyPrediction} = 0.004 W * 0.99 * 16 h = 0.06336 Wh$$
$$E_{SleepDayFullPrediction} = 0.004 W * 8 h = 0.032 Wh$$

Then, we can compute the energy consumption in a day:
$$E_{TotalDayPrediction} = E_{SamplingDayPrediction} + E_{SleepDayDutyPrediction} + E_{SleepDayFullPrediction} + E_{ServingServoDay} + E_{ServingPumpDay} + E_{WiFiDay} = 0.09776 Wh$$

Then in a year:
$$E_{TotalYearPrediction} = 365 * E_{TotalDayPrediction} = 35.7004 Wh$$

So, using a data prediction model, theoretically, we can reduce the energy consumption of the device by 0.8 Wh in a year, which is a percentage of 2.2%.

### Data aggregation on the edge

Federeting learning: we can use the data collected from the single dog to build a partial model, and share the parameters of the model with the cloud-based service to build a global model, using all the partial models from all the dogs.
The global model should consider the characteristics of the dog, like breed, age, weight, etc. to predict the eating habits of a new dog with similar characteristics.
With this approach, we can avoid sending the data to the cloud-based service, but only the parameters of the partial model, to preserve the privacy of the dog and minimize the energy consumption of the device; since sending only the parameters of the partial model is less energy consuming than sending the data.

### Data aggregation on the cloud

Another approach is to send the data to the cloud-based service and build the global model there. In this case, we need to consider the privacy of the dog and the greater energy consumption of the device.

## 3. What are the connected components, the protocols to connect them and the overall IoT architecture?

### Network diagram

Data are collected from the load cell sensors via the HX711 amplifier, which is connected to the ESP32. The ESP32 makes some local data computation and sends the data to an MQTT bridge via the MQTT protocol. The MQTT bridge sends this data to an AWS endpoint. This AWS endpoint implements some rules to store the data in a DynamoDB table. Finally, the data are visualized on a web page using the AWS API Gateway which triggers a Lambda function to query the DynamoDB table and return the web page. [Web site](https://6qxfwqry1k.execute-api.us-east-1.amazonaws.com/prod/autonomous-dog-feeder)

![aws_architecture](./images/aws-architecture.drawio.png)

![web_site](./images/web_site.jpg)

### Software components

- Device level:
  - Food dispenser: at a given schedule, the ESP32 will shake the first servo to avoid the food getting stuck in the dispenser, then it will activate the second servo to pour the food into the bowl. The ESP32 will monitor the weight of the food in the bowl through the load cell and close the second servo when the weight of the food reaches the desired value (food per day/number of meals per day). Then the ESP32 will stop the first servo.
  - Food data computation: consider the first meal, the bowl will be filled with food, so when we sample a weight smaller than the weight of the previous sample, we can assume that the dog has eaten something, so we can compute the weight of the food eaten by subtracting the weight of the previous sample from the weight of the current sample. Then we will send this data to the cloud-based service. If the bowl is empty, then we can assume that the dog has finished its meal, so we can stop the sensing until a new scheduled meal is served.
  - Water dispenser: we want that the bowl is always filled with water at a certain level. So, at the first activation, the ESP32 will activate the water pump to fill the bowl with water and stop the water pump when the weight of the water reaches the desired value (water bowl capacity), using the load cell to monitor the weight of the water in the bowl. If the weight of the water bowl reaches a certain level (water bowl capacity/2), then the ESP32 will activate the water pump to fill the bowl with water and stop the water pump when the weight of the water reaches the desired value (water bowl capacity), using the load cell to monitor the weight of the water in the bowl.
  - Water data computation: when we sample a weight smaller than the weight of the previous sample, we can assume that the dog has drunk something, so we can compute the weight of the water drunk by subtracting the weight of the previous sample from the weight of the current sample. Then we will send this data to the cloud-based service.
- Cloud level:
  - Lambda data query: the Lambda function will query the DynamoDB table selecting the samples in a given interval of time.
  - Lambda data computation: the Lambda function will compute the sum of the food eaten and the sum of the water drunk in a given interval of time.
  - Lambda data visualization: formatting data to be visualized on a web page. Then the Lambda function will return a web page with the data computed.

### Software architecture

![software_architecture](./images/software_architecture.drawio.png)

## 4. How do you measure the performance of the system?

We can measure the performance of the system by comparing the data collected from the system with testing samples. These testing samples can be collected to test both the dispensed food and water but also the amount of food and water eaten and drunk by the dog.
Another metric to measure the performance of the system is the energy consumption of the device, that should satisfy the requirement.

### Aggregation on cloud

Time:

- about 400 milliseconds to sample the weight on the load cell, compute the consumption and send an MQTT message to the MQTT bridge.
- about 300 milliseconds to propagate an MQTT message to the MQTT bridge through WiFi. It depends on the WiFi protocol and on the distance between the board and the MQTT bridge. In the prototype is used an Heltec wifi-lora-32-v3, which has as WiFi protocols 802.11 b/g/n, up to 150Mbps.
- about 100 milliseconds to store the data in the DynamoDB table.
- about 400 milliseconds to compute the lambda function, to query the DynamoDB table and return the web page, in which 100 milliseconds to aggregate the data.
- total time: 1.2 seconds.

Volume:

- One message contains the deviceId and the value of the sample, so the message size is the size of two integers, which is 8 bytes. Considering that we send two messages every 30 seconds, the volume of data sent to the cloud is 16 bytes every 30 seconds, which is 32 bytes every minute, which is 1920 bytes every hour, which is 46080 bytes every day, which is 1.4 MB every month.

### Aggregation on the device

Time:

- about 400 milliseconds to sample the weight on the load cell, compute the consumption, compute the new aggrgation data (which is about 0 milliseconds since it's required only a sum) and send an MQTT message to the MQTT bridge.
- about 300 milliseconds to propagate the message to the MQTT bridge.
- about 100 milliseconds to store the data in the DynamoDB table.
- about 300 milliseconds to compute the lambda function to query the DynamoDB table and return the web page, without the aggregation of the data.
- total time: 1.1 seconds.

Volume:

- One message contains the deviceId, the value of the sample, and the aggregated data, so the message size is the size of three integers, which is 12 bytes. Considering that we send two messages every 30 seconds, the volume of data sent to the cloud is 24 bytes every 30 seconds, which is 48 bytes every minute, which is 2880 bytes every hour, which is 69120 bytes every day, which is 2.1 MB every month.

## Other resources

- [hackster.io](https://www.hackster.io/simonescaccia99/autonomous-dog-feeder-8ce118)
- [YouTube]()