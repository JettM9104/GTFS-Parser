from flask import Flask, jsonify, send_from_directory, request, abort # type: ignore
import subprocess
import json
import os
from functools import wraps
from t_confidental_info import token


app = Flask(__name__)

SECRET_TOKEN = token 

def require_token(f):
    @wraps(f)
    def decorated(*args, **kwargs):
        token = request.headers.get("X-Auth-Token") or request.args.get("token")
        if token != SECRET_TOKEN:
            abort(401)
        return f(*args, **kwargs)
    return decorated

@app.route('/api/trip/<trip_id>')
@require_token
def get_trip(trip_id):
    try:
        result = subprocess.run(['./tripjson', trip_id], capture_output=True, text=True)
        if result.returncode != 0:
            return jsonify({'error': result.stderr}), 500
        data = json.loads(result.stdout)
        return jsonify(data)
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/api/stop/<stop_id>/<date>')
@require_token
def get_stop(stop_id, date):
    try:
        year, month, day = date.split('-')
        result = subprocess.run(['./stopjson', stop_id, year, month, day], capture_output=True, text=True)
        if result.returncode != 0:
            return jsonify({'error': result.stderr}), 500
        data = json.loads(result.stdout)
        return jsonify(data)
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/api/nearest/<lat>/<lon>')
@require_token
def get_nearest_stops(lat, lon):
    try:
        result = subprocess.run(['./getneareststopsjson', lat, lon], capture_output=True, text=True)
        if result.returncode != 0:
            return jsonify({'error': result.stderr}), 500
        data = json.loads(result.stdout)
        return jsonify(data)
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/api/rt/location/<tripID>')
@require_token
def get_rt_location(tripID):
    try:
        result = subprocess.run(['../gtfs-rt/proto-conversion/webserver-implementation/./decodeTrip', tripID], capture_output=True, text=True)
        if result.returncode != 0:
            return jsonify({'error': result.stderr}), 500
        data = json.loads(result.stdout)
        return jsonify(data)
    except Exception as e:
        return jsonify({'error': str(e)}), 500
    
@app.route('/api/rt/stop/<stopID>')
@require_token
def get_rt_stop(stopID):
    try:
        result = subprocess.run(['../gtfs-rt/proto-conversion/webserver-implementation/./decodeStop', stopID], capture_output=True, text=True)
        if result.returncode != 0:
            return jsonify({'error': result.stderr}), 500
        data = json.loads(result.stdout)
        return jsonify(data)
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/')
@require_token
def index():
    return send_from_directory('.', 'tokenindex.html')

@app.route('/<path:path>')
@require_token
def static_files(path):
    return send_from_directory('.', path)

# OPTIONAL: OFFLINE TILES
@app.route('/tiles/<int:z>/<int:x>/<int:y>.png')
@require_token
def tiles(z, x, y):
    return send_from_directory(f'tiles/{z}/{x}', f'{y}.png')

@app.route('/api/route/<route_id>/<year>/<month>/<day>')
@require_token
def get_trip_root(route_id, year, month, day):
    try:
        result = subprocess.run(['./getTrips', route_id, year, month, day], capture_output=True, text=True)
        if result.returncode != 0:
            return jsonify({'error': result.stderr}), 500
        data = json.loads(result.stdout)
        return jsonify(data)
    except Exception as e:
        return jsonify({'error': str(e)}), 500


@app.route("/crash")
@require_token
def home():
    raise Exception("test error") 


if __name__ == '__main__':
    app.run(debug=True, port=5015, host = '0.0.0.0')
