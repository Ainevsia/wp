// SPDX-License-Identifier: MIT
pragma solidity ^0.8.20;
interface IL2ERC20Bridge {
    event WithdrawalInitiated(
        address indexed _l1Token, address indexed _l2Token, address indexed _from, address _to, uint256 _amount
    );

    event DepositFinalized(
        address indexed _l1Token, address indexed _l2Token, address indexed _from, address _to, uint256 _amount
    );

    event DepositFailed(
        address indexed _l1Token, address indexed _l2Token, address indexed _from, address _to, uint256 _amount
    );

    /**
     * @dev get the address of the corresponding L1 bridge contract.
     * @return Address of the corresponding L1 bridge contract.
     */
    function l1TokenBridge() external returns (address);

    /**
     * @dev initiate a withdraw of some tokens to the caller's account on L1
     * @param _l2Token Address of L2 token where withdrawal was initiated.
     * @param _amount Amount of the token to withdraw.
     */
    function withdraw(address _l2Token, uint256 _amount) external;

    /**
     * @dev initiate a withdraw of some token to a recipient's account on L1.
     * @param _l2Token Address of L2 token where withdrawal is initiated.
     * @param _to L1 adress to credit the withdrawal to.
     * @param _amount Amount of the token to withdraw.
     */
    function withdrawTo(address _l2Token, address _to, uint256 _amount) external;

    /**
     * @dev Complete a deposit from L1 to L2, and credits funds to the recipient's balance of this
     * L2 token. This call will fail if it did not originate from a corresponding deposit in
     * L1StandardTokenBridge.
     * @param _l1Token Address for the l1 token this is called with
     * @param _l2Token Address for the l2 token this is called with
     * @param _from Account to pull the deposit from on L2.
     * @param _to Address to receive the withdrawal at
     * @param _amount Amount of the token to withdraw
     */
    function finalizeDeposit(address _l1Token, address _l2Token, address _from, address _to, uint256 _amount)
        external;
}