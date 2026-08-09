# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Unit tests for the DayZ XML serialization helpers."""

from dayzconfigmaster.economy.ce_xml import CeXml


def test_serialize_preserves_standalone_and_formatting():
    """Regression: CeXml must produce DayZ-style output, not broken multi-line tags."""
    xml_in = (
        '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n'
        "<types>\n"
        '    <type name="ACOGOptic">\n'
        "        <nominal>9</nominal>\n"
        "        <lifetime>14400</lifetime>\n"
        "        <category name=\"weapons\"/>\n"
        "    </type>\n"
        "</types>\n"
    )

    doc = CeXml.parse_doc(xml_in)
    out = CeXml.serialize(doc)

    assert '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>' in out
    assert "<nominal>9</nominal>" in out
    assert "<lifetime>14400</lifetime>" in out
    assert '<category name="weapons"/>' in out
    # The broken formatting this test guards against.
    assert "<nominal>\n9\n" not in out
