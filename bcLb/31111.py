# 3.11.11 Program for Bitcoin Mining

import hashlib
import time


class Block:
    def __init__(self, transactions, previous_hash):
        self.time = time.time()
        self.transactions = transactions
        self.previous_hash = previous_hash
        self.nonce = 0
        self.hash = self.get_hash()

    def get_hash(self): 
        data = str(self.time) + str(self.transactions) + str(self.previous_hash) + str(self.nonce)
        return hashlib.sha256(data.encode()).hexdigest()

    def mine(self, difficulty):
        print("Mining block...")
        start = time.time()

        while self.hash[:difficulty] != "0" * difficulty:
            self.nonce += 1
            self.hash = self.get_hash()

        end = time.time()

        print("Block Mined")
        print("Hash     :", self.hash)
        print("Nonce    :", self.nonce)
        print("Time     :", end - start, "seconds")

print("\nMining Block 0 (Genesis Block)")

tx = [
    "Alice pays Bob 5 BTC",
    "Bob pays Charlie 3 BTC",
    "Charlie pays David 2 BTC"
] 

difficulty = 3 
block = Block(tx, "0" * 64) 
block.mine(difficulty)


# print("\nMining Block 1")

# tx = [
#     "Alice pays Bob 10 BTC",
#     "Eve pays Charlie 50 BTC",
#     "David pays Eve 20 BTC"
# ]

# block = Block(tx, block.hash)
# block.mine(difficulty)


# print("\nMining Block 2")

# tx = [
#     "Alice pays Bob 15 BTC",
#     "Bob pays Charlie 7 BTC",
#     "Charlie pays David 3 BTC"
# ]

# block = Block(tx, block.hash)
# block.mine(difficulty)
# [
#     "Alice pays Bob 20 BTC",
#     "Bob pays Charlie 10 BTC"
# ]

# block = Block(tx, block.hash)
# block.mine(difficulty)


# print("\nMining Block 2")

# tx = [
#     "Alice pays Bob 25 BTC",
#     "Bob pays Charlie 15 BTC",
#     "Charlie pays David 5 BTC"
# ]

# block = Block(tx, block.hash)
# block.mine(difficulty)