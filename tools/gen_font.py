#!/usr/bin/env python3

import argparse
import pathlib
import logging
import re

logging.basicConfig(level=logging.ERROR, format="[%(levelname)s] %(message)s")

TEMPLATE_PLACEHOLDER = "@FONT_DATA@"
DATA_LINE_PATTERN = re.compile(r"^([.#\s]*)(;+(.*))?$")
COMMENT_LINE_PATTERN = re.compile(r"^(;+)(.*)$")


def validate_data_line(line: str) -> tuple[str, str | None]:
    """
    Validate a line and extract data and comment
    :param line: Raw input line.
    :return: A tuple (data, comment), where:
        - prefix: The data extracted from the line without spaces. It is None if no data is present.
        - comment: The comment extracted from the line. It is None if no comment is present.

    :raise ValueError: If the line contains invalid characters.
    """

    match = DATA_LINE_PATTERN.match(line)
    if not match:
        raise ValueError(f"Invalid data format at line {line!r}.")

    prefix = re.sub(r"\s+", "", match.group(1))
    comment = match.group(2)
    return prefix, comment


def _map_data(data: int) -> str:
    return "std::byte {" + str(data) + "}"


def transform_data_prefix(prefix: str) -> str:
    """
    Transform a validated prefix into its numeric representation.
    :param prefix: String containing only '.', '#', and spaces.
    :return: Transformed representation.
    :raise ValueError: If the prefix is not valid.
    """
    result: list[int] = []
    for ch in prefix:
        if ch == ".":
            result.append(0)
        elif ch == "#":
            result.append(1)
        else:
            raise ValueError(f"Invalid character in prefix: {ch}.")
    return ", ".join(map(_map_data, result)) + ","


def transform_comment(comment: str) -> str:
    """
    Transform a comment into its valid comment in C++
    :param comment: Data format comment
    :return: Transformed representation with multiple consecutive comment symbols stripped to one.

    :raise: ValueError: If the comment is not valid.
    """
    comment_match: re.Match[str] | None = COMMENT_LINE_PATTERN.match(comment)
    if comment_match is None:
        raise ValueError(f"Invalid comment format at line {comment}. Comment must start with ';'.")
    content = comment_match.group(2)
    return f"//{content}"


def process_data_line(line: str) -> str:
    """
    Process a single line of data.
    :param line: Raw input line.
    :return: Transformed representation.
    :raise ValueError: If the validation or transformation fails.
    """

    # Preserve empty lines
    if line == "\n":
        return "\n"

    stripped_line = line.strip()

    prefix, comment = validate_data_line(stripped_line)

    transformed_prefix: str = ""
    transformed_comment: str = ""

    if prefix:
        transformed_prefix = transform_data_prefix(prefix)

    if comment:
        transformed_comment = transform_comment(comment)

    return f"{transformed_prefix}{transformed_comment}\n"


def expand_template_line(line: str, replacement_lines: list[str]) -> list[str]:
    """
    Expand a template line with replacement lines.
    :param line: Template line to be expanded.
    :param replacement_lines: List of replacement lines.
    :return:
    """
    indentation: str = line[:len(line) - len(line.lstrip(" \t"))]
    expanded_template_lines: list[str] = []
    for replacement_line in replacement_lines:
        expanded_template_lines.append(f"{indentation}{replacement_line}")
    return expanded_template_lines


def process_data_file(path: pathlib.Path) -> list[str]:
    """
    Process a single data file.
    :param path: Path to the input data file.
    :return: List of transformed data lines.
    :raise SystemExit: On first validation error.
    """
    output_lines: list[str] = []

    try:
        with path.open("r", encoding="utf-8") as file_handler:
            for line_number, line in enumerate(file_handler, start=1):
                try:
                    output_lines.append(process_data_line(line))
                except ValueError as exception:
                    logging.error(f"Line {line_number}: {exception}")
                    raise SystemExit(1)
    except OSError as exception:
        logging.error(f"Failed to read file {path}: {exception}")
        raise SystemExit(1)

    return output_lines


def process_template_file(template_path: pathlib.Path, placeholder: str, replacement: list[str]) -> list[str]:
    """
    Process a template file.
    :param template_path: Path to the input template file.
    :param placeholder: Placeholder to look for in line to replace whole line.
    :param replacement: List of lines to replace template line.
    :return: List of template expanded.

    :raise SystemExit: On open failed.
    """

    expanded_lines: list[str] = []

    try:
        with template_path.open("r", encoding="utf-8") as file_handler:
            for line in file_handler:
                if placeholder not in line:
                    expanded_lines.append(line)
                    continue
                expanded = expand_template_line(line, replacement)
                expanded_lines.extend(expanded)
            file_handler.close()

    except OSError as exception:
        logging.error(f"Failed to read file {template_path}: {exception}")
        raise SystemExit(1)

    return expanded_lines


def write_output_file(output_path: pathlib.Path, lines: list[str]) -> None:
    """
    Write output to file.
    :param output_path: Output file path.
    :param lines: Lines to write.
    :return: None

    :raise SystemExit: On failed to write to path.
    """
    try:
        with output_path.open("w", encoding="utf-8") as file_handler:
            file_handler.writelines(lines)
            file_handler.close()
    except OSError as exception:
        logging.error(f"Failed to write to file {output_path}: {exception}")
        raise SystemExit(1)


def main() -> None:
    """
    Entry point for the font data code generator.

    Parses command-line arguments, processes the input data file,
    expands the template, and writes the result either to stdout
    or to an output file.
    """
    parser = argparse.ArgumentParser(
        description="Generate C++ font data from a .dat file and a template."
    )

    parser.add_argument(
        "input_data_file",
        type=pathlib.Path,
        help="Input data file (.dat)."
    )

    parser.add_argument(
        "template_file",
        type=pathlib.Path,
        help="Path to the template file."
    )

    parser.add_argument(
        "-o",
        "--output",
        type=pathlib.Path,
        required=False,
        help="Path to the output file. If omitted, output is printed to stdout."
    )

    parser.add_argument(
        "-p",
        "--placeholder",
        type=str,
        default=TEMPLATE_PLACEHOLDER,
        help="Placeholder to replace in the template."
    )

    args = parser.parse_args()

    # --- Processing pipeline ---
    replacement_lines: list[str] = process_data_file(args.input_data_file)

    expanded_lines: list[str] = process_template_file(
        args.template_file,
        args.placeholder,
        replacement_lines
    )

    # --- Output handling ---
    if args.output is None:
        # Print to stdout
        for line in expanded_lines:
            print(line, end="")
    else:
        write_output_file(args.output, expanded_lines)


if __name__ == "__main__":
    main()
