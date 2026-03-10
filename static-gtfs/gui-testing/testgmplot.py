from flask import Flask, jsonify, send_from_directory
import subprocess
import os

app = Flask(__name__)

# Serve your HTML
@app.route("/")
def index():
    return send_from_directory(os.getcwd(), "index.html")

# Bus times API
@app.route("/bustimes/<bus_id>")
def bus_times(bus_id):
    result = subprocess.run(["/path/to/bus_times_executable", bus_id],
                            capture_output=True, text=True)
    return jsonify({"bus_id": bus_id, "times": result.stdout.splitlines()})

# Trip stops API
@app.route("/tripstops/<trip_id>")
def trip_stops(trip_id):
    result = subprocess.run(["/Users/jettmu/Documents/VSCode/GTFS Parser/static-gtfs/build/get_trip_stops", trip_id],
                            capture_output=True, text=True)
    stops = eval(result.stdout)  # Simple JSON parsing, in production use json.loads
    return jsonify({"trip_id": trip_id, "stops": stops})

if __name__ == "__main__":
    app.run(port=5000)