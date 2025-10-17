# Static GTFS
## Information
Static GTFS Data is usually provided in a txt file in a csv format. It provides the expected times of buses, trains, trams, etc. All transit agencies should have these so transit apps and parsers can access the expected data. One the file is downloaded once it does not need to be downloaded again until the set expires.

## How is the data given?
Like I just said above the data is given in a txt file in a csv format. Each value is seperated by a comma. There are multiple files that a transit agency can provide, and some of them include:
- agency.txt (information about the agency)
- routes.txt (list of all the bus/train routes)
- trips.txt (list of **every** trip made by *EVERY SINGLE VEHICLE IN THE SYSTEM*)
- stops.txt (list of all of the stops)
- stop_times.txt (expected times vehicles are to arrive at given stops)
- shapes.txt (what path a vehicle takes along the route)

There are way more that I didn't include but you can find all of them in the [GTFS Refrence Website](https://gtfs.org/documentation/schedule/reference). As you can tell, for bigger transit agencies, the GTFS data can be a lot to handle. That's why I chose to code this in C++.

## How do I use this parser?
All you have to do is to:
1. Clone this repository onto your machine - `git clone https://github.com/JettM9104/GTFS-Parser/`
2. Create a folder called data inside the `static-gtfs` folder
3. Change the paths inside the code to match the paths of your data folder.
