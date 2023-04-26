# Autonomous-Dog-Feeder

## What is the problem and why do you need IoT?

Problems we want to solve:

- Dogs have a daily amount of food they need to eat, according to their age, activity level, and health condition. This amount of food is suggested by the vet or written on the dog food package. We want to make sure that the dog gets the right amount of food every day.
- Dogs need to eat their food into several meals throughout the day. Each meal should be served at a specific time, according to the dog's age and activity level or preference. We want to make sure that the dog gets the right amount of food at the right time.
- Dog owners may not be able to feed their dogs on time due to work or other commitments.
- Water is essential for dogs. We want to make sure that the dog has access to water at all times.
- We want to know the monitor the trends of the dog's eating habits and water consumption, in order to support medical diagnosis.

How we can monitor the environment?

- Food:
  - We want to measure the amount of dry food to pour in the bowl, in order to be albe to define the amount of food for each meal. We can use a load cell sensor to measure the weight of the food.
- Water:
  - We want to measure the amount of water left in the bowl.

Sensors:

- **Load cell** sensor to measure the weight of the food in the bowl.
A periodic sensing is required in order to measure over time the quantity of food in the bowl.
(**Identify a suitable sensor data prediction model.**)
- **Load cell** sensor to measure the weight of the water in the bowl in order to quantify the liters of water in the bowl (the density of the water is about 1 kg/L, considering the temperature of the water at room temperatre and the pressure at the see level).
A periodic sensing is required in order to measure over time the quantity of water in the bowl.
(**Identify a suitable sensor data prediction model.**)

Actuators:

- A **servo motor** to open and close the recipient of the food.
The activation of the servo motor will be triggered by the cloud-based service only the bowl can contain the required amount of food.
- A **servo motor** to shake the recipient of the food. The activation of the servo motor will be triggered by the cloud-based service only the bowl can contain the required amount of food.
- A **water pump** to fill the bowl with the water.
The activation of the water pump will be triggered by the load cell sensor only when the bowl doesn't contain the required amount of water.

## What data are collected and by which sensors?

- The **Load cell** cell sensor to measure the weight of the food in the bowl has a capacity of 5 kg, since the 1 kg version can not be suitable for large breeds dogs.
In fact, large breeds dogs eat about 600/700 g of food per day divided into two or three meals(the amount of food is written on the dog food package). But in special cases, like pregnancy or lactation, the food quantity can be three times the normal amount. So, if a pregnant dog eats 2,1 kg of food per day divided into two meals, a 1 kg load cell can not be suitable for this case.
Finally, we can support the fact that the resolution of the 5 kg load cell is good enough for our purpose. In fact, 