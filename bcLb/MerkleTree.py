import hashlib

# Calculate SHA-256 hash
def calculate_hash(data):
    return hashlib.sha256(data.encode()).hexdigest()

# Build Merkle Tree
def build_merkle_tree(transactions):
    hashes = [calculate_hash(tx) for tx in transactions]

    while len(hashes) > 1:
        if len(hashes) % 2 != 0:
            hashes.append(hashes[-1])
        hashes = [calculate_hash(hashes[i] + hashes[i + 1]) for i in range(0, len(hashes), 2)]
    return hashes[0]

# Main program
transactions = ["Apple", "Orange", "Banana", "Mango"]

merkle_root = build_merkle_tree(transactions)

print("Transactions:", transactions)
print("Merkle Root:", merkle_root)



# import hashlib

# def sha256(data):
#     return hashlib.sha256(data.encode()).hexdigest()

# # Transactions
# transactions = ["Apple", "Orange", "Banana", "Mango"]

# # Level 0: Leaf hashes
# A, O, B, M = [sha256(tx) for tx in transactions]

# # Level 1: Parent hashes
# H01 = sha256(A + O)
# H23 = sha256(B + M)

# # Level 2: Merkle Root
# root = sha256(H01 + H23)

# # Print Merkle Tree
# print("\n             Merkle Root")
# print("                  |")
# print("          +-------+-------+")
# print("          |               |")
# print("       Hash 01          Hash 23")
# print("       /    \\           /    \\")
# print("   Hash A   Hash O   Hash B   Hash M")
# print("      |        |        |        |")
# print("    Apple    Orange   Banana   Mango")

# print("\nMerkle Root Hash:", root)