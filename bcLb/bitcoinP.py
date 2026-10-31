import hashlib
import base58
from ecdsa import SigningKey, SECP256k1

def sha256(d): return hashlib.sha256(d).digest()
def checksum(d): return sha256(sha256(d))[:4]

private_key = SigningKey.generate(curve=SECP256k1)
public_key = private_key.get_verifying_key().to_string()
private_key = private_key.to_string()

ripemd160 = hashlib.new("ripemd160", sha256(public_key)).digest()

pay = b'\x00' + ripemd160
ck = checksum(pay)
full = pay + ck
addr = base58.b58encode(full).decode()

def verify(a):
    try:
        d = base58.b58decode(a)
        return len(d) == 25 and checksum(d[:-4]) == d[-4:]
    except Exception:  return False

print("Private Key:", private_key.hex())
print("Public Key:", public_key.hex())
print(len(public_key.hex()))
print("\nBitcoin Address:", addr)
print("Address Status : ", verify(addr))   
addr = addr[:2] + 'a' + addr[3:]
print("Modified Address     :", addr)
print("Modified Address Status : ", verify(addr))