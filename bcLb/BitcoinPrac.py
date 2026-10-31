import hashlib
import base58
from ecdsa import SigningKey,SECP256k1

def sha(d): return hashlib.sha256(d).digest()
def cksum(d): return sha(sha(d))[:4]

pk=SigningKey.generate(curve=SECP256k1)
pubk=pk.get_verifying_key().to_string()
pay=b'\x00'+hashlib.new("ripemd160",sha(pubk)).digest()
full=pay+cksum(pay)
addr=base58.b58encode(full).decode()

def verify(a):
    d=base58.b58decode(a) 
    return len(d)==25 and cksum(d[:-4])==d[-4:]

print("Private Key:", pk.to_string().hex())
print("Public Key:", pubk.hex())
print(len(pubk.hex()))
print("\nBitcoin Address:", addr)
print("Address Status : ", verify(addr))   
addr = addr[:2] + 'a' + addr[3:]
print("Modified Address     :", addr)
print("Modified Address Status : ", verify(addr))