from flask import Flask, send_from_directory, jsonify
import os

app = Flask(__name__)

BASE_DIR = "files"

# Supported file extensions
EXTENSIONS = [".py", ".cpp", ".c", ".java", ".sql", ".txt", ".asm"]

# ✅ Download file: /labname/qnumber
@app.route("/<lab>/<q>")
def get_file(lab, q):
    lab_path = os.path.join(BASE_DIR, lab)

    if not os.path.exists(lab_path):
        return jsonify({"error": "Lab not found"}), 404

    filename_base = f"q{q}"

    for ext in EXTENSIONS:
        filename = filename_base + ext
        file_path = os.path.join(lab_path, filename)

        if os.path.exists(file_path):
            return send_from_directory(
                lab_path,
                filename,
                as_attachment=True
            )

    return jsonify({"error": "Question not found"}), 404


# ✅ List all labs
@app.route("/labs")
def list_labs():
    labs = [
        d for d in os.listdir(BASE_DIR)
        if os.path.isdir(os.path.join(BASE_DIR, d))
    ]
    return jsonify(labs)


# ✅ List all questions in a lab
@app.route("/<lab>")
def list_questions(lab):
    lab_path = os.path.join(BASE_DIR, lab)

    if not os.path.exists(lab_path):
        return jsonify({"error": "Lab not found"}), 404

    files = os.listdir(lab_path)
    return jsonify(files)


# ✅ Health check
@app.route("/")
def home():
    return "Lab API Running 🚀"


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=10000)