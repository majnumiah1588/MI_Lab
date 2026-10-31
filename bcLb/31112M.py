import hashlib

class Block:
    def __init__(self, dat, prev): self.dat, self.prev, self.hsh = dat, prev, self.calc()
    def calc(self): return hashlib.sha256((self.dat + self.prev).encode()).hexdigest()

class Blockchain:
    def __init__(self): self.chain = [Block("Genesis Block", "0")]
    def add(self, dat): self.chain.append(Block(dat, self.chain[-1].hsh))

    def validate(self):
        for i in range(1, len(self.chain)):
            cur, prv = self.chain[i], self.chain[i - 1]
            if cur.hsh != cur.calc(): print("Invalid hash for block", i); return False
            if cur.prev != prv.hsh:   print("Invalid previous hash for block", i); return False
        print("Blockchain is valid"); return True

    def hashrate(self): return sum(int(b.hsh, 16) for b in self.chain)

    def check51(self):
        tot = self.hashrate()
        for i, blk in enumerate(self.chain):
            if int(blk.hsh, 16) / tot > 0.50: print("51% attack detected at block", i); return True
        print("No 51% attack detected"); return False

print("Scenario 1: NO 51% ATTACK")
bc = Blockchain()
bc.add("Alice pays Bob 5 BTC")
bc.add("Bob pays Charlie 3 BTC")
bc.add("Charlie pays David 2 BTC")
bc.validate(); bc.check51()

print("\nScenario 2: 51% ATTACK")
bc2 = Blockchain()
bc2.add("Alice pays Bob 1000000000000000000000000000099999999999999915 BTC ")
bc2.add("Bob pays Charlie 1050009 BTC")
bc2.add("Charlie pays David 1000099 BTC")
bc2.check51()
bc2.chain[1].hsh = "f" * 64
bc2.validate()