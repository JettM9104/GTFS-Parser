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

### Step 1 — Install protobuf

Install [Homebrew](https://brew.sh) if you don't have it:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Then install and link the latest protobuf (currently 35.x, which pulls in `abseil` as a dependency):

```zsh
brew install protobuf pkg-config
brew link --force protobuf
```

### Step 2 — Generate protobuf source files

Navigate to `proto-conversion/transit-files/` and run:

```zsh
protoc --cpp_out=. gtfs-realtime.proto
```

This generates `gtfs-realtime.pb.cc` and `gtfs-realtime.pb.h`.

### Step 3 — Compile the decoder binaries

Modern protobuf (22+) depends on `abseil`, which means linking now needs dozens of `absl_*` libraries in addition to `-lprotobuf`. Rather than listing them by hand, use `pkg-config` to generate the right flags.

The repo-root `Makefile` does this for you and only rebuilds what's stale:

```zsh
cd ..
make rt
```

To compile manually instead, navigate to `proto-conversion/webserver-implementation/` and run:

```zsh
clang++ -std=c++17 -O3 decodeTrip.cpp ../transit-files/gtfs-realtime.pb.cc \
  $(pkg-config --cflags --libs protobuf) -o decodeTrip

clang++ -std=c++17 -O3 decodeStop.cpp ../transit-files/gtfs-realtime.pb.cc \
  $(pkg-config --cflags --libs protobuf) -o decodeStop

clang++ -std=c++17 -O3 decodeAlerts.cpp ../transit-files/gtfs-realtime.pb.cc \
  $(pkg-config --cflags --libs protobuf) -o decodeAlerts
```


## Setup (Ubuntu/Linux)

These steps only need to be done once per machine.

### Step 1 — Install protobuf

```bash
sudo apt update
sudo apt install -y protobuf-compiler libprotobuf-dev pkg-config build-essential
```

### Step 2 — Generate protobuf source files

Navigate to `proto-conversion/transit-files/` and run:

```bash
protoc --cpp_out=. gtfs-realtime.proto
```

This generates `gtfs-realtime.pb.cc` and `gtfs-realtime.pb.h`.

### Step 3 — Compile the decoder binaries

The repo-root `Makefile` does this for you and only rebuilds what's stale:

```bash
cd ..
make rt
```

To compile manually instead, navigate to `proto-conversion/webserver-implementation/` and run:

```bash
g++ -std=c++17 -O3 decodeTrip.cpp ../transit-files/gtfs-realtime.pb.cc \
  $(pkg-config --cflags --libs protobuf) -o decodeTrip

g++ -std=c++17 -O3 decodeStop.cpp ../transit-files/gtfs-realtime.pb.cc \
  $(pkg-config --cflags --libs protobuf) -o decodeStop

g++ -std=c++17 -O3 decodeAlerts.cpp ../transit-files/gtfs-realtime.pb.cc \
  $(pkg-config --cflags --libs protobuf) -o decodeAlerts
```

## Setup (Windows)

These steps only need to be done once per machine. Use a terminal with a C++ toolchain on `PATH` — either the "Developer PowerShell for VS" that ships with Visual Studio (MSVC), or MSYS2/MinGW64.

### Step 1 — Install protobuf

Easiest via [vcpkg](https://github.com/microsoft/vcpkg):

```powershell
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install protobuf:x64-windows
```

This gives you `protoc.exe` (under `vcpkg\installed\x64-windows\tools\protobuf\`) plus the protobuf headers/libs.

If you're on MSYS2/MinGW64 instead, you can just use pacman:

```bash
pacman -S mingw-w64-x86_64-protobuf mingw-w64-x86_64-pkg-config mingw-w64-x86_64-toolchain
```

### Step 2 — Generate protobuf source files

Navigate to `proto-conversion/transit-files/` and run (adjust the path to `protoc.exe` for your setup):

```powershell
protoc.exe --cpp_out=. gtfs-realtime.proto
```

This generates `gtfs-realtime.pb.cc` and `gtfs-realtime.pb.h`.

### Step 3 — Compile the decoder binaries

The repo's Makefile targets (`make rt`) assume a Unix-like shell, so on plain MSVC/PowerShell it's easiest to compile manually. Navigate to `proto-conversion/webserver-implementation/` and run, with `<vcpkg>` replaced by your vcpkg root:

```powershell
cl /std:c++17 /O2 /EHsc decodeTrip.cpp ..\transit-files\gtfs-realtime.pb.cc `
  /I <vcpkg>\installed\x64-windows\include `
  /link /LIBPATH:<vcpkg>\installed\x64-windows\lib libprotobuf.lib

# repeat for decodeStop.cpp and decodeAlerts.cpp
```

On MSYS2/MinGW64 (bash), the same `pkg-config`-based invocation from the Mac/Linux sections above works as-is with `g++`:

```bash
g++ -std=c++17 -O3 decodeTrip.cpp ../transit-files/gtfs-realtime.pb.cc \
  $(pkg-config --cflags --libs protobuf) -o decodeTrip.exe
```

## Usage

Each binary reads a local `.pb` file and writes JSON to stdout.

```zsh
./decodeTrip <trip_id>       # trip update for a specific trip
./decodeStop <stop_id>       # upcoming arrivals at a stop
./decodeAlerts               # all active service alerts
```

The webserver calls these automatically via its `/api/rt/` endpoints.



## Common Issues

If you get an error about `options.always_print_fields_with_no_presence = true;`, change the line to `options.always_print_primitive_fields = true;`. This is because your `protobuf` version is too old but it can still work.