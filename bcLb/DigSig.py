import hashlib
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.exceptions import InvalidSignature


# 1. Generate RSA key pair
private_key = rsa.generate_private_key(public_exponent=65537, key_size=2048)
public_key = private_key.public_key()

# 2. Original message and its SHA-256 hash
message = b"Blockchain ensures data integrity"
print("Message:", message.decode())
print("SHA-256:", hashlib.sha256(message).hexdigest())

# 3. Sign the message using the private key
signature = private_key.sign(
    message,
    padding.PSS(mgf=padding.MGF1(hashes.SHA256()), salt_length=padding.PSS.MAX_LENGTH),
    hashes.SHA256()
)
print("\nDigital Signature Created.")

# 4. Verify the original message using the public key
try:
    public_key.verify(
        signature, message,
        padding.PSS(mgf=padding.MGF1(hashes.SHA256()),
                    salt_length=padding.PSS.MAX_LENGTH),
        hashes.SHA256()
    )
    print("Original Message: VALID")
except InvalidSignature:
    print("Original Message: INVALID")

# 5. Verify a tampered message
tampered_message = b"Blockchain ensures data security"

try:
    public_key.verify(
        signature, tampered_message,
        padding.PSS(mgf=padding.MGF1(hashes.SHA256()),
                    salt_length=padding.PSS.MAX_LENGTH),
        hashes.SHA256()
    )
    print("Tampered Message: VALID")
except InvalidSignature:
    print("Tampered Message: INVALID - Tampering Detected")