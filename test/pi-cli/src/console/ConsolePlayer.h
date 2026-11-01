/*
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

class PlayerBackend;

class ConsolePlayer {
public:
    int Run(PlayerBackend& backend);
};
