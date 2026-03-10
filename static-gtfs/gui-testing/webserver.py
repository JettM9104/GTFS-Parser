from flask import Flask, send_from_directory
import os

app = Flask(__name__)

# Serve index.html at root
@app.route("/")
def index():
    return send_from_directory(os.getcwd(), "index.html")  # <-- current folder

if __name__ == "__main__":
    app.run(port=5000)