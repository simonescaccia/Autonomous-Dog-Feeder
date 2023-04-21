# Autonomous-Dog-Feeder

## Preface

The goal of the assignment is to create a cloud-based IoT system that:

- collects information from a set of environmental sensors:
  - load cell sensor to measure the weight of the food in the bowl.
  - load cell sensor to measure the weight of the water in the bowl.
- and interacts with the environment using actuators following the 'Sense-Think-Act' paradigm:
  - two servo motors to pour food in the bowl (one for shaking, one to open/close).
  - servo motor to pour water in the bowl.

The IoT device will be developed using RIOT-OS while the cloud-based services will be based on the AWS cloud-based service ecosystem.

## What is the problem and why do you need IoT?

Problems we want to solve:

- Dogs have a daily amount of food they need to eat, according to their age, activity level, and health condition. This amount of food is suggested by the vet or written on the dog food package. We want to make sure that the dog gets the right amount of food every day.
- Dogs need to eat their food into several meals throughout the day. Each meal should be served at a specific time, according to the dog's age and activity level or preference. We want to make sure that the dog gets the right amount of food at the right time.
- Dog owners may not be able to feed their dogs on time due to work or other commitments.
- Water is essential for dogs. We want to make sure that the dog has access to water at all times.
- We want to know the monitor the trends of the dog's eating habits and water consumption, in order to support medical diagnosis.

How we can monitor the environment?

- Food:
  - We want to measure the amount of food to pour in the bowl, in order to be albe to define the amount of food for each meal. We can use a load cell sensor to measure the weight of the food.
- Water:
  - We want to measure the amount of water left in the bowl.

Sensors:

- Load cell sensor to measure the weight of the food in the bowl.
A periodic sensing is required in order to measure over time the quantity of food in the bowl.
(**Identify a suitable sensor data prediction model.**)
- Load cell sensor to measure the weight of the water in the bowl in order to quantify the liters of water in the bowl (the density of the water is about 1 kg/L, considering the temperature of the water at room temperatre and the pressure at the see level).
A periodic sensing is required in order to measure over time the quantity of water in the bowl.
(**Identify a suitable sensor data prediction model.**)

Actuators:

- A servo motor to open and close the recipient of the food.
The activation of the servo motor will be triggered by the cloud-based service only the bowl can contain the required amount of food.
- A servo motor to shake the recipient of the food. The activation of the servo motor will be triggered by the cloud-based service only the bowl can contain the required amount of food.
- A servo motor to open and close the recipient of the water.
The activation of the servo motor will be triggered if the water load cell sensor detects that the water level is below a certain threshold.

## What data are collected and by which sensors?
