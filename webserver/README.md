# GTFS Trip Visualizer

A web application to visualize GTFS bus trips on a map.

## Setup

1. Compile the C++ program:
   ```
   clang++ tripjson.cpp -std=c++17 -o tripjson
   ```

2. Install Python dependencies:
   ```
   pip install -r requirements.txt
   ```

3. Run the server:
   ```
   python3 server.py
   ```

4. Open http://localhost:5015 in your browser.

## Usage

- Enter a Trip ID in the input field.
- Click "Load Trip" or press Enter.
- The map will display the route and stops for that trip.