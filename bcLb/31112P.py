import hashlib

class Block:
    def __init__ (self,data,prevHash):
        self.data=data
        self.prevHash=prevHash
        self.hash=self.getHash()
    def getHash(self):
        data=str(self.data)+str(self.prevHash)
        return hashlib.sha256(data.encode()).hexdigest()
    
class Blockchain:
    def __init__(self):
        self.chain=[Block("Genesis block 0","0")]
    def add_block(self,data):
        self.chain.append(Block(data,self.chain[-1].hash))
    def chainValidate(self):
        for i in range (1,len(self.chain)):
            current=self.chain[i]
            prev=self.chain[i-1]
            if current.hash != current.getHash():
                print("Invalid hash of block : ",i)
                return False
            if current.prevHash != prev.hash :
                print("Invalid PrevHash of block : ",i)
                return False
        print("Block is valid...!")
        return True
    def getChainLen(self):
        return (len(self.chain))
    def getChainHashRate(self):
        return sum(int(block.hash,16) for block in self.chain)
    def check_51(self):
        totalHashRate=self.getChainHashRate()
        for i,block in enumerate(self.chain):
            blockHashRate=(int(block.hash,16))
            if(blockHashRate/totalHashRate > 0.50):
                print("51% attack detected at block : ",i)
                print(self.chain[i].data)
            return True
blockchain = Blockchain()
blockchain.add_block("Alice send to Bob 900000000000 BTC")
blockchain.add_block("Bob send to Charlie 500000000088888888888888888888000000888 BTC")
print(blockchain.getChainLen())

blockchain.chainValidate()
blockchain.check_51()

blockchain.chain[1].hash="f"*64
blockchain.chainValidate()
