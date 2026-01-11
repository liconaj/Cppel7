#!/usr/bin/env python3

import pathlib
import sys
import re

# Configure module path to find modules relative to the script path
SCRIPT_DIR = pathlib.Path(__file__).resolve().parent
sys.path.insert(0, str(SCRIPT_DIR))
from gen_common import run_generator, process_data_line

TEMPLATE_PLACEHOLDER = "@PALETTE_DATA@"

DATA_LINE_PATTERN = re.compile(r"^([\d\s]*)(#+(.*))?$")
COMMENT_LINE_PATTERN = re.compile(r"^(#+)(.*)$")


def transform_data_prefix(prefix: str) -> str:
    parts = re.sub(r"\s+", " ", prefix).split()
    if len(parts) != 3:
        raise ValueError("Palette entry must have exactly 3 components (R G B).")

    values: list[int] = []
    for part in parts:
        try:
            value = int(part)
        except ValueError:
            raise ValueError(f"Invalid integer value: {part}")

        if not (0 <= value <= 255):
            raise ValueError(f"Value out of byte range: {value}")

        values.append(value)

    r, g, b = values
    return f"{b}, {g}, {r}, 0,"


if __name__ == "__main__":
    run_generator(
        description="Generate C++ palette data from an RGB palette file and a template.",
        placeholder=TEMPLATE_PLACEHOLDER,
        process_line_fn=lambda line: process_data_line(
            line,
            data_pattern=DATA_LINE_PATTERN,
            comment_pattern=COMMENT_LINE_PATTERN,
            transform_prefix_fn=transform_data_prefix,
            output_comment_prefix="//",
        ),
    )
