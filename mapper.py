import os
import json
import streamlit as st

BASE_DIR = "files"

st.set_page_config(page_title="Semvi Labs Admin", layout="centered")

st.title("🚀 Semvi Labs Admin Panel")

# -------------------------
# Helpers
# -------------------------

def get_labs():
    return [
        d for d in os.listdir(BASE_DIR)
        if os.path.isdir(os.path.join(BASE_DIR, d))
    ]

def get_files(lab):
    lab_path = os.path.join(BASE_DIR, lab)
    return [
        f for f in os.listdir(lab_path)
        if f != "mapping.json"
    ]

def load_mapping(lab):
    path = os.path.join(BASE_DIR, lab, "mapping.json")
    if not os.path.exists(path):
        return {}
    with open(path, "r") as f:
        return json.load(f)

def save_mapping(lab, mapping):
    path = os.path.join(BASE_DIR, lab, "mapping.json")
    with open(path, "w") as f:
        json.dump(mapping, f, indent=4)

# -------------------------
# UI
# -------------------------

labs = get_labs()

if not labs:
    st.error("No labs found in 'files/' directory")
    st.stop()

lab = st.selectbox("📁 Select Lab", labs)

mapping = load_mapping(lab)
files = get_files(lab)

mapped_files = {data["file"]: qid for qid, data in mapping.items()}

# -------------------------
# Show ALL files with status
# -------------------------

st.subheader("📂 Files in Lab")

if files:
    for f in files:
        if f in mapped_files:
            st.markdown(f"🟢 **{f}** → mapped as Q{mapped_files[f]}")
        else:
            st.markdown(f"🔴 **{f}** → not mapped")
else:
    st.warning("No files found in this lab")

st.divider()

# -------------------------
# Show existing mappings
# -------------------------

st.subheader("📚 Existing Questions")

if mapping:
    for qid, data in mapping.items():
        st.markdown(f"**{qid}** → {data['title']} (`{data['file']}`)")
else:
    st.info("No questions mapped yet")

st.divider()

# -------------------------
# Add mapping
# -------------------------

st.subheader("➕ Map Existing File")

if not files:
    st.stop()

selected_file = st.selectbox("Select File", files)
title = st.text_area("Enter Question Title")

if st.button("Add Mapping"):
    if not title.strip():
        st.error("Title cannot be empty")
    else:
        # Prevent duplicate mapping
        for qid, data in mapping.items():
            if data["file"] == selected_file:
                st.error(f"File already mapped to Question {qid}")
                st.stop()

        # Generate next ID
        next_id = str(max([int(k) for k in mapping.keys()] + [0]) + 1)

        mapping[next_id] = {
            "title": title.strip(),
            "file": selected_file
        }

        save_mapping(lab, mapping)

        st.success(f"✅ Mapped as Question {next_id}")
        st.rerun()