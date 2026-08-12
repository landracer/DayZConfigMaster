# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""XML validation and repair helpers for DayZ mod fragments.

DayZ mods frequently ship XML that is *almost* valid but misses a root
element, contains multiple top-level elements, has CRLF line endings that
break attribute names across lines, or uses invalid comments such as
``<!-- ------MUMMY------ -->``.

This module detects those problems and attempts a conservative repair.
Fragments that cannot be repaired are rejected with a clear message so they
do not silently corrupt the final mission XML.
"""

from __future__ import annotations

import re
from dataclasses import dataclass, field
from typing import Optional, Tuple
from xml.etree import ElementTree as ET


@dataclass
class XmlRepairResult:
    """Outcome of validating/repairing a mod XML fragment."""

    ok: bool
    text: str
    message: str
    error: str = ""
    was_repaired: bool = False


# Mapping from target mission path to the expected root element tag.  This is
# used when a fragment has to be wrapped because it has no root or the root
# does not match the file it is being merged into.
# NOTE: these tags match the DayZ *root* mission files, not the CE-routed
# variants inside the economy PBO.
TARGET_TO_ROOT_TAG: dict[str, str] = {
    "db/types.xml": "types",
    "cfgspawnabletypes.xml": "spawnabletypes",
    "db/events.xml": "events",
    "cfgeventspawns.xml": "eventposdef",
    "db/economy.xml": "economy",
    "cfglimitsdefinition.xml": "lists",
    "cfglimitsdefinitionuser.xml": "user_lists",
    "cfgrandompresets.xml": "randompresets",
    "cfgenvironment.xml": "env",
    "mapgroupproto.xml": "prototype",
    "mapgrouppos.xml": "map",
    "mapgroupcluster.xml": "map",
    "mapgroupdirt.xml": "map",
    "mapclusterproto.xml": "prototype",
    "cfgplayerspawnpoints.xml": "playerspawnpoints",
}

# Expected child tag(s) for each root tag.  Used by final validation.
# Only well-known CE files are checked strictly; other files skip child-tag
# validation so that unusual but valid mod layouts are not rejected.
# A set means any of those tags is accepted; ``None`` skips child validation.
ROOT_TO_CHILD_TAG: dict[str, Optional[object]] = {
    "types": "type",
    "spawnabletypes": {"type", "damage", "cargo", "attachments", "preset", "hoarder", "item"},
    "events": "event",
    "eventposdef": "event",
}


# ------------------------------------------------------------------
# Low-level sanitizers
# ------------------------------------------------------------------
def _remove_invalid_control_chars(text: str) -> str:
    """Strip control characters that are illegal in XML 1.0.

    Tab, newline and carriage return are allowed; everything else below
    0x20 is removed.
    """
    return "".join(ch for ch in text if ch in "\t\n\r" or ord(ch) >= 0x20)


def _strip_xml_declaration(text: str) -> str:
    """Remove the ``<?xml ... ?>`` declaration from a fragment."""
    return re.sub(r"<\?xml.*?\?>", "", text, flags=re.DOTALL).strip()


def _remove_newlines_inside_tags(text: str) -> str:
    """Remove line breaks that occur inside element start/end tags.

    DayZ mod XML sometimes has CRLF line endings that split attribute names
    across lines (e.g. ``count_in\n_player="0"``).  Removing newlines
    inside ``<...>`` joins those fragments back together.
    """

    def _clean_tag(match: "re.Match[str]") -> str:
        body = match.group(1)
        # Remove literal newlines/carriage returns inside the tag, preserving
        # normal spaces/tabs that separate attributes.
        body = body.replace("\r", "").replace("\n", "")
        return f"<{body}>"

    # Match a start tag, including self-closing, across multiple lines.
    return re.sub(r"<([^>]+)>", _clean_tag, text, flags=re.DOTALL)


def sanitize_xml_comments(text: str) -> str:
    """Remove invalid double-dashes inside XML comments.

    DayZ mod fragments sometimes contain decorative comments like
    ``<!-- ------MUMMY------ -->`` which are not valid XML.
    """

    def _clean(match: "re.Match[str]") -> str:
        body = match.group(1)
        if "--" in body:
            clean = re.sub(r"-{2,}", "-", body)
            return "<!-- " + clean + " -->"
        return match.group(0)

    return re.sub(r"<!--(.*?)-->(?=\s*<|\s*$)", _clean, text, flags=re.DOTALL)


# ------------------------------------------------------------------
# Parsing helpers
# ------------------------------------------------------------------
def _try_parse(text: str) -> Tuple[bool, str]:
    """Return (ok, error_message) for an XML string."""
    try:
        ET.fromstring(text)
        return True, ""
    except ET.ParseError as exc:
        return False, str(exc)


def _root_tag(text: str) -> Optional[str]:
    """Return the tag of the first element, without fully parsing."""
    cleaned = re.sub(r"<\?xml.*?\?>", "", text, flags=re.DOTALL)
    cleaned = re.sub(r"<!--.*?-->", "", cleaned, flags=re.DOTALL)
    match = re.search(r"<([a-zA-Z_][\w.-]*)", cleaned)
    return match.group(1) if match else None


def _serialize_children(root: ET.Element) -> str:
    """Return the concatenated XML of an element's children."""
    return "".join(ET.tostring(child, encoding="unicode") for child in root)


def _extract_inner_xml(text: str, root_tag: str) -> str:
    """Return the content between the root element's start and end tags."""
    pattern = re.compile(
        rf"<{re.escape(root_tag)}[^>]*>(.*?)</{re.escape(root_tag)}>",
        re.DOTALL | re.IGNORECASE,
    )
    match = pattern.search(text)
    return match.group(1).strip() if match else ""


# ------------------------------------------------------------------
# Public repair API
# ------------------------------------------------------------------
def repair_mod_xml(text: str, expected_root: str) -> XmlRepairResult:
    """Validate and, if necessary, repair a mod XML fragment.

    Args:
        text: Raw fragment content.
        expected_root: The root element that this fragment should have once
            merged (e.g. ``types``, ``spawnabletypes``).

    Returns:
        :class:`XmlRepairResult`.  When ``ok`` is True, ``text`` contains the
        repaired XML.  When ``ok`` is False, ``text`` is the original input
        and ``error`` explains why repair failed.
    """
    original = text
    messages: list[str] = []

    # 1. Strip BOM.
    if text.startswith("\ufeff"):
        text = text[1:]
        messages.append("removed BOM")

    # 2. Remove invalid control characters.
    cleaned = _remove_invalid_control_chars(text)
    if cleaned != text:
        text = cleaned
        messages.append("removed invalid control characters")

    # 3. Normalize CRLF and remove line breaks inside tags.
    text = text.replace("\r\n", "\n").replace("\r", "\n")
    cleaned = _remove_newlines_inside_tags(text)
    if cleaned != text:
        text = cleaned
        messages.append("joined broken attributes across line endings")

    # 4. Sanitize comments.
    cleaned = sanitize_xml_comments(text)
    if cleaned != text:
        text = cleaned
        messages.append("sanitized invalid XML comments")

    # 5. Try to parse as-is.
    ok, err = _try_parse(text)
    if ok:
        root = ET.fromstring(text)
        root_tag = root.tag.lower()
        expected_lower = expected_root.lower()
        if root_tag == expected_lower:
            msg = _join_messages(messages)
            return XmlRepairResult(
                ok=True,
                text=text,
                message=msg,
                was_repaired=bool(messages),
            )
        # Parseable but root tag does not match the target.  If the original
        # root is the expected child element (e.g. a lone <type> block that
        # should live under <types>), wrap the whole element.  Otherwise
        # rewrap the children.
        expected_child = ROOT_TO_CHILD_TAG.get(expected_root.lower())
        allowed_children = (
            expected_child
            if isinstance(expected_child, set)
            else {expected_child}
            if expected_child
            else set()
        )
        if root_tag in allowed_children:
            text = (
                f"<{expected_root}>"
                f"{ET.tostring(root, encoding='unicode')}"
                f"</{expected_root}>"
            )
            messages.append(f"wrapped fragment root <{root.tag}> in <{expected_root}>")
        else:
            inner = _serialize_children(root)
            text = f"<{expected_root}>{inner}</{expected_root}>"
            messages.append(f"rewrapped root from <{root.tag}> to <{expected_root}>")
        ok2, err2 = _try_parse(text)
        if ok2:
            return XmlRepairResult(
                ok=True,
                text=text,
                message=_join_messages(messages),
                was_repaired=True,
            )
        return XmlRepairResult(
            ok=False,
            text=original,
            message="",
            error=f"Rewrapped fragment still invalid: {err2}",
        )

    # 6. Parse failed.  Common mod problems:
    #    - missing root element (raw list of <type> blocks)
    #    - multiple top-level elements ("junk after document element")
    #    - mismatched tags caused by copy-paste errors
    error_lower = err.lower()
    if any(
        token in error_lower
        for token in ("junk after document element", "mismatched tag")
    ):
        stripped = _strip_xml_declaration(text)
        wrapped = f"<{expected_root}>{stripped}</{expected_root}>"
        ok2, err2 = _try_parse(wrapped)
        if ok2:
            messages.append(f"wrapped raw fragment in <{expected_root}>")
            return XmlRepairResult(
                ok=True,
                text=wrapped,
                message=_join_messages(messages),
                was_repaired=True,
            )
        return XmlRepairResult(
            ok=False,
            text=original,
            message="",
            error=f"Could not repair fragment: {err2}",
        )

    return XmlRepairResult(
        ok=False,
        text=original,
        message="",
        error=f"Could not repair fragment: {err}",
    )


def _join_messages(messages: list[str]) -> str:
    if not messages:
        return "XML is valid"
    return "Repaired: " + "; ".join(messages)


# ------------------------------------------------------------------
# Final validation for merged mission files
# ------------------------------------------------------------------
@dataclass
class XmlValidationResult:
    """Outcome of validating a final merged mission XML file."""

    ok: bool
    message: str
    warnings: list[str]
    errors: list[str]


def validate_mission_xml(text: str, expected_root: str) -> XmlValidationResult:
    """Validate a merged mission XML file.

    Checks that the document parses, has the expected root tag, and that all
    top-level children use the expected child tag.  Duplicate ``name``
    attributes are reported as warnings rather than hard errors because some
    mods intentionally overlay existing types.
    """
    warnings: list[str] = []
    errors: list[str] = []

    ok, err = _try_parse(text)
    if not ok:
        errors.append(f"XML parse error: {err}")
        return XmlValidationResult(False, "Parse failed", warnings, errors)

    try:
        root = ET.fromstring(text)
    except ET.ParseError as exc:
        errors.append(f"XML parse error: {exc}")
        return XmlValidationResult(False, "Parse failed", warnings, errors)

    actual_root = root.tag
    if actual_root.lower() != expected_root.lower():
        errors.append(
            f"Unexpected root element <{actual_root}>, expected <{expected_root}>"
        )

    expected_child = ROOT_TO_CHILD_TAG.get(expected_root.lower())
    if expected_child is not None:
        allowed = (
            expected_child
            if isinstance(expected_child, set)
            else {expected_child}
        )
        for child in root:
            if child.tag not in allowed:
                errors.append(
                    f"Unexpected top-level element <{child.tag}> under <{expected_root}>"
                )
                break

    # Detect duplicate top-level name attributes.  For types.xml and
    # cfgspawnabletypes.xml duplicate names are a common source of trouble.
    names_seen: dict[str, int] = {}
    if expected_root.lower() in ("types", "spawnabletypes", "events"):
        for child in root:
            name = child.get("name")
            if name:
                lower = name.lower()
                names_seen[lower] = names_seen.get(lower, 0) + 1
        duplicates = [name for name, count in names_seen.items() if count > 1]
        if duplicates:
            warnings.append(
                f"Duplicate name attributes ({len(duplicates)}): "
                + ", ".join(duplicates[:10])
                + ("..." if len(duplicates) > 10 else "")
            )

    if errors:
        return XmlValidationResult(
            False,
            "Validation failed",
            warnings,
            errors,
        )

    message = f"<{expected_root}> is valid ({len(root)} children)"
    if warnings:
        message += f"; {len(warnings)} warning(s)"
    return XmlValidationResult(True, message, warnings, errors)


@dataclass
class TypesAuditResult:
    """Quick health check of a types.xml file."""

    total_types: int = 0
    weapon_types: int = 0
    zero_nominal_weapons: int = 0
    missing_nominal_weapons: int = 0
    zero_lifetime_weapons: int = 0
    sample_weapons: list[str] = field(default_factory=list)
    zero_nominal_names: list[str] = field(default_factory=list)


def audit_types_xml(text: str) -> TypesAuditResult:
    """Return a quick health check of a types.xml file.

    Counts total <type> entries, weapon entries, and flags weapons with
    ``<nominal>0</nominal>`` or no ``<nominal>`` element, both of which
    prevent spawning.
    """
    result = TypesAuditResult()
    ok, _ = _try_parse(text)
    if not ok:
        return result

    try:
        root = ET.fromstring(text)
    except ET.ParseError:
        return result

    if root.tag.lower() != "types":
        return result

    weapon_keywords = (
        "rifle", "pistol", "shotgun", "smg", "submachine", "machinegun",
        "lmg", "hmg", "sniper", "dmr", "carbine", "launcher", "akm", "ak74",
        "ak101", "ak102", "m4a1", "m16", "ar15", "scar", "fal", "mosin",
        "svd", "vss", "mp5", "ump", "glock", "fnx", "cz", "deagle",
        "revolver", "magnum", "mak", "mp133", "izh", "winchester", "repeater",
        "blazer", "longhorn", "crossbow", "bow", "rpg", "law", "m72",
        "m249", "m60", "pkp", "rpk", "m14", "m1a", "sks", "aug", "famas",
        "g3", "m110", "mk17", "mk18", "mk14", "m24", "m40", "awp", "awm",
        "l96", "tundra", "cr527", "cz527", "ssg82", "scout", "boomstick",
        "spas12", "saiga", "b95", " blaze", "mlock", "kolt", "longhorn",
    )

    for type_elem in root.findall("type"):
        result.total_types += 1
        name = type_elem.get("name", "")
        lower_name = name.lower()
        if not any(kw in lower_name for kw in weapon_keywords):
            continue

        result.weapon_types += 1
        nominal_elem = type_elem.find("nominal")
        if nominal_elem is None or nominal_elem.text is None:
            result.missing_nominal_weapons += 1
            result.zero_nominal_names.append(name)
        else:
            try:
                nominal = int(nominal_elem.text)
            except ValueError:
                nominal = -1
            if nominal <= 0:
                result.zero_nominal_weapons += 1
                result.zero_nominal_names.append(name)

        lifetime_elem = type_elem.find("lifetime")
        if lifetime_elem is not None and lifetime_elem.text:
            try:
                if int(lifetime_elem.text) <= 0:
                    result.zero_lifetime_weapons += 1
            except ValueError:
                pass

        if len(result.sample_weapons) < 10 and name not in result.sample_weapons:
            result.sample_weapons.append(name)

    return result


def summarize_types_audit(audit: TypesAuditResult) -> str:
    """Return a short human-readable summary of a types.xml audit."""
    msg = (
        f"{audit.total_types} types, {audit.weapon_types} weapon entries"
    )
    if audit.zero_nominal_weapons or audit.missing_nominal_weapons:
        msg += (
            f"; {audit.zero_nominal_weapons} weapons with nominal=0, "
            f"{audit.missing_nominal_weapons} weapons with no <nominal>"
        )
    if audit.zero_lifetime_weapons:
        msg += f"; {audit.zero_lifetime_weapons} weapons with lifetime=0"
    if audit.sample_weapons:
        msg += f"; examples: {', '.join(audit.sample_weapons[:5])}"
    return msg


def validate_cfgeconomycore(text: str) -> XmlValidationResult:
    """Validate cfgeconomycore.xml structure.

    Requires a root ``<economycore>`` element with ``<classes>`` and
    ``<defaults>`` sections.  ``<ce>`` sections are checked for well-formed
    ``<file name="..." type="..." />`` entries.
    """
    warnings: list[str] = []
    errors: list[str] = []

    ok, err = _try_parse(text)
    if not ok:
        errors.append(f"cfgeconomycore.xml parse error: {err}")
        return XmlValidationResult(False, "Parse failed", warnings, errors)

    try:
        root = ET.fromstring(text)
    except ET.ParseError as exc:
        errors.append(f"cfgeconomycore.xml parse error: {exc}")
        return XmlValidationResult(False, "Parse failed", warnings, errors)

    if root.tag.lower() != "economycore":
        errors.append(f"Unexpected root <{root.tag}>, expected <economycore>")

    required = {"classes", "defaults"}
    found = {child.tag.lower() for child in root}
    missing = required - found
    if missing:
        errors.append(f"Missing required sections: {', '.join(sorted(missing))}")

    ce_count = sum(1 for child in root if child.tag.lower() == "ce")
    if ce_count == 0:
        warnings.append("No <ce> sections found; DayZ will ignore custom CE files")

    for ce in root.findall("ce"):
        folder = ce.get("folder", "")
        for file_elem in ce.findall("file"):
            name = file_elem.get("name")
            ftype = file_elem.get("type")
            if not name:
                errors.append("<ce> contains <file> without a name attribute")
            if not ftype:
                warnings.append(f"<ce folder='{folder}'><file name='{name}'/> missing type attribute")

    if errors:
        return XmlValidationResult(False, "cfgeconomycore.xml invalid", warnings, errors)
    return XmlValidationResult(
        True,
        f"cfgeconomycore.xml valid ({ce_count} <ce> section(s))",
        warnings,
        errors,
    )
