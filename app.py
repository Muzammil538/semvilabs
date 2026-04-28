import os
import json
from flask import Flask, jsonify, send_file
from flask import Response
import logging


app = Flask(__name__)

BASE_DIR = os.path.join(os.getcwd(), "files")


# -------------------------
# Helpers
# -------------------------

def get_lab_path(lab):
    return os.path.join(BASE_DIR, lab)


def get_mapping(lab):
    lab_path = get_lab_path(lab)
    mapping_path = os.path.join(lab_path, "mapping.json")

    if not os.path.exists(mapping_path):
        return None

    with open(mapping_path, "r") as f:
        return json.load(f)


# -------------------------
# Routes
# -------------------------

# ✅ List all labs
@app.route("/labs")
def list_labs():
    if not os.path.exists(BASE_DIR):
        return jsonify([])

    labs = [
        d for d in os.listdir(BASE_DIR)
        if os.path.isdir(os.path.join(BASE_DIR, d))
    ]

    return jsonify(labs)


# ✅ List questions in lab
@app.route("/<lab>")
def list_questions(lab):
    mapping = get_mapping(lab)

    if mapping is None:
        return jsonify({"error": "Lab not found"}), 404

    # Sort by question ID
    sorted_items = sorted(mapping.items(), key=lambda x: int(x[0]))
    
    # Create line-wise text
    output_lines = []
    for qid, data in sorted_items:
        title = data["title"].replace("\n", " ")  # Replace newlines with spaces
        output_lines.append(f"{qid}. {title}")
    
    # Join with newlines
    text_output = "\n".join(output_lines)
    
    # Return as plain text
    return Response(text_output, mimetype='text/plain')


# ✅ Preview code (prints in terminal)
@app.route("/<lab>/<qid>")
def preview_code(lab, qid):
    mapping = get_mapping(lab)

    if mapping is None:
        return jsonify({"error": "Lab not found"}), 404

    if qid not in mapping:
        return jsonify({"error": "Question not found"}), 404

    file_name = mapping[qid]["file"]
    file_path = os.path.join(get_lab_path(lab), file_name)

    if not os.path.exists(file_path):
        return jsonify({"error": "File missing"}), 404

    with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
        return f.read(), 200, {"Content-Type": "text/plain"}


# ✅ Download file

@app.route("/<lab>/<qid>/file")
def download_file(lab, qid):
    app.logger.info(f"Download requested for lab={lab}, qid={qid}")
    
    mapping = get_mapping(lab)

    if mapping is None:
        app.logger.error(f"Lab {lab} not found")
        return jsonify({"error": "Lab not found"}), 404

    if qid not in mapping:
        app.logger.error(f"Question {qid} not found in lab {lab}")
        return jsonify({"error": "Question not found"}), 404

    # Log what's in mapping
    app.logger.info(f"Mapping for {qid}: {mapping[qid]}")
    
    actual_filename = mapping[qid].get("file")
    app.logger.info(f"Filename from mapping: {actual_filename}")
    
    if not actual_filename:
        app.logger.error(f"No 'file' key in mapping for {qid}")
        return jsonify({"error": "No file associated"}), 404

    file_path = os.path.join(get_lab_path(lab), actual_filename)
    app.logger.info(f"Full file path: {file_path}")
    app.logger.info(f"File exists: {os.path.exists(file_path)}")

    if not os.path.exists(file_path):
        return jsonify({"error": "File missing"}), 404

    return send_file(
        file_path,
        as_attachment=True,
        download_name=actual_filename
    )
# ✅ Health check
@app.route("/")
def home():
    return "Semvi Labs API Running 🚀"


# -------------------------
# Run
# -------------------------

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=10000)