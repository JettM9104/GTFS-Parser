# Schedule GTFS Parser (updated Apr 2026)

## Information

Transit agencies usually provide GTFS-schedule data in a `.zip` file, composed of many `.txt` files in a **csv** format. The refrence for GTFS-schedule can be found [here](https://gtfs.org/documentation/schedule/reference/).

The required files (the agency **MUST** include these):

- `agency.txt`: The information on the agency/agencies and their corresponding `agency_id`
- `routes.txt`: The information on the routes, with their corresponding `route_id`
- `trips.txt`: The information on the trips, with their corresponding `trip_id`
- `stop_times.txt`: The information the trip times making reference to `trip_id`

As you can probably guess, for larger transit agencies (for bigger cities), it can get very big (200,000+ lines!!)

Agencies are encouraged (and *sometimes* required) to include files with more info, like:

- `stops.txt`: The information on the stops and their corresponding `stop_id`, this is required unless a `location.geojson` is provided.
- `location.geojson`: The location information for on-demand service
- `calendar.txt`: The information on the trips' service schedules (based off days of the week)
- `calendar_dates.txt`: The informaton on the trips' service schedules' exceptions, like added or removed service on certain days. Some agencies could choose to just use `calendar_dates.txt` solely and omit `calendar.txt`. 

Agencies can also choose to include other files with more information, like:

- `fare_attributes.txt`: Information on the fares (i.e. where it is paid, how much it is)
- `frequencies.txt`: Information on the frequency of certain routes

Within the files, agencies can choose to include optional fields (like `route_color` inside `routes.txt` for map displays, and `block_id` inside `trips.txt` to specify which trips are conducted by the same vehicles!)


## Usage

The `gtfs.hpp` uses `fstream`'s `ifstream` to read and parse data values. Using this API without the webserver can work.

To make use, please follow these instructions, even if using the webserver:

1. Download your agency's GTFS data and create a folder `data` inside `./GTFS-Parser/static-gtfs/`. Extract the `.zip` file into the `data` folder, so the structure looks something like this:
```
static-gtfs
├── gtfs.hpp
└── data
    ├── google_transit.zip
    └── google_transit
        ├── agency.txt
        ├── trips.txt
        ├── routes.txt
        └── ... (continued)
```

1. `gtfs.hpp` line 34: update 
```cpp
const string root = "...";
```

Replace whatever's in there with the path to the folder `static-gtfs` **without** the leading `/` or `\`. You can run

```zsh
pwd
```
on mac, and 
```bash 
cd
```
(with no arguments), on windows

to get it.

2. `gtfs.hpp` line 32: update
```cpp
const string path = "/data/whatever_is_here/";
```
Replace `whatever_is_here` with the name of your extracted folder containing the gtfs files. 

3. Test it
Create a new file, `main.cpp` with this code:
```cpp
#include <iostream>
#include <ctime>
#include <iomanip>
#include "gtfs.hpp"


int main(int argc, char* argv[]) {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    std::cout << gtfs::verifyGTFS(now->tm_year + 1900, now->tm_mon + 1, now->tm_mday) << std::endl;
    return 0;
}
```

Check the output. Refer to this table:

| Program Output | Meaning |
| -------------- | ------- |
| 0              | GTFS Data read and validated |
| 1              | GTFS Data read but is expired |
| -1             | GTFS Data read but it is not yet in place |
| 10             | GTFS Data's feed_info.txt not read |
| 11             | GTFS Data read but feed_info.txt is missing feed_start_date field |
| 12             | GTFS Data read but feed_info.txt is missing feed_end_date field |
