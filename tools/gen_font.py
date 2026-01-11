#!/usr/bin/env python3

import pathlib
import sys
import re

# Configure module path to find modules relative to the script path
SCRIPT_DIR = pathlib.Path(__file__).resolve().parent
sys.path.insert(0, str(SCRIPT_DIR))
from gen_common import run_generator, process_data_line

TEMPLATE_PLACEHOLDER = "@FONT_DATA@"

DATA_LINE_PATTERN = re.compile(r"^([.#\s]*)(;+(.*))?$")
COMMENT_LINE_PATTERN = re.compile(r"^(;+)(.*)$")


def _map_data(value: int) -> str:
    return f"{value}"


def transform_data_prefix(prefix: str) -> str:
    result: list[int] = []
    prefix = re.sub(r"\s+", "", prefix)

    for ch in prefix:
        if ch == ".":
            result.append(0)
        elif ch == "#":
            result.append(1)
        else:
            raise ValueError(f"Invalid character in prefix: {ch}.")

    return ", ".join(map(_map_data, result)) + ","


if __name__ == "__main__":
    run_generator(
        description="Generate C++ font data from a .dat file and a template.",
        placeholder=TEMPLATE_PLACEHOLDER,
        process_line_fn=lambda line: process_data_line(
            line,
            data_pattern=DATA_LINE_PATTERN,
            comment_pattern=COMMENT_LINE_PATTERN,
            transform_prefix_fn=transform_data_prefix,
            output_comment_prefix="//",
        ),
    )
