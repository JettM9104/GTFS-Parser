# webserver

Flask server that bridges the C++ GTFS parsers to an HTML/JavaScript frontend. Runs on `http://localhost:5015`.

## Setup (Mac)

### Step 1 — Create a virtual environment

```zsh
python3 -m venv .venv
source .venv/bin/activate
```

### Step 2 — Install dependencies

```zsh
python3 -m pip install flask
```

### Step 3 — Compile C++ backends

From the `webserver/` directory:

```zsh
find . -name "*.cpp" | xargs -I{} bash -c 'clang++ -std=c++17 -O3 -o "${0%.cpp}" "$0"' {}
```

### Step 4 — Compile GTFS-RT decoders

Follow the [gtfs-rt README](../gtfs-rt/readme.md) to build `decodeTrip`, `decodeStop`, and `decodeAlerts`.

### Step 5 — Start the server

```zsh
python3 server.py
```

Open `http://localhost:5015` in your browser.

## API Endpoints

| Method | Endpoint | Description |
|---|---|---|
| GET | `/api/trip/<trip_id>` | Schedule data for a trip |
| GET | `/api/stop/<stop_id>/<YYYY-MM-DD>` | Arrivals at a stop on a given date |
| GET | `/api/nearest/<lat>/<lon>` | Nearest stops to a coordinate |
| GET | `/api/route/<route_id>/<year>/<month>/<day>` | All trips for a route on a date |
| GET | `/api/rt/location/<trip_id>` | Live vehicle location for a trip |
| GET | `/api/rt/stop/<stop_id>` | Live arrivals at a stop |

## Token Authentication (optional)

To require a token for all requests, run the token server instead:

```zsh
python3 tokenindex_converter.py | python3 tokenserver_converter.py
```

Create `t_confidental_info.py` with your token:

```py
token = "your_secret_token"
```

Generate a secure token:

```zsh
python3 -c "import secrets; print(secrets.token_hex(32))"
```

Access the server with: `http://localhost:5015/?token=your_secret_token`

## Offline Map Tiles (optional)

Place map tiles in `tiles/<z>/<x>/<y>.png`. The server serves them at `/tiles/<z>/<x>/<y>.png` for use with a Leaflet or similar map frontend.
