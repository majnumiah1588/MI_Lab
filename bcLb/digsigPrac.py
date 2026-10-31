import hashlib 
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import rsa,padding
from cryptography.exceptions import InvalidSignature

priK=rsa.generate_private_key(public_exponent=65537,key_size=2048)
pubK=priK.public_key()
msgS=b"Alice send 10 BTC to Bob"
print("msg sent: ", msgS)
sig=priK.sign( msgS, padding.PSS(mgf=padding.MGF1(hashes.SHA256()), salt_length=padding.PSS.MAX_LENGTH), hashes.SHA256() )
try: 
    pubK.verify( sig,msgS, padding.PSS(mgf=padding.MGF1(hashes.SHA256()), salt_length=padding.PSS.MAX_LENGTH), hashes.SHA256())
    print("msg is valid.")
except InvalidSignature:
    print("Original msg : ",msgS)
    print("msg is corrupted!")
msgR=b"Alice send 100 BTC to Bob"
print("Received msg : ", msgR)
try:
    pubK.verify( sig,msgR, padding.PSS(mgf=padding.MGF1(hashes.SHA256()), salt_length=padding.PSS.MAX_LENGTH), hashes.SHA256())
    print("msg is valid.")
except InvalidSignature:
    print("Original msg : ",msgS)
    print("Corrupted msg : ", msgR)
    print("msg is corrupted!")  
