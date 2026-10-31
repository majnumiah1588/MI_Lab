import hashlib

def h(d):
    return hashlib.sha256(d.encode()).hexdigest()

def combine(a, b):
    # ছোট hash আগে, বড় hash পরে — direction-free
    return h(a + b) if a < b else h(b + a)

def merkle(txs):
    cur = [h(t) for t in txs]
    for t, x in zip(txs, cur):
        print(t, "---->", x)
    lvl = 0
    while len(cur) > 1:
        lvl += 1
        print("\nLevel", lvl, "Hash Generation")
        if len(cur) % 2:
            cur.append(cur[-1])
        nxt = []
        for i in range(0, len(cur), 2):
            nxt.append(combine(cur[i], cur[i + 1]))   # ← sorted
        cur = nxt
    return cur[0]

def proof(txs, s):
    lv = [[h(t) for t in txs]]
    while len(lv[-1]) > 1:
        c = lv[-1]
        if len(c) % 2:
            c = c + [c[-1]]
        lv.append([combine(c[i], c[i + 1]) for i in range(0, len(c), 2)])
    if s not in txs:
        return None
    i, p = txs.index(s), []
    for l in lv[:-1]:
        if len(l) % 2:
            l = l + [l[-1]]
        p.append(l[i ^ 1])            # ← শুধু sibling hash, direction নেই
        i //= 2
    return p

def check(txs, s, root):
    p = proof(txs, s)
    if p is None:
        return False
    c = h(s)
    for sibling in p:
        c = combine(c, sibling)       # ← direction-free
    return c == root

# ---- Run ----
txs = ["Apple", "Orange", "Banana", "Mango"]
root = merkle(txs)
print("\nFinal Merkle Root:")
print(root)

for s in ["Banana", "Grape"]:
    print(f"\n'{s}' in tree? {check(txs, s, root)}")