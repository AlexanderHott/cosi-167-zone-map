import itertools

import matplotlib.pyplot as plt
import numpy as np

raw = [
    105604506,
    242824120.666667,
    73.333333,
    52765387,
    15329.333333,
    53251569.333333,
    52941525,
    14374.333333,
    4920272123.333333,
    1066178953,
    422687877.000000,
    106.666667,
    1064531949,
    19133.333333,
    152309561.666667,
    1069647295,
    21698.333333,
    5066319744.000000,
]

assert len(raw) % 3 == 0, "must have triplets of data"

build_times = []
pq_times = []
rq_times = []

workloads = ["W1", "W2", "W3", "W4", "W5", "W6"]
for i in range(0, len(raw), 3):
    build, pq, rq = raw[i : i + 3]
    build_times.append(build)
    pq_times.append(pq)
    rq_times.append(rq)

n = len(workloads)

bar_width = 5
group_gap = bar_width + 10
index = np.arange(n) * (bar_width * 2 + group_gap)

fig, ax = plt.subplots()

# Create the bar charts
build_bar = ax.bar(
    index + bar_width * 0.5,
    build_times,
    bar_width * 0.9,
    label="Build Time",
    color="#fbbf24",
)
pq_bar = ax.bar(
    index + bar_width * 1.5, pq_times, bar_width * 0.9, label="PQ Time", color="#a3e635"
)
rq_bar = ax.bar(
    index + bar_width * 2.5, rq_times, bar_width * 0.9, label="RQ Time", color="#38bdf8"
)

# Add labels, title, and ticks
ax.set_xlabel("Workloads")
ax.set_ylabel("Time (ns)")
ax.set_yscale("log")
ax.set_title("Zonemap time comparison")
ax.set_xticks(index + bar_width / 2)
ax.set_xticklabels(workloads)
ax.legend(loc="upper left", bbox_to_anchor=(1, 1))  # Position legend outside the plot


# Add value labels on top of each bar
def add_value_labels(bars):
    for bar in bars:
        height = bar.get_height()
        ax.annotate(
            "",
            # f"{round(height):.2E}",
            xy=(bar.get_x() + bar.get_width() / 2, height),
            xytext=(0, 5),  # Offset label above the bar
            textcoords="offset points",
            ha="center",
            va="bottom",
        )


add_value_labels(build_bar)
add_value_labels(pq_bar)
add_value_labels(rq_bar)

plt.tight_layout(rect=(0, 0, 0.95, 1))  # Reserve space on the right for the legend
plt.savefig("bar_graph.png", dpi=400)
