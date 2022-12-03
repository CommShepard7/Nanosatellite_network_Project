# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
    
    module = bld.create_ns3_module('dynamic-constellation', ['point-to-point', 'csma', 'internet', 'applications','mobility','propagation','wifi','aodv'])

    # Source files
    module.source = [
            'constellation/Constellation.cpp',
            'coordinates/Coordinates.cpp',
            'nanosatellite/NanoSatellite.cpp',
            'satellite-links/SatelliteLinks.cpp',
            'file-parser/FilePos.cpp',
            'network/NetworkController.cpp'
        ]

    # Header files
    headers = bld(features='ns3header')
    headers.module = 'dynamic-constellation'
    headers.source = [
            'constellation/Constellation.h',
            'coordinates/Coordinates.h',
            'nanosatellite/NanoSatellite.h',
            'satellite-links/SatelliteLinks.h',
            'file-parser/FilePos.h',
            'network/NetworkController.h'
        ]

    
    # Main
    bld.recurse('main')

    # For now, no Python bindings are generated
    # bld.ns3_python_bindings()
    