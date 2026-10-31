import hashlib
import time

class Block:
    def __init__(self,tx,prevH):
        self.tx=tx
        self.nonce=0
        self.time=time.time()
        self.prevH=prevH
        self.hash=self.getHash()
    def getHash(self):
       h=str(self.time)+str(self.nonce)+str(self.prevH)+str(self.tx)
       return hashlib.sha256(h.encode()).hexdigest()
    def mine(self,diff):
        print("Start Mining block...")
        start=time.time()
        while self.hash[:diff] != "0"*diff :
           self.nonce+=1
           self.hash = self.getHash()
        end=time.time()
        print("Time : ",end-start)
        print("difficulty : ",diff)
        print("Nonce : ",self.nonce)
        print("Hash : ",self.hash,"\n")
        
print("Start Creating Genesis block 0 ...")
diff=3
tx=["Alice send 10 BTC to Bob","Alice send 10 BTC to Charlie","Bob send 10 BTC to Kevin"]
block=Block(tx,"0"*64)
block.mine(diff)
print("Start Creating block 1 ...")
diff=4
tx=["Alice send 10000 BTC to Bob","Alice send 10000 BTC to Charlie","Bob send 10000 BTC to Kevin"]
block2=Block(tx,block.hash)
block2.mine(diff)

print("Start Creating block 2 ...")
diff=5
tx=["Alice send 10000 BTC to Bob","Alice send 10000 BTC to Charlie","Bob send 10000 BTC to Kevin"]
block2=Block(tx,block.hash)
block2.mine(diff)