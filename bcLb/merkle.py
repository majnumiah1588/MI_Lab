import hashlib

def h(x):
    return hashlib.sha256(x.encode()).hexdigest()

# ---------- Merkle Root ----------
def merkle(txs):
    level = [h(t) for t in txs]
    for t, x in zip(txs, level):
        print(t, "---->", x)

    step = 0
    while len(level) > 1:
        step += 1
        print("\nLevel", step, "Hash Generation")

        if len(level) % 2 == 1:                 # odd হলে শেষটা duplicate
            level.append(level[-1])
            print("Odd number of nodes detected.")
            print("Duplicating last hash.")

        next_level = []
        for i in range(0, len(level), 2):
            left  = level[i]
            right = level[i + 1]
            parent = h(left + right)
            print("\nPair:")
            print("Left :", left)
            print("Right:", right)
            print("Parent Hash:")
            print(parent)
            next_level.append(parent)

        level = next_level

    print("\n========== MERKLE ROOT ==========")
    return level[0]

# ---------- Build all levels (proof-এর জন্য) ----------
def build_levels(txs):
    levels = [[h(t) for t in txs]]
    while len(levels[-1]) > 1:
        cur = levels[-1]
        if len(cur) % 2 == 1:                   # odd হলে duplicate
            cur = cur + [cur[-1]]
        nxt = []
        for i in range(0, len(cur), 2):
            nxt.append(h(cur[i] + cur[i + 1]))
        levels.append(nxt)
    return levels

# ---------- Merkle Proof ----------
def proof(txs, target):
    if target not in txs:
        return None

    levels = build_levels(txs)
    index = txs.index(target)
    path = []

    for level in levels[:-1]:                  # root level বাদ
        if len(level) % 2 == 1:
            level = level + [level[-1]]

        sibling = index ^ 1                    # জোড়া: 0↔1, 2↔3, ...
        direction = 'L' if sibling < index else 'R'
        path.append((level[sibling], direction))

        index //= 2                            # উপরে উঠলাম

    return path

# ---------- Verify ----------
def check(txs, target, root):
    path = proof(txs, target)
    if path is None:
        return False

    current = h(target)
    for sibling, direction in path:
        if direction == 'L':
            current = h(sibling + current)     # sibling বামে
        else:
            current = h(current + sibling)     # sibling ডানে
    return current == root

# ---------- Main ----------
txs  = ["Apple", "Orange", "Banana", "Mango"]
root = merkle(txs)
print("\nFinal Merkle Root:")
print(root)

for s in ["Banana", "Grape"]:
    print(f"\n'{s}' in tree? {check(txs, s, root)}")