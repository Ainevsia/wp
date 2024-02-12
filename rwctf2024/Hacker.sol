// SPDX-License-Identifier: MIT

pragma solidity ^0.8.20;

contract FakeToken {
    address public l1Token;
    constructor(address _l1Token) {
        l1Token = _l1Token;
    }

    function mint(address _to, uint256 _amount) external {
        return;
    }

    function burn(address _from, uint256 _amount) external {
        return;
    }
}
