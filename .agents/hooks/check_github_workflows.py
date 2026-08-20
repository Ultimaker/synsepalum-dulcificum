#!/usr/bin/env python3
"""
check_github_workflows.py — Validate GitHub Actions workflow definitions and action references.

Checks:
1. Valid YAML syntax in all .github/workflows/*.yml files.
2. Verified action references in `uses:` steps:
   - Catches unversioned or malformed tags (e.g. known tag-prefix bugs like missing 'v' in 'platisd/clang-tidy-pr-comments@1.8.0').
   - Catches invalid major version tags (e.g. 'actions/upload-artifact@v7').
3. Valid directory paths in `paths:` triggers:
   - Warns and blocks when workflow paths reference non-existent directories (e.g. 'test/' when only 'tests/' exists).
"""

import os
import re
import subprocess
import sys
from pathlib import Path
import yaml

_ROOT = subprocess.run(["git", "rev-parse", "--show-toplevel"],
                       capture_output=True, text=True).stdout.strip()
if _ROOT:
    os.chdir(_ROOT)

KNOWN_BAD_ACTION_REFS = {
    "platisd/clang-tidy-pr-comments@1.8.0": "platisd/clang-tidy-pr-comments@v1.8.0 (tag requires 'v' prefix)",
    "actions/upload-artifact@v7": "actions/upload-artifact@v4 (v7 does not exist)",
    "actions/download-artifact@v7": "actions/download-artifact@v4 (v7 does not exist)",
}

def check_workflows() -> int:
    workflows_dir = Path(".github/workflows")
    if not workflows_dir.is_dir():
        return 0

    violations = []
    workflow_files = list(workflows_dir.glob("*.yml")) + list(workflows_dir.glob("*.yaml"))

    for wf in workflow_files:
        try:
            with open(wf, "r", encoding="utf-8") as f:
                content = f.read()
                data = yaml.safe_load(content)
        except Exception as e:
            print(f"ERROR: {wf}: Invalid YAML syntax: {e}", file=sys.stderr)
            violations.append(f"{wf}: Invalid YAML syntax: {e}")
            continue

        if not isinstance(data, dict):
            continue

        # Check path triggers against existing repo directories
        on_block = data.get("on", {}) or data.get(True, {})
        triggers = []
        if isinstance(on_block, dict):
            triggers.extend(on_block.values())
        elif isinstance(on_block, list):
            triggers.extend(on_block)

        for trigger in triggers:
            if isinstance(trigger, dict):
                paths = trigger.get("paths", [])
                if isinstance(paths, list):
                    for path_pattern in paths:
                        match = re.match(r"^([a-zA-Z0-9_\-]+)/", path_pattern)
                        if match:
                            top_dir = match.group(1)
                            if top_dir in ("test", "srctest") and not Path(top_dir).exists() and Path("tests").exists():
                                violations.append(
                                    f"{wf}: Path pattern '{path_pattern}' references non-existent directory '{top_dir}/' (repository uses 'tests/')"
                                )

        # Check all `uses:` in raw lines and jobs
        for line_num, line in enumerate(content.splitlines(), start=1):
            stripped = line.strip()
            if stripped.startswith("uses:"):
                action_ref = stripped.split("uses:", 1)[1].strip().strip("'\"")
                if action_ref in KNOWN_BAD_ACTION_REFS:
                    remediation = KNOWN_BAD_ACTION_REFS[action_ref]
                    violations.append(
                        f"{wf}:{line_num}: Invalid action reference '{action_ref}' -> Use {remediation}"
                    )
                # Catch unversioned platisd/clang-tidy-pr-comments without 'v'
                if re.search(r"platisd/clang-tidy-pr-comments@[0-9]", action_ref):
                    violations.append(
                        f"{wf}:{line_num}: Malformed action tag '{action_ref}' -> Tags for platisd/clang-tidy-pr-comments must start with 'v' (e.g. @v1.8.0)"
                    )

    if violations:
        print("❌ GitHub Actions Workflow Validation Failed:")
        for v in violations:
            print(f"  • {v}")
        return 1

    print("✅ GitHub Actions workflows validated successfully.")
    return 0

if __name__ == "__main__":
    sys.exit(check_workflows())
