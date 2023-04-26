# Autonomous-Dog-Feeder

## What is the problem and why do you need IoT?

Problems we want to solve:

- Dogs have a daily amount of food they need to eat, according to their age, activity level, and health condition. This amount of food is suggested by the vet or written on the dog food package. We want to make sure that the dog gets the right amount of food every day.
- Dogs need to eat their food several meals throughout the day. Each meal should be served at a specific time, according to the dog's age and activity level or preference. We want to make sure that the dog gets the right amount of food at the right time.
- Dog owners may not be able to feed their dogs on time due to work or other commitments.
- Water is essential for dogs. We want to make sure that the dog has access to water at all times.
- We want to monitor the trends of the dog's eating habits and water consumption, to support the medical diagnosis.

How we can monitor the environment?

- Food:
  - We want to measure the amount of dry food to pour in the bowl, to be able to define the amount of food for each meal. We can use a load cell sensor to measure the weight of the food.
- Water:
  - We want to measure the amount of water left in the bowl.

Sensors:

- **Load cell** sensor to measure the weight of the food in the bowl.
Periodic sensing is required to measure over time the quantity of food in the bowl.
(**Identify a suitable sensor data prediction model.**)
- **Load cell** sensor to measure the weight of the water in the bowl to quantify the liters of water in the bowl (the density of the water is about 1 Kg/L, considering the temperature of the water at room temperature and the pressure at sea level).
Periodic sensing is required to measure over time the quantity of water in the bowl.
(**Identify a suitable sensor data prediction model.**)

Actuators:

- A **servo motor** to open and close the recipient of the food.
The activation of the servo motor will be triggered by the cloud-based service only the bowl can contain the required amount of food.
- A **servo motor** to shake the recipient of the food. The activation of the servo motor will be triggered by the cloud-based service only the bowl can contain the required amount of food.
- A **water pump** fills the bowl with the water.
The activation of the water pump will be triggered by the load cell sensor only when the bowl doesn't contain the required amount of water.

## What data are collected and by which sensors?

- Sensors accuracy, the unit of measurement and periodicity:
  - The **load cell** sensor to measure the weight of the food in the bowl has a capacity of 5 Kg since the 1 Kg load cell is not suitable for large breeds of dogs.
  Large breeds of dogs eat about 600/700 g of food per day divided into two or three meals(the amount of food is written on the dog food package). But in special cases, like pregnancy or lactation, the food quantity can be three times the normal amount. So, if a pregnant dog eats 2.1 Kg of food per day divided into two meals, a 1 Kg load cell can not be suitable for this case. More, the tare i.e. the weight of the bowl should be considered too.  
  Also, we can support the fact that the resolution of the 5 Kg load cell is good enough for our purpose. The ESP32 is connected to the load cell via the HX711 amplifier, which has a resolution of 24 bits. This means that the load cell can measure the weight of the food in the bowl with a resolution of 5/2^24 = 0.0000003 Kg = 0.0003 g.
  The chosen unit of measurement is grams (g) and is set using a calibration factor.
  To choose the periodicity of the sensing we need to consider the fact that we are interested in monitoring the dog's eating habits, like at which time the dog starts eating considering that the food is scheduled to be served at a specific time, and the speed of eating the food in the bowl. So, to reconstruct the dog's eating velocity, we can consider the time between two consecutive bites. This period lasts a maximum of 5 seconds and depends heavily on the dog. Considering the sampling theorem, we need to sample every at least 2.5 seconds, so we can choose a sampling period of 2 seconds.
  - The **load cell** sensor to measure the weight of the water in the bowl has a capacity of 5 Kg since the weight of the bowl can saturate the 1 Kg load cell.
  The chosen unit of measurement is liters (L) and is set using a calibration factor since we can assume 1 Kg = 1 L.
  To choose the periodicity of the sensing we need to consider the fact that we are interested in having water in the bowl at all times and monitoring the dog's water consumption habits, like how much water the dog drinks during the day.
  So, to have water in the bowl at all times, we can consider that the dog drinks continuously for a maximum of 1 minute and after that, the bowl can be empty, so we need to refill it.
  Using the sampling theorem, we need to sample every at least 30 seconds, so we can choose a sampling period of 25 seconds.
