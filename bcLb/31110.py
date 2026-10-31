# 3.11.10 Program to Create a Bitcoin Transaction and Sign It with senders private key
# Using the bitcoinlib Library

from bitcoinlib.transactions import Transaction
from bitcoinlib.keys import Key

print("Bitcoin UTXO Transaction Signing\n")

key= Key()

# keyand receiver addresses
address = key.address()
print(len(address))
receiver_address = "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa"
print(len(receiver_address))

# Existing UTXO being spent
previous_transaction_id = "1" * 64
previous_output_index = 0
previous_utxo_value = 150000

# Transaction amounts
receiver_amount = 100000
mining_fee = 1000

# Calculate sender's change
sender_change = (
    previous_utxo_value
    - receiver_amount
    - mining_fee
)

if sender_change < 0:
    raise ValueError("Insufficient UTXO value")

# Create transaction
tx = Transaction()

# Add UTXO as transaction input
tx.add_input(
    prev_txid="1" * 64,
    output_n=0
)

# Add receiver output
tx.add_output(
    address=receiver_address,
    value=receiver_amount
)

# Add change output
if sender_change >= 0:
    tx.add_output(
        address=address,
        value=sender_change
    )

# Sign transaction with sender's private key
tx.sign(key)
tx.add_output(address=receiver_address,value=10000)
print("verify : " ,tx.verify())


# Display transaction information
print("Transaction Details")
print("keyAddress       :", address)
print("Receiver Address     :", receiver_address)

print("\nInput UTXO")
print("Previous TXID        :", previous_transaction_id)
print("Previous Output Index:", previous_output_index)
print("UTXO Value           :", previous_utxo_value, "satoshi")

print("\nTransaction Outputs")
print("Receiver Amount      :", receiver_amount, "satoshi")
print("Change Amount        :", sender_change, "satoshi")
print("Mining Fee           :", mining_fee, "satoshi")

print("\nValue Verification")
print("Input Value          :", previous_utxo_value, "satoshi")
print("Output Value         :", receiver_amount + sender_change, "satoshi")
print("Mining Fee           :", mining_fee, "satoshi")

print("\nSigned Transaction Hex")
print(tx.raw_hex())