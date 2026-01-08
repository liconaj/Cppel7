#!/usr/bin/env python3

import argparse
import pathlib
import logging
from typing import Callable, Pattern

logging.basicConfig(level=logging.INFO, format="[%(levelname)s] %(message)s")


def validate_data_line(
        line: str,
        data_pattern: Pattern[str],
) -> tuple[str, str | None]:
    """
    Validate a data line and extract prefix and comment.
    :param line: Raw input line (stripped).
    :param data_pattern: Compiled regex used to validate the line.
    :return: Tuple (prefix, comment).
    :raise ValueError: If line format is invalid.
    """
    match = data_pattern.match(line)
    if not match:
        raise ValueError(f"Invalid data format at line {line!r}.")

    prefix = match.group(1)
    comment = match.group(2)
    return prefix, comment


def transform_comment(
        comment: str,
        comment_pattern: Pattern[str],
        output_comment_prefix: str,
) -> str:
    """
    Transform an input comment into an output comment.
    :param comment: Raw comment string.
    :param comment_pattern: Regex validating the comment format.
    :param output_comment_prefix: Output comment prefix (e.g. '//').
    :return: Transformed comment.
    :raise ValueError: If comment format is invalid.
    """
    match = comment_pattern.match(comment)
    if match is None:
        raise ValueError("Invalid comment format.")
    return f"{output_comment_prefix}{match.group(2)}"


def process_data_line(
        line: str,
        *,
        data_pattern: Pattern[str],
        comment_pattern: Pattern[str],
        transform_prefix_fn: Callable[[str], str],
        output_comment_prefix: str,
) -> str:
    """
    Process a single data line using injected parsing and transformation logic.
    :param output_comment_prefix: Prefix of output comment (Ej: '//' for C/C++ comment).
    :param comment_pattern: Prefix of comment in data file
    :param data_pattern: Regex used to validate the line.
    :param transform_prefix_fn: Function to transform data prefix
    :param line: Raw input line.
    :return: Transformed line.
    """
    if line == "\n":
        return "\n"

    prefix, comment = validate_data_line(line.strip(), data_pattern)

    output = ""
    if prefix:
        output += transform_prefix_fn(prefix)

    if comment:
        output += transform_comment(
            comment,
            comment_pattern,
            output_comment_prefix,
        )

    return output + "\n"


def process_data_file(
        path: pathlib.Path,
        process_line_fn: Callable[[str], str],
) -> list[str]:
    """
    Process an entire data file line by line.
    :param path: Input data file path.
    :param process_line_fn: Line processing function.
    :return: List of transformed lines.
    """
    logging.info(f"Processing data file: {path}")
    output_lines: list[str] = []

    try:
        with path.open("r", encoding="utf-8") as file_handler:
            for line_number, line in enumerate(file_handler, start=1):
                try:
                    output_lines.append(process_line_fn(line))
                except ValueError as exception:
                    logging.error(f"Line {line_number}: {exception}")
                    raise SystemExit(1)
    except OSError as exception:
        logging.error(f"Failed to read file {path}: {exception}")
        raise SystemExit(1)

    logging.info(f"Data file processed successfully ({len(output_lines)} lines)")
    return output_lines


def expand_template_line(line: str, replacement_lines: list[str]) -> list[str]:
    """
    Expand a template line preserving indentation.
    """
    indentation: str = line[:len(line) - len(line.lstrip(" \t"))]
    return [f"{indentation}{r}" for r in replacement_lines]


def process_template_file(
        template_path: pathlib.Path,
        placeholder: str,
        replacement: list[str],
) -> list[str]:
    """
    Process template file and expand placeholder.
    """
    logging.info(f"Processing template file: {template_path}")
    expanded_lines: list[str] = []
    template_found: bool = False
    generated_lines: int = 0

    try:
        with template_path.open("r", encoding="utf-8") as file_handler:
            for line in file_handler:
                if placeholder not in line:
                    expanded_lines.append(line)
                    continue

                template_found = True
                generated_lines += len(replacement)
                logging.info(f"Placeholder '{placeholder}' found, expanding template")

                expanded_lines.extend(
                    expand_template_line(line, replacement)
                )
    except OSError as exception:
        logging.error(f"Failed to read file {template_path}: {exception}")
        raise SystemExit(1)

    if not template_found:
        logging.warning(f"Placeholder '{placeholder}' not found in template file")

    logging.info(f"Template expansion completed ({generated_lines} lines generated)")
    return expanded_lines


def write_output(
        output: pathlib.Path | None,
        lines: list[str],
) -> None:
    """
    Write output either to stdout or to a file.
    """
    if output is None:
        for line in lines:
            print(line, end="")
        return

    try:
        logging.info(f"Writing output file: {output}")
        with output.open("w", encoding="utf-8") as file_handler:
            file_handler.writelines(lines)
        logging.info(f"Output file written successfully ({len(lines)} lines)")
    except OSError as exception:
        logging.error(f"Failed to write to file {output}: {exception}")
        raise SystemExit(1)


def run_generator(
        *,
        description: str,
        placeholder: str,
        process_line_fn: Callable[[str], str],
) -> None:
    """
    Generic generator entry point.
    """
    parser = argparse.ArgumentParser(description=description)

    parser.add_argument(
        "input_data_file",
        type=pathlib.Path,
        help="Input data file."
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
        default=placeholder,
        help="Placeholder to replace in the template."
    )

    args = parser.parse_args()

    replacement_lines = process_data_file(
        args.input_data_file,
        process_line_fn,
    )

    expanded_lines = process_template_file(
        args.template_file,
        args.placeholder,
        replacement_lines,
    )

    write_output(args.output, expanded_lines)
