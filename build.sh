#!/bin/sh
# SPDX-License-Identifier: BSD-2-Clause

cc -static $(find . -name "*.c") -o prootbox
