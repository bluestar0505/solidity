// SPDX-License-Identifier: UNLICENSED
pragma solidity >=0.8.0;

import './lib.sol';

/// My contract MyTestContract.
///
contract MyTestContract
{
    Weather lastWeather = Weather.Rainy;
    uint constant fixedValue = 1234;
    Color c = Color.Red;

    constructor()
    {
    }

    /// Sum is summing two args and returning the result
    ///
    /// @param a me it is
    /// @param b me it is also
    function sum(uint a, uint b) public pure returns (uint result)
    {
        Weather weather = Weather.Sunny;
        uint foo = 12345 + fixedValue;
        if (a == b)
            revert E(a, b);
        weather = Weather.Cloudy;
        result = a + b + foo;
    }

    function main() public pure returns (uint)
    {
        return sum(2, 3 - 123 + 456);
    }
}

contract D
{
    function main() public payable returns (uint)
    {
        MyTestContract c = new MyTestContract();
        return c.sum(2, 3);
    }
}

// TODO: Also add stubs for custom operators so we can test goto-definition with it.
