import os
from flask import Flask, send_file, jsonify

app = Flask(__name__)

BASE_DIR = "files"

EXTENSIONS = [ ".cpp", ".java", ".txt",".asm"]

@app.route("/<lab>/<question>")
def get_file(lab, question):
    lab_path = os.path.join(BASE_DIR, lab)

    if not os.path.exists(lab_path):
        return jsonify({"error": "Lab not found"}), 404

    # 🔥 Search for any file starting with question name
    for file in os.listdir(lab_path):
        name, ext = os.path.splitext(file)

        if name == question:
            file_path = os.path.join(lab_path, file)

            return send_file(
                file_path,
                as_attachment=True,
                download_name=file   # ensures correct extension
            )

    return jsonify({"error": "Question not found"}), 404

# Optional: list questions in a lab
@app.route("/<lab>")
def list_questions(lab):
    lab_path = os.path.join(BASE_DIR, lab)

    if not os.path.exists(lab_path):
        return jsonify({"error": "Lab not found"}), 404

    return jsonify(os.listdir(lab_path))


# Health check
@app.route("/")
def home():
    return "Semvi Labs API Running 🚀"


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=10000)