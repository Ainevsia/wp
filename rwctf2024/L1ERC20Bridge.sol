// SPDX-License-Identifier: MIT
pragma solidity ^0.8.20;

interface L1ERC20Bridge {
    function depositERC20(address _l1Token, address _l2Token, uint256 _amount) external;
}