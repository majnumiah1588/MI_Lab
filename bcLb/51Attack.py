import hashlib

class Block:
    def __init__(self, data, previous_hash):
        self.data = data
        self.previous_hash = previous_hash
        self.hash = self.calculate_hash()

    def calculate_hash(self):
        return hashlib.sha256(
            (self.data + self.previous_hash).encode()
        ).hexdigest()


class Blockchain:
    def __init__(self):
        self.chain = [Block("Genesis Block", "0")]

    def add_block(self, data):
        self.chain.append(Block(data, self.chain[-1].hash))

    def validate_chain(self):
        for i in range(1, len(self.chain)):
            current = self.chain[i]
            previous = self.chain[i - 1]

            if current.hash != current.calculate_hash():
                print("Invalid hash at block", i)
                return False

            if current.previous_hash != previous.hash:
                print("Invalid previous hash at block", i)
                return False

        print("Blockchain is valid")
        return True

    def check_51_attack(self):
        total = sum(int(block.hash, 16) for block in self.chain)

        for i, block in enumerate(self.chain):
            if int(block.hash, 16) / total > 0.50:
                print("51% Attack Detected at block", i)
                return True

        print("No 51% Attack Detected")
        return False


# Scenario 1: Normal blockchain
print("Scenario 1: Normal Blockchain")
bc = Blockchain()
bc.add_block("Alice pays Bob 5 BTC")
bc.add_block("Bob pays Charlie 3 BTC")
bc.add_block("Charlie pays David 2 BTC")
bc.validate_chain()
bc.check_51_attack()

# Scenario 2: Modified blockchain
print("\nScenario 2: Modified Blockchain")
bc2 = Blockchain()
bc2.add_block("Alice pays Bob 1000000000000000000000000000099999999999999915 BTC")
bc2.add_block("Bob pays Charlie 1050009 BTC")
bc2.add_block("Charlie pays David 1000099 BTC")
bc2.check_51_attack()

bc2.chain[1].hash = "f" * 64
bc2.validate_chain()