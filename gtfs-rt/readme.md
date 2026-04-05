# GTFS-RT
## Information
Unlike Static GTFS which is given in txt files in a csv format, the information is given in a *probuf* file (.pb file) which can then be converted into a txt file in whatever format is best for the user.

## How is the data given?
Transit Agencies usually give 3 .pb files avaliable to download:
- TripUpdates.pb (vehicle expected times)
- VehiclePositions.pb (positions of the vehicles)
- ServiceAlerts.pb (real-time service alerts)

These are usually the 3 that a transit agency provides

--- 

## How do I use the GTFS-RT Converter?
All I gave was the code. It's a big pain for you to get ready, but if you have too much spare time, go for it. (Most of the steps you only have to do once if you use the same computer and OS).
**MAC**

**Step 1**: Install `protobuf@21`
Using homebrew, open the terminal and run
```zsh
brew install protobuf@21
```
and
```zsh
brew link --force protobuf@21
```

If you don't have homebrew, install it with 
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
**Step 2**: Locate `protobuf@21`
