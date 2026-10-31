from bitcoinlib.transactions import Transaction
from bitcoinlib.keys import Key

sk,rk=Key(),Key() #sender , receiver private key generate
sa,ra=sk.address(),rk.address() # sender , receiver address generate
tx=Transaction() 
amount=1000

tx.add_input( prev_txid="1"*64, output_n=0)   #keys=sk.public_hex)
tx.add_input( prev_txid="2"*64, output_n=1)   #keys=sk.public_hex)


tx.add_output( address=ra, value=amount)
tx.sign(sk)
print("Transaction validity status : ",tx.verify())
print("sender : ",sa)
print("receiver : ",ra)
print("Amount sent: ",amount)
print("Amount received:", tx.outputs[0].value)
print("Tx hash : ",tx.raw_hex(),"\n")

#debugging
tx.sign(sk)
tx.add_output( address=ra, value=amount+100000)
print("Transaction validity status : ", tx.verify())
print("Transaction id : ",tx.inputs[0].prev_txid)
print("Transaction output_n : ",tx.inputs[0].output_n)
print("Transaction id : ",tx.inputs[1].prev_txid)
print("Transaction output_n : ",tx.inputs[1].output_n)

print("sender : ",sa)
print("receiver : ",ra)
print("Amount sent: ",amount)
print("Amount received:", tx.outputs[1].value)
print("Tx hash : ",tx.raw_hex())