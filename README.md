# FilamentCarer

Schnoogs experiment to manage the filament for 3D printer

## Idea & Inspiration
The project is born out of 2 separate issues.
**Issue 1:**
Ever run a long time 3D print and a few layers before the print should have been finished, the filament ran out?
OK, why not use a filament sensor?
I do, but pause the print is not always the best solution. Having the printer bed and nozzle heated for hours degrading the filament?
I want to know how much filament is left on the spool, prior to starting the print job.
**Issue 2:**
Filament is hygroscopic and absorbed water will result in bad prints.
A common approach seems to be to bag the spools in plastic, add some desiccant and seal the bag.
But what’s with the loaded filament? Unload it every time? Use a plastic bag and some adhesive tape to seal it?
And what’s about the desiccant? After years in the chemical laboratory I got very used  to the standard silica desiccant, which doesn’t mean I like it.
I wanted a creative, maker solution.

## My Solution
**Case:** I use a poly acrylic case, made of 8mm plates. (50 x 40 x 45 – l x b x h). 
Filament amount: The filament spool holder will have an integrated load cell which is able to record the spools weight.
Each spool is identified by an attached RFID Card. The data is sent to a local MQTT server

**Filament humidity:** I use a saturated lithium chloride solution. This is also a well known desiccant which goes down to 11.3% RF. To be able to control the humidity to my designates target range, a small 5V air pump blows the air from the chamber through the desiccant solution which is placed within the chamber.
The humidity control works autonomous but send however the state to a local MQTT server and also is able to execute basic commands.

### What is required
- NodeMCU
- HX711 board with a 5kg load cell
- DHT22 sensor 
- 5V Air Pump 
- 10k & 330 Ohm resistor (each 1x)
- IRLZ34N n-channel logic level MOSFET
- Solder utilities
- RC522 RF IC Reader Sensor Modul
- Gas wash bottle
- A little development PCB
- Lithium chlorid + water 




