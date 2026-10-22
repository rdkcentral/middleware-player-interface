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

sys.path.append(os.path.join(os.environ['root_dir'],'utility_classes'))
from l3TestController import testController

class PlaybackMPD(testController):
    markers = []

    def __init__(self, TestName="MPD Playback"):
        super().__init__(TestName, maxRunTime=8)
    

    def runTest(self):
        url = "https://cpetestutility.stb.r53.xcal.tv/VideoTestStream/public/aamptest/streams/L2/Shortstream_video.tar.xz"
        mtf_root = os.environ['root_dir']
        pi_cli = os.path.join(mtf_root, self.dut.rawConfig["dut"]["pi-cli"])
        segment_dir = os.path.join(mtf_root, self.dut.rawConfig["dut"]["segment_dir"])

        self.testPrepareCommandList = [
            # Download Canned Dash Stream
            {"download" :url, "path" : f"{segment_dir}/", "overwrite": True, "postdelay": 1000},
            # Start pi-cli
            {"cmd" : pi_cli, "postdelay": 1000},
        ]
        self.testCommandList = [
            # create pipeline
            {"cmd" : "createpipeline testpipe -1"},
            {"expect" : "CreatePipeline executed"},

            # set player name
            {"cmd" : "setplayername testplayer"},
            {"expect" : "SetPlayerName executed."},

            # set audio volume
            {"cmd" : "setaudiovolume 100"},
            {"cmd" : "setvideorectangle 0 0 1280 720"},

            # configure pipeline
            {"cmd" : f"configurepipeline 0 0 0 0 false false false false 0 1 testpipe 0 false {segment_dir}/VideoTestStream/main.mpd"},
            {"expect" : "ConfigurePipeline executed"},

            # setup stream
            {"cmd" : f"setupstream 0 {segment_dir}/VideoTestStream/main.mpd"},
            {"expect" : "InterfacePlayerRDK: Connected deep-notify::source"},
            {"expect" : "Found source for video"},
            {"expect" : r"InterfacePlayerRDK: Connected (need)-data"},
            {"expect" : r"InterfacePlayerRDK: Connected (need|enough)-data"},
            {"expect" : "InterfacePlayerRDK: Connected seek-data"},

            {"cmd" : f"setupstream 1 {segment_dir}/VideoTestStream/main.mpd"},
            {"expect" : "InterfacePlayerRDK: Connected deep-notify::source"},

            # inject header segments
            {"cmd" : f"injectfragment 0 {segment_dir}/VideoTestStream/dash/1080p_init.m4s 1"},
            {"expect" : "injectfragment executed. Success: true"},
            {"cmd" : f"injectfragment 1 {segment_dir}/VideoTestStream/dash/en_init.m4s 1"},
            {"expect" : "injectfragment executed. Success: true"},

            # start reporting playback position every 1 second
            {"cmd" : f"reportpositions 1000"},
            {"expect" : "Started position polling every 1000 ms."},

            # inject media segments
            {"cmd" : f"injectfragment 0 {segment_dir}/VideoTestStream/dash/1080p_001.m4s 0"},
            {"expect" : "injectfragment executed. Success: true"},
            {"cmd" : f"injectfragment 1 {segment_dir}/VideoTestStream/dash/en_001.mp3 0"},
            {"expect" : "injectfragment executed. Success: true"},
            
            {"cmd" : f"injectfragment 0 {segment_dir}/VideoTestStream/dash/1080p_002.m4s 0"},
            {"expect" : "injectfragment executed. Success: true"},
            {"cmd" : f"injectfragment 1 {segment_dir}/VideoTestStream/dash/en_002.mp3 0"},
            {"expect" : "injectfragment executed. Success: true"},

            {"cmd" : f"injectfragment 0 {segment_dir}/VideoTestStream/dash/1080p_003.m4s 0"},
            {"expect" : "injectfragment executed. Success: true"},
            {"cmd" : f"injectfragment 1 {segment_dir}/VideoTestStream/dash/en_003.mp3 0"},
            {"expect" : "injectfragment executed. Success: true"},

            # check current position
            {"cmd" : "getpositionmilliseconds"},
            {"expect" : r"GetPositionMilliseconds: (\d+) ms"},

            # add more media segments
            {"cmd" : f"injectfragment 0 {segment_dir}/VideoTestStream/dash/1080p_004.m4s 0"},
            {"expect" : "injectfragment executed. Success: true"},
            {"cmd" : f"injectfragment 1 {segment_dir}/VideoTestStream/dash/en_004.mp3 0"},
            {"expect" : "injectfragment executed. Success: true"},

            {"cmd" : f"injectfragment 0 {segment_dir}/VideoTestStream/dash/1080p_005.m4s 0"},
            {"expect" : "injectfragment executed. Success: true"},
            {"cmd" : f"injectfragment 1 {segment_dir}/VideoTestStream/dash/en_005.mp3 0"},
            {"expect" : "injectfragment executed. Success: true"},

            # check playback position starting from 2 seconds upto 5 seconds confirming playback progress
            {"expect" : r"GetPositionMilliseconds: ([2-9](\d{3}))"},
            {"expect" : r"GetPositionMilliseconds: ([3-9](\d{3}))"},
            {"expect" : r"GetPositionMilliseconds: ([4-9](\d{3}))"},
            {"expect" : r"GetPositionMilliseconds: ([5-9](\d{3}))"},

        ]

        self.testEndCommandList = [
            # stop pi-cli and wait for 1 second
            {"cmd" : 'exit', "postdelay" : 1000},
        ]

    

# This is what the script will run when executed
if __name__ == '__main__':
    TEST = PlaybackMPD()
    TEST.run()
