# 3.11.10 Program to Create a Bitcoin Transaction and Sign It with senders private key
# Using the bitcoinlib Library

from bitcoinlib.transactions import Transaction
from bitcoinlib.keys import Key


print("Bitcoin transaction signing: \n")

tx = Transaction()

sender = "Sender Generated Private Key"
receiver = "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa"
amount = 100000

tx.add_input(
    prev_txid="1" * 64,
    output_n=0
)

tx.add_output(
    address=receiver,
    value=amount
)

key = Key()

tx.sign(key)
# tx.add_output(
#     address=receiver,
#     value=amount+1000
# )
print("verify : " ,tx.verify())

print("\nTransaction Details")
print("Sender :", sender)
print("Receiver :", receiver)
print("Amount :", amount, "satoshi")

print("\nSigned Transaction Hex")
print(tx.raw_hex())


# from bitcoinlib.transactions import Transaction
# from bitcoinlib.keys import Key

# sk, rk = Key(), Key()

# tx = Transaction()
# tx.add_input(prev_txid="1"*64, output_n=0, keys=sk.public_hex)
# tx.add_output(address=rk.address(), value=1000)
# tx.sign(sk)

# print(tx.verify())
# print(sk.address())
# print(rk.address())
# print(1000)
# print(tx.raw_hex())