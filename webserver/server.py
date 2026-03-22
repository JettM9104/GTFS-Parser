from flask import Flask, jsonify, send_from_directory
import subprocess
import json
import os

app = Flask(__name__)

@app.route('/api/trip/<trip_id>')
def get_trip(trip_id):
    try:
        result = subprocess.run(['./tripjson', trip_id], capture_output=True, text=True)
        if result.returncode != 0:
            return jsonify({'error': result.stderr}), 500
        data = json.loads(result.stdout)
        return jsonify(data)
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/api/stop/<stop_id>')
def get_stop(stop_id):
    try:
        result = subprocess.run(['./stopjson', stop_id], capture_output=True, text=True)
        if result.returncode != 0:
            return jsonify({'error': result.stderr}), 500
        data = json.loads(result.stdout)
        return jsonify(data)
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/api/nearest/<lat>/<lon>')
def get_nearest_stops(lat, lon):
    try:
        result = subprocess.run(['./getneareststopsjson', lat, lon], capture_output=True, text=True)
        if result.returncode != 0:
            return jsonify({'error': result.stderr}), 500
        data = json.loads(result.stdout)
        return jsonify(data)
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/')
def index():
    return send_from_directory('.', 'index.html')

@app.route('/<path:path>')
def static_files(path):
    return send_from_directory('.', path)

# OPTIONAL: OFFLINE TILES
@app.route('/tiles/<int:z>/<int:x>/<int:y>.png')
def tiles(z, x, y):
    return send_from_directory(f'tiles/{z}/{x}', f'{y}.png')

@app.route("/crash")
def home():
    raise Exception("test error") 


if __name__ == '__main__':
    app.run(debug=True, port=5015, host = '0.0.0.0')
