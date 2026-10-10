#!/usr/bin/env python3
#** *****************************************************************************
# *
# * If not stated otherwise in this file or this component's LICENSE file the
# * following copyright and licenses apply:
# *
# * Copyright 2023 RDK Management
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *
# http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *
#* ******************************************************************************

import sys
import os
import re
import time

sys.path.append(os.path.join(os.environ['root_dir'], 'utility_classes'))
from l3TestController import testController

class PlayerInterface(testController):
    def __init__(self, TestName="VideoPlaybackValidation"):
        super().__init__(TestName)

    def runTest(self):
        os.environ["DISPLAY"] = ":0"  # Ensure DISPLAY is set if needed for pi-cli
        mtf_root = os.environ['root_dir']
        video_url = "https://download.blender.org/peach/bigbuckbunny_movies/BigBuckBunny_320x180.mp4"
        video_uri = os.path.join(mtf_root, self.dut.rawConfig["dut"]["segment_dir"], "BigBuckBunny_320x180.mp4")
        video_path = os.path.dirname(video_uri)
        pi_cli = os.path.join(mtf_root, self.dut.rawConfig["dut"]["pi-cli"])
        
        self.testPrepareCommandList = [
            {"download" :video_url, "path" : f"{video_path}", "overwrite": True, "postdelay": 1000},
            {"cmd": pi_cli},
            {"expect": r"Please enter a command \(type 'help' for available commands\):"},
        ]

        self.testCommandList = [
            # create pipeline
            {"cmd": "createpipeline testPipeline 0"},
            {"expect": r"Creating gstreamer pipeline testPipeline priority 0"},
            {"expect": r"testPipeline buffering_enabled 0"},
            {"expect": r"CreatePipeline executed. Success: true"},

            # set player name
            {"cmd": "setplayername myPlayer"},
            {"expect": r"SetPlayerName executed."},

            # configure pipeline
            {"cmd": f"configurepipeline 0 0 0 0 false false true false 0 1 testPipeline 0 false {video_uri}"},
            {"expect": r"Gstreamer subs disabled"},
            {"expect": r"Rialto disabled"},
            {"expect": r"InterfacePlayerRDK: Attempting to set testPipeline state to READY"},
            {"expect": r"InterfacePlayerRDK: testPipeline state set to READY"},
            {"expect": r"ConfigurePipeline executed."},

            # playback rate
            {"cmd": "setplaybackrate 1.0"},
            {"expect": r"SetPlayBackRate executed\. Success: false"},

            # set audio volume
            {"cmd": "setaudiovolume 100"},
            {"expect": r"SetAudioVolume executed\."},

            # set video rectangle
            {"cmd": "setvideorectangle 0 0 1280 720"},
            {"expect": r"Rect 0,0,1280,720, video_sink =\(nil\)"},
            {"expect": r"New co-ordinates ignored since westerossink is used"},
            {"expect": r"SetVideoRectangle executed: \(0, 0, 1280, 720\)"},

            # setup stream
            {"cmd": f"setupstream 0 {video_uri}"},
            {"expect": r"Unknown event - No callback registered!"},
            {"expect": r"playbin flags1: 0x617"},
            {"expect": r"InterfacePlayerRDK: Connected deep-notify::source"},
            {"expect": r"Found source for video"},
            {"expect": r"InterfacePlayerRDK: Connected need-data"},
            {"expect": r"InterfacePlayerRDK: Connected enough-data"},
            {"expect": r"InterfacePlayerRDK: Connected seek-data"},
            {"expect": r"Unsupported format 0SetupStream executed\. Result: 0", "postdelay":10000},

            # inject fragment
            {"cmd": f"injectfragment 0 {video_uri} 1"},
            {"expect": "injectfragment executed. Success: true"},

            # get position milliseconds
            {"cmd": "getpositionmilliseconds"},
            {"expect": r"GetPositionMilliseconds: (\d+) ms"},
        ]

        self.testEndCommandList = [
           {"cmd" : 'exit', "postdelay" : 1000},
        ]

if __name__ == '__main__':
    TEST = PlayerInterface()
    TEST.run()