from flask import Flask, send_from_directory, jsonify
import os

app = Flask(__name__)

BASE_DIR = os.path.join(os.getcwd(), "files")

EXTENSIONS = [ ".cpp", ".java", ".txt",".asm"]

@app.route("/<lab>/<question>")
def get_file(lab, question):
    lab_path = os.path.join(BASE_DIR, lab)

    if not os.path.exists(lab_path):
        return jsonify({"error": "Lab not found"}), 404

    # sanitize input (important)
    question = question.strip().lower()

    for ext in EXTENSIONS:
        filename = question + ext
        file_path = os.path.join(lab_path, filename)

        if os.path.exists(file_path):
            return send_from_directory(
                lab_path,
                filename,
                as_attachment=True
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