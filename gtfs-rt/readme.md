# gtfs-rt

C++ tools for decoding GTFS Realtime (GTFS-RT) `.pb` protobuf files and outputting JSON. Used directly or via the webserver.

## Background

Unlike GTFS Schedule (CSV text files), GTFS-RT data is distributed as binary protobuf (`.pb`) files. Transit agencies typically provide three feeds:

| File | Contents |
|---|---|
| `TripUpdates.pb` | Expected arrival/departure times per vehicle |
| `VehiclePositions.pb` | Live vehicle positions |
| `ServiceAlerts.pb` | Real-time service alerts |

The compiled tools in `proto-conversion/webserver-implementation/` decode these feeds and output JSON to stdout.

## Setup (Mac)

These steps only need to be done once per machine.

### Step 1 — Install protobuf@21

Install [Homebrew](https://brew.sh) if you don't have it:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Then install and link protobuf:

```zsh
brew install protobuf@21
brew link --force protobuf@21
```

### Step 2 — Generate protobuf source files

Navigate to `proto-conversion/transit-files/` and run:

```zsh
protoc --cpp_out=. gtfs-realtime.proto
```

This generates `gtfs-realtime.pb.cc` and `gtfs-realtime.pb.h`.

### Step 3 — Compile the decoder binaries

Navigate to `proto-conversion/webserver-implementation/` and get your protobuf prefix:

```zsh
brew --prefix protobuf@21
```

Use that output in place of `(prefix)` below:

```zsh
clang++ -std=c++17 -O3 decodeTrip.cpp ../transit-files/gtfs-realtime.pb.cc \
  -I(prefix)/include -L(prefix)/lib -lprotobuf -o decodeTrip

clang++ -std=c++17 -O3 decodeStop.cpp ../transit-files/gtfs-realtime.pb.cc \
  -I(prefix)/include -L(prefix)/lib -lprotobuf -o decodeStop

clang++ -std=c++17 -O3 decodeAlerts.cpp ../transit-files/gtfs-realtime.pb.cc \
  -I(prefix)/include -L(prefix)/lib -lprotobuf -o decodeAlerts
```

## Usage

Each binary reads a local `.pb` file and writes JSON to stdout.

```zsh
./decodeTrip <trip_id>       # trip update for a specific trip
./decodeStop <stop_id>       # upcoming arrivals at a stop
./decodeAlerts               # all active service alerts
```

The webserver calls these automatically via its `/api/rt/` endpoints.
