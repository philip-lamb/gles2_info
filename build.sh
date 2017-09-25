#! /bin/bash
#
#  build.sh
#
#  Build script for minimal code to dump EGL and GLESv2 info for
#  Raspberry Pi.
#
#  Copyright 2017 Philip Lamb. All Rights Reserved.
#
#  Author(s): Philip Lamb
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
# 
#      http:#www.apache.org/licenses/LICENSE-2.0
# 
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

gcc -DRASPBERRY_PI -I/opt/vc/include -L/opt/vc/lib -lbrcmGLESv2 -lbrcmEGL -lbcm_host -o gles2_info gles2_info.c
