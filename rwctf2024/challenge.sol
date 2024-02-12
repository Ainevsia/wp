pragma solidity ^0.8.20;

contract Challenge {
    address public immutable BRIDGE;
    address public immutable MESSENGER;
    address public immutable WETH;

    constructor(address bridge, address messenger, address weth) {
        BRIDGE = bridge;
        MESSENGER = messenger;
        WETH = weth;
    }

    function isSolved() external view returns (bool) {
        return true;
    }
}
