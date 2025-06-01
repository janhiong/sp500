from flask import Flask, request, jsonify, send_from_directory
from flask_cors import CORS
import subprocess
import os

app = Flask(__name__)
CORS(app, supports_credentials=True, origins=["http://127.0.0.1:5000"])

BACKEND_EXEC = "../backend/sim"

@app.route("/signup", methods=["POST", "OPTIONS"])
def signup():
    if request.method == "OPTIONS":
        return jsonify({"message": "CORS preflight"}), 200

    data = request.json
    result = subprocess.run([BACKEND_EXEC, "signup", data["username"], data["password"]],
                            capture_output=True)
    stdout = result.stdout.decode(errors='ignore')
    stderr = result.stderr.decode(errors='ignore')

    print("📤 STDOUT:", stdout)
    print("🛑 STDERR:", stderr)

    return jsonify({"output": stdout.strip()})


@app.route("/login", methods=["POST", "OPTIONS"])
def login():
    if request.method == "OPTIONS":
        return '', 200

    data = request.json
    result = subprocess.run([BACKEND_EXEC, "login", data["username"], data["password"]],
                            capture_output=True)
    stdout = result.stdout.decode(errors='ignore')
    stderr = result.stderr.decode(errors='ignore')

    print("📤 STDOUT:", stdout)
    print("🛑 STDERR:", stderr)

    return jsonify({"output": stdout.strip()})


@app.route("/buy", methods=["POST", "OPTIONS"])
def buy():
    if request.method == "OPTIONS":
        return '', 200

    data = request.json
    result = subprocess.run([BACKEND_EXEC, "buy", data["username"], data["symbol"],
                             str(data["shares"]), data["date"]],
                            capture_output=True)
    stdout = result.stdout.decode(errors='ignore')
    stderr = result.stderr.decode(errors='ignore')

    print("📤 STDOUT:", stdout)
    print("🛑 STDERR:", stderr)

    return jsonify({"output": stdout.strip()})


@app.route("/sell", methods=["POST", "OPTIONS"])
def sell():
    if request.method == "OPTIONS":
        return '', 200

    data = request.json
    result = subprocess.run([BACKEND_EXEC, "sell", data["username"], data["symbol"],
                             str(data["shares"]), data["date"]],
                            capture_output=True)
    stdout = result.stdout.decode(errors='ignore')
    stderr = result.stderr.decode(errors='ignore')

    print("📤 STDOUT:", stdout)
    print("🛑 STDERR:", stderr)

    return jsonify({"output": stdout.strip()})


@app.route("/portfolio", methods=["GET"])
def get_portfolio():
    username = request.args.get("username")
    result = subprocess.run([BACKEND_EXEC, "portfolio", username],
                            capture_output=True)
    stdout = result.stdout.decode(errors='ignore')
    stderr = result.stderr.decode(errors='ignore')

    print("📤 STDOUT:", stdout)
    print("🛑 STDERR:", stderr)

    return jsonify({"output": stdout.strip()})


@app.route("/")
def serve_index():
    full_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../frontend"))
    return send_from_directory(full_path, "index.html")

@app.route("/app.js")
def serve_js():
    return send_from_directory("../frontend", "app.js")

@app.route("/styles.css")
def serve_css():
    return send_from_directory("../frontend", "styles.css")

if __name__ == "__main__":
    app.run(debug=True)