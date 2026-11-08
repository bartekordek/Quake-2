#!/usr/bin/env python3
"""Normalize source file line endings to the repository default (LF).

C/C++ extensions match root .editorconfig:
  [*.{c,h,cc,cpp,cxx,hh,hpp,hxx,inl,ipp}]  end_of_line = lf
  [deps/**]                               end_of_line = unset  (skipped)

Also converts GLSL shader sources (.vert, .frag, .glsl, ...).

Usage:
  python scripts/normalize_line_endings.py
  python scripts/normalize_line_endings.py --dry-run
  python scripts/normalize_line_endings.py --root F:/Dev/Quake-2
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

# C/C++ (same as root .editorconfig) + GLSL shader sources.
SOURCE_EXTENSIONS = {
    ".c",
    ".h",
    ".cc",
    ".cpp",
    ".cxx",
    ".hh",
    ".hpp",
    ".hxx",
    ".inl",
    ".ipp",
    # Shaders
    ".vert",
    ".frag",
    ".glsl",
    ".comp",
    ".geom",
    ".tesc",
    ".tese",
}

# Directory names skipped anywhere under the tree.
SKIP_DIR_NAMES = {
    ".git",
    ".vs",
    ".vscode",
    ".github",
    "__pycache__",
    "Build_Win64",
    "deps",  # vendored; keep their own line endings
}


def should_skip_dir(path: Path) -> bool:
    return path.name in SKIP_DIR_NAMES


def is_source_file(path: Path) -> bool:
    return path.suffix.lower() in SOURCE_EXTENSIONS


def normalize_to_lf(path: Path, *, dry_run: bool) -> bool:
    """Return True if the file needed (or would need) a change."""
    raw = path.read_bytes()

    # Skip obviously binary content (NUL bytes).
    if b"\x00" in raw:
        return False

    # Normalize CRLF and lone CR to LF.
    text = raw.replace(b"\r\n", b"\n").replace(b"\r", b"\n")
    if text == raw:
        return False

    if not dry_run:
        path.write_bytes(text)
    return True


def iter_source_files(root: Path):
    stack = [root]
    while stack:
        current = stack.pop()
        try:
            entries = list(current.iterdir())
        except (PermissionError, OSError) as exc:
            print(f"skip dir (error): {current} ({exc})", file=sys.stderr)
            continue

        for entry in entries:
            if entry.is_dir():
                if should_skip_dir(entry):
                    continue
                stack.append(entry)
            elif entry.is_file() and is_source_file(entry):
                yield entry


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Convert repo C/C++ and shader sources to LF; skip deps/ and build dirs."
    )
    parser.add_argument(
        "--root",
        type=Path,
        default=Path(__file__).resolve().parents[1],
        help="Repository root (default: parent of scripts/)",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Report files that would change without writing",
    )
    args = parser.parse_args()

    root = args.root.resolve()
    if not root.is_dir():
        print(f"error: root is not a directory: {root}", file=sys.stderr)
        return 1

    changed = 0
    scanned = 0
    for path in iter_source_files(root):
        scanned += 1
        rel = path.relative_to(root)
        try:
            if normalize_to_lf(path, dry_run=args.dry_run):
                changed += 1
                prefix = "would fix" if args.dry_run else "fixed"
                print(f"{prefix}: {rel.as_posix()}")
        except (PermissionError, OSError) as exc:
            print(f"error: {rel.as_posix()} ({exc})", file=sys.stderr)

    mode = "dry-run" if args.dry_run else "write"
    print(f"done ({mode}): scanned={scanned}, changed={changed}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
