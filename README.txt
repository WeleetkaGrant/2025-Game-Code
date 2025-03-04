# Botball Game Code Repository

## Overview
Welcome to the repository for our Botball robots! This repository contains the game codes for two specialized robots designed to work together in an automated beverage preparation and delivery task.

### Robot 1: Drink Dispensing Robot
This robot is equipped with a camera to detect colored cups and match them with the appropriate beverage. It follows these steps:
1. Identifies cup colors using the camera.
2. Navigates to the beverage dispenser.
3. Fills each cup with the correctly matched drink.
4. Ensures proper placement and handling to avoid spillage.

### Robot 2: Bottle & Ice Handling Robot
This robot uses a specialized claw to handle both water bottles and ice. It performs the following tasks:
1. Picks up water bottles using a custom-designed claw.
2. Deposits bottles in the beverage station.
3. Collects a large quantity of ice.
4. Places the ice into the correctly filled cups.

## Code Structure
- `drink_robot/` - Contains code for the drink-dispensing robot.
- `bottle_ice_robot/` - Contains code for the bottle and ice-handling robot.
- `common/` - Shared functions and utilities used by both robots.

## Dependencies
- KIPR Wallaby or Link controller
- LEGO, metal, and/or 3D-printed components
- Camera for color detection
- Custom claw mechanism for bottle and ice handling

## Setup & Execution
1. Load the respective code onto the appropriate robot.
2. Ensure all mechanical components are properly attached and functional.
3. Start the robots in their designated positions.
4. Monitor their performance and adjust as needed.

## Future Improvements
- Enhance camera accuracy for better cup color recognition.
- Optimize claw grip for more secure bottle handling.
- Improve ice collection efficiency.

Feel free to contribute and improve the robots' functionality!

