# GTFS Parser

A C++ library and Python webserver for parsing and serving **GTFS Schedule** and **GTFS Realtime** transit data.

**GTFS** (General Transit Feed Specification) is the worldwide standard for transit agency schedule and location data. See the [official reference](https://gtfs.org/documentation/schedule/reference/) and the [`google/transit`](https://github.com/google/transit) repository.

<img width="1914" height="956" alt="image" src="https://github.com/user-attachments/assets/08733e08-7a5b-4e54-ba88-a782d231d065" />

## Components

| Directory | Description |
|---|---|
| [`static-gtfs/`](static-gtfs/) | C++ header (`gtfs.hpp`) for parsing GTFS Schedule `.txt` files |
| [`gtfs-rt/`](gtfs-rt/) | C++ tools for decoding GTFS Realtime `.pb` protobuf files |
| [`webserver/`](webserver/) | Flask server + HTML frontend that exposes both as a REST API |

## Quick Start

```bash
git clone https://github.com/JettM9104/GTFS-Parser/
```

Then follow the README in whichever component you need:

1. [static-gtfs](static-gtfs/readme.md) — parse schedule data
2. [gtfs-rt](gtfs-rt/readme.md) — decode realtime feeds
3. [webserver](webserver/README.md) — run the full web UI

## Requirements

- C++17 compiler (`clang++` or `g++`)
- Python 3 with `flask`
- `protobuf@21` (for GTFS-RT only)
- `git`

## Versions

| Release | Notes |
|---|---|
| v1.8.0 | Stable — reorganized into `gtfs.hpp`, improved GTFS-RT (JSON output) |
| v3.0 beta | HTML GUI via Flask + JavaScript frontend |
