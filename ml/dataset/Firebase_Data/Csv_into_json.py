import pandas as pd
import json

# Load CSV
df = pd.read_csv("01_unique_timesamples.csv")

# Convert to dictionary
data = df.to_dict(orient="index")

# Wrap inside RainData node
firebase_data = {"RainData": data}

# Save JSON
with open("firebase_dataset.json", "w") as f:
    json.dump(firebase_data, f, indent=4)

print("JSON file created successfully")