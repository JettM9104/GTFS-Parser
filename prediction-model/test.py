import math
import torch
import os
import csv
import json

DATA_FILE = "data.csv"
WEIGHTS_PATH = "model_weights.pt"

CONTINUOUS_COLUMNS = ["temperature", "precipitation", "rain", "showers", "snowfall"]
OUTPUT_COLUMN = "delay"

# unix_timestamp is seconds since last Monday 00:00 UTC, so it wraps weekly.
WEEK_SECONDS = 7 * 24 * 3600

class test_neural_network(torch.nn.Module):
    def __init__(self, in_size, hidden_size, out_size):
        super().__init__()
        self.net = torch.nn.Sequential(
            torch.nn.Linear(in_size, hidden_size),
            torch.nn.ReLU(),
            torch.nn.Linear(hidden_size, out_size)
        )
    def forward(self, x):
        return self.net(x)

def save_weights_readable(state_dict, path):
    # Plain-text JSON instead of a pickled blob, since the hidden layer is only
    # 16 neurons wide the whole state_dict is small enough to eyeball directly.
    readable = {name: tensor.tolist() for name, tensor in state_dict.items()}
    with open(path, "w") as f:
        json.dump(readable, f, indent=2)

def load_weights_readable(path):
    with open(path) as f:
        readable = json.load(f)
    return {name: torch.tensor(values) for name, values in readable.items()}

def load_data(data_file):
    rows = []
    outputs_data = []
    with open(data_file, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            if row["route_id"] == "":
                continue  # skip rows we couldn't match a route for
            rows.append(row)
            outputs_data.append([float(row[OUTPUT_COLUMN])])
    return rows, outputs_data

def build_route_vocab(rows):
    return sorted({row["route_id"] for row in rows})

def encode_time(unix_timestamp):
    angle = 2 * math.pi * (float(unix_timestamp) % WEEK_SECONDS) / WEEK_SECONDS
    return [math.sin(angle), math.cos(angle)]

def encode_route(route_id, route_vocab):
    one_hot = [0.0] * len(route_vocab)
    if route_id in route_vocab:
        one_hot[route_vocab.index(route_id)] = 1.0
    return one_hot

def build_feature_vector(row, route_vocab):
    continuous = [float(row[col]) for col in CONTINUOUS_COLUMNS]
    return continuous + encode_time(row["unix_timestamp"]) + encode_route(row["route_id"], route_vocab)

rows, outputs_data = load_data(DATA_FILE)
route_vocab = build_route_vocab(rows)

inputs = [build_feature_vector(row, route_vocab) for row in rows]

X_raw = torch.tensor(inputs)
y = torch.tensor(outputs_data)

# Standardize only the continuous columns; sin/cos and one-hot columns are already bounded.
n_continuous = len(CONTINUOUS_COLUMNS)
X_mean = torch.zeros(X_raw.shape[1])
X_std = torch.ones(X_raw.shape[1])
X_mean[:n_continuous] = X_raw[:, :n_continuous].mean(dim=0)
X_std[:n_continuous] = X_raw[:, :n_continuous].std(dim=0)
X_std[X_std == 0] = 1.0  # avoid divide-by-zero on constant columns
X = (X_raw - X_mean) / X_std

y_mean = y.mean(dim=0)
y_std = y.std(dim=0)
y_std[y_std == 0] = 1.0
y_norm = (y - y_mean) / y_std

model = test_neural_network(in_size=X.shape[1], hidden_size=16, out_size=1)
criterion = torch.nn.MSELoss()
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

# --- Ask user for number of epochs ---
epochs = int(input("Enter number of epochs to train (0 to skip training and go straight to inference): "))

if epochs == 0:
    # Skip training, load existing weights instead
    if os.path.exists(WEIGHTS_PATH):
        model.load_state_dict(torch.load(WEIGHTS_PATH))
        print(f"Loaded saved weights from {WEIGHTS_PATH}")
    else:
        print(f"No saved weights found at {WEIGHTS_PATH} — model is untrained (random weights).")
else:
    if os.path.exists(WEIGHTS_PATH):
        model.load_state_dict(torch.load(WEIGHTS_PATH))
        print(f"Loaded saved weights from {WEIGHTS_PATH}, continuing training")

    for epoch in range(epochs):
        optimizer.zero_grad()
        preds = model(X)
        loss = criterion(preds, y_norm)
        loss.backward()
        optimizer.step()

        if (epoch + 1) % 20 == 0 or epoch == 0:
            print(f"Epoch {epoch+1}/{epochs}, Loss: {loss.item():.4f}")

        if (epoch + 1) % 10000 == 0:
            torch.save(model.state_dict(), WEIGHTS_PATH)
            print(f"Checkpoint: saved weights at epoch {epoch+1} to {WEIGHTS_PATH}")

    # Save weights after training
    torch.save(model.state_dict(), WEIGHTS_PATH)
    print(f"Saved trained weights to {WEIGHTS_PATH}")

# --- Using the trained model ---
model.eval()
print("\nEnter values for inference:")
continuous_values = [float(input(f"{col}: ")) for col in CONTINUOUS_COLUMNS]
unix_timestamp = input("unix_timestamp: ")
route_id = input("route_id: ")

if route_id not in route_vocab:
    print(f"Warning: route_id {route_id!r} was not seen during training, using an unknown-route encoding.")

values = continuous_values + encode_time(unix_timestamp) + encode_route(route_id, route_vocab)

with torch.no_grad():
    test_input_raw = torch.tensor([values])
    test_input = (test_input_raw - X_mean) / X_std
    predicted_delay = model(test_input) * y_std + y_mean
    print(f"Predicted delay: {predicted_delay.item():.2f}")
