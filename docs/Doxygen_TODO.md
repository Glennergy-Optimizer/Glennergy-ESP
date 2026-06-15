# Embedded Doxygen Future TODOs

This file tracks ideas and open questions for future iterations of the embedded Doxygen workflow and documentation standard.

These notes are not part of the active documentation rules used by the automation prompt.

## Workflow Follow-Ups

- Add per-model pricing support so workflow token-cost summaries reflect the actual selected model instead of a single default pricing rate.
- Revisit remaining-only rerun behavior so it can resume from the most relevant failed/deferred state when newer successful runs have happened in between.
- Consider whether the workflow should detect and document `app_main`, task workers, and event callbacks with more specialized templates.
- Consider whether `MAX_CHARS_PER_FILE` or `MAX_FILES_PER_RUN` should differ for ESP-IDF modules with many tightly coupled files.

## Documentation Style Questions

- Revisit whether warnings about blocking behavior in app/task files should remain part of the preferred style by default, or only be included when especially important.
- Revisit whether queue/event dependencies should be mentioned at file level or only at function level.
- Evaluate whether some low-level hardware/board config files should intentionally remain lightly documented if the code is mostly vendor or generated glue.

## Context Expansion Options

- Decide whether matching `.h` and `.c/.cpp` pairing should remain the only automatic context expansion rule.
- Revisit optional small-module support where a module may also include a closely related `main`, task, or integration file when that relationship is explicit and low-risk.
- Evaluate whether explicit per-module mapping would be useful for a few special embedded cases, for example one header, one source file, and one tightly coupled task or callback file.
