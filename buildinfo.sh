#!/bin/bash
#
# If not stated otherwise in this file or this component's license file the
# following copyright and licenses apply:
#
# Copyright 2026 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Emits build identification for libplayergstinterface, mirroring aamp/buildinfo.sh.
# Output lines are joined with '-' by CMake to form PLAYER_IF_BUILD_INFO.
#
# Example output:
#   GNU/Linux-x86_64-support/0.2.0_8.6-edb52e8-0.2.0-r3
#
uname -o
uname -m
git branch --show-current
git rev-parse --short HEAD
if git describe --tags --exact-match HEAD > /dev/null 2>&1; then
  git describe --tags --exact-match HEAD
else
  echo "No-Tag"
fi
