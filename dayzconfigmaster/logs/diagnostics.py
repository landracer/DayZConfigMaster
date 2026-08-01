# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Diagnostics engine for analyzing DayZ logs.

Identifies common failure patterns and suggests fixes.
Covers DayZ verification kick codes and build-tool failure signatures.
"""

import re
from typing import Any, Dict, List, Optional, Tuple


class DiagnosticRule:
    """A diagnostic rule with pattern matching."""

    def __init__(
        self,
        id: str,
        name: str,
        patterns: List[str],
        cause: str,
        fix: str,
        severity: str = "warning"
    ):
        """
        Initialize a diagnostic rule.

        Args:
            id: Unique rule ID
            name: Human-readable name
            patterns: Regex patterns to match in logs
            cause: Description of the problem
            fix: Suggested solution
            severity: error|warning|info
        """
        self.id = id
        self.name = name
        self.patterns = [re.compile(p, re.IGNORECASE) for p in patterns]
        self.cause = cause
        self.fix = fix
        self.severity = severity

    def check(self, log_content: str) -> Optional[Dict[str, str]]:
        """Check if rule matches the log content."""
        for pattern in self.patterns:
            if pattern.search(log_content):
                return {
                    "rule": self.id,
                    "name": self.name,
                    "cause": self.cause,
                    "fix": self.fix,
                    "severity": self.severity
                }
        return None


class DiagnosticsEngine:
    """Engine for running diagnostic rules on logs."""

    def __init__(self):
        self.rules = self._build_rules()

    @staticmethod
    def _build_rules() -> List[DiagnosticRule]:
        """Return the full diagnostic rule set."""
        return [
            # Verification kick codes
            DiagnosticRule(
                id="kick-missing-bisign",
                name="Client kicked: missing .bisign (VE_MISSING_BISIGN / 0x0004007e)",
                patterns=[
                    r"VE_MISSING_BISIGN",
                    r"0x0004007e",
                    r"missing bisignature",
                    r"missing.*\.bisign"
                ],
                cause="A shipped .pbo has no matching .bisign file.",
                fix="Rebuild with signing on and ship the Keys\\*.bikey with the server.",
                severity="error"
            ),
            DiagnosticRule(
                id="kick-patched-pbo",
                name="Client kicked: patched PBO (VE_PATCHED_PBO / 0x0004007c)",
                patterns=[
                    r"VE_PATCHED_PBO",
                    r"0x0004007c",
                    r"modified after signing",
                    r"patched pbo"
                ],
                cause="Client PBO bytes differ from the signed original.",
                fix="Rebuild and redistribute; ensure client and server run the same build.",
                severity="error"
            ),
            DiagnosticRule(
                id="kick-version-skew",
                name="Client kicked: mod version skew (VE_UM_* / 0x00040079-0x0004007a)",
                patterns=[
                    r"VE_UM_CLIENT_UPDATED",
                    r"VE_UM_SERVER_UPDATED",
                    r"0x00040079",
                    r"0x0004007a",
                    r"version mismatch",
                    r"different versions of mod"
                ],
                cause="Client and server run different versions of the same mod.",
                fix="Update both sides to the same build.",
                severity="error"
            ),
            DiagnosticRule(
                id="kick-missing-mod",
                name="Client kicked: missing server mod (VE_MISSING_MOD / 0x00040073)",
                patterns=[
                    r"VE_MISSING_MOD",
                    r"0x00040073",
                    r"missing.*mod.*server"
                ],
                cause="The server loads a mod the client does not have.",
                fix="Add the mod to the client's -mod chain or remove it server-side.",
                severity="error"
            ),
            DiagnosticRule(
                id="kick-extra-mod",
                name="Client kicked: extra client mod (VE_EXTRA_MOD / 0x00040074)",
                patterns=[
                    r"VE_EXTRA_MOD",
                    r"0x00040074",
                    r"unexpected.*mod.*pbo",
                    r"extra.*mod"
                ],
                cause="The client loads a mod the server does not run.",
                fix="Match the client's mod list to the server's, or relax equalModRequired.",
                severity="error"
            ),
            DiagnosticRule(
                id="kick-filepatching-mismatch",
                name="Client kicked: filePatching mismatch (0x00020005)",
                patterns=[
                    r"0x00020005",
                    r"filepatching mismatch"
                ],
                cause="Client runs -filePatching but serverDZ.cfg lacks allowFilePatching = 1;.",
                fix="Add allowFilePatching = 1; to the server config.",
                severity="error"
            ),

            # Build / tool failure signatures
            DiagnosticRule(
                id="build-config-include",
                name="Config include could not be resolved",
                patterns=[
                    r"cannot include file",
                    r"preprocessor failed"
                ],
                cause="A #include points at a file the tool can't find from its working path.",
                fix="Check the #include path in config.cpp and that the .hpp exists where expected.",
                severity="error"
            ),
            DiagnosticRule(
                id="build-access-violation",
                name="Binarize crashed with an access violation",
                patterns=[
                    r"0xc0000005",
                    r"access violation"
                ],
                cause="Usually an already-binarized ODOL .p3d fed back into Binarize.",
                fix="Run preflight to flag ODOL models and keep them out of binarization.",
                severity="error"
            ),
            DiagnosticRule(
                id="build-config-syntax",
                name="Config syntax error",
                patterns=[
                    r"error 3 while parsing",
                    r"config : some input after end of file",
                    r"cfgconvert failed",
                    r"cfgconvert returned error",
                    r"error reading config file",
                    r"encountered instead of"
                ],
                cause="CfgConvert/Binarize could not parse a config or included .hpp.",
                fix="Fix the reported line; run preflight to catch this up front.",
                severity="error"
            ),
            DiagnosticRule(
                id="build-signing-failed",
                name="Signing failed",
                patterns=[
                    r"dssignfile failed",
                    r"no \.bisign",
                    r"private key"
                ],
                cause="DSSignFile didn't produce a signature — bad key, permissions, or AV lock.",
                fix="Check the key path, output-folder permissions, and AV exclusions.",
                severity="error"
            ),
            DiagnosticRule(
                id="build-texture-failed",
                name="Texture conversion failed",
                patterns=[
                    r"imagetopaa failed",
                    r"unsupported texture",
                    r"texture conversion failed"
                ],
                cause="ImageToPAA rejected a source image (format/size; dimensions must be power-of-two).",
                fix="Re-export the texture as a clean PNG/TGA with power-of-two dimensions.",
                severity="error"
            ),
            DiagnosticRule(
                id="build-workdrive",
                name="Work drive path problem",
                patterns=[
                    r"cannot open p:",
                    r"work drive",
                    r"path not found"
                ],
                cause="A tool tried to read through P:\\ and the path was not mounted or is a dangling junction.",
                fix="Mount the work drive; the tool recreates junctions on the next build.",
                severity="warning"
            ),

            # Runtime / server issues
            DiagnosticRule(
                id="runtime-memory-leak",
                name="Possible memory leak",
                patterns=[
                    r"out of memory",
                    r"memory allocation failed",
                    r"failed to allocate"
                ],
                cause="Server or tool ran out of available memory.",
                fix="Reduce mod count, restart more frequently, or increase system RAM.",
                severity="error"
            ),
            DiagnosticRule(
                id="runtime-script-error",
                name="Script compilation/runtime error",
                patterns=[
                    r"can't compile",
                    r"script error",
                    r"compile error"
                ],
                cause="A SQF/Enforce script failed to compile or threw at runtime.",
                fix="Check the reported script file and line for syntax or undefined variables.",
                severity="error"
            ),
        ]

    def run(self, log_content: str) -> List[Dict[str, Any]]:
        """Run all diagnostic rules on log content."""
        results = []
        for rule in self.rules:
            match = rule.check(log_content)
            if match:
                results.append(match)
        return results

    @staticmethod
    def summarize(log_content: str) -> Tuple[int, int, int]:
        """Return (errors, warnings, missing) counts for a log tail."""
        errors = warnings = missing = 0
        for raw in (log_content or "").split("\n"):
            line = raw.lower()
            if any(n in line for n in ("error", "failed", "cannot ")):
                errors += 1
            if "warning" in line:
                warnings += 1
            if any(n in line for n in ("missing", "cannot open", "cannot load")):
                missing += 1
        return errors, warnings, missing
