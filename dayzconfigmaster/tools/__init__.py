# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
DayZ Tools wrappers.

Thin subprocess wrappers around DayZ Tools executables:
- binarize.exe: Convert models to binary format
- cfgconvert.exe: Convert config.bin <-> config.cpp
- imagetopaa.exe: Convert PNG/TGA to PAA texture format
"""

from .binarize import (
    find_binarize,
    make_args,
    run as binarize_run,
    binarize_folder
)

from .cfgconvert import (
    find_cfgconvert,
    unbinarize_args,
    to_bin_args,
    run as cfgconvert_run,
    unbinarize,
    to_bin,
    convert_to_cpp,
    convert_to_bin
)

from .imagetopaa import (
    find_imagetopaa,
    has_valid_suffix,
    convert_args,
    plan_folder,
    run as imagetopaa_run,
    convert_folder,
    convert_image,
    check_suffix
)

__all__ = [
    # binarize
    'find_binarize',
    'make_args',
    'binarize_run',
    'binarize_folder',
    # cfgconvert
    'find_cfgconvert',
    'unbinarize_args',
    'to_bin_args',
    'cfgconvert_run',
    'unbinarize',
    'to_bin',
    'convert_to_cpp',
    'convert_to_bin',
    # imagetopaa
    'find_imagetopaa',
    'has_valid_suffix',
    'convert_args',
    'plan_folder',
    'imagetopaa_run',
    'convert_folder',
    'convert_image',
    'check_suffix',
]
