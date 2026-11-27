# Search And Destroy Chess

<!-- markdownlint-disable MD013 --><!-- Badges cannot be split up over lines, hence will break 80 characters per line -->

[![Check build](https://github.com/richelbilderbeek/SearchAndDestroyChess/actions/workflows/check_build.yaml/badge.svg?branch=main)](https://github.com/richelbilderbeek/SearchAndDestroyChess/actions/workflows/check_build.yaml)
[![check_links](https://github.com/richelbilderbeek/SearchAndDestroyChess/actions/workflows/check_links.yaml/badge.svg?branch=main)](https://github.com/richelbilderbeek/SearchAndDestroyChess/actions/workflows/check_links.yaml)
[![Check markdown](https://github.com/richelbilderbeek/SearchAndDestroyChess/actions/workflows/check_markdown.yaml/badge.svg?branch=main)](https://github.com/richelbilderbeek/SearchAndDestroyChess/actions/workflows/check_markdown.yaml)
[![Check spelling](https://github.com/richelbilderbeek/SearchAndDestroyChess/actions/workflows/check_spelling.yaml/badge.svg?branch=main)](https://github.com/richelbilderbeek/SearchAndDestroyChess/actions/workflows/check_spelling.yaml)
[![codecov](https://codecov.io/gh/richelbilderbeek/SearchAndDestroyChess/branch/main/graph/badge.svg)](https://codecov.io/gh/richelbilderbeek/SearchAndDestroyChess)
[![Create Zensical website](https://github.com/richelbilderbeek/SearchAndDestroyChess/actions/workflows/create_zensical_website.yaml/badge.svg?branch=main)](https://github.com/richelbilderbeek/SearchAndDestroyChess/actions/workflows/create_zensical_website.yaml)

<!-- markdownlint-enable MD013 -->

- [Go to the nicely rendered website](https://richelbilderbeek.github.io/SearchAndDestroyChess/).

## Files used by continuous integration scripts

<!-- markdownlint-disable MD013 --><!-- Tables cannot be split up over lines, hence will break 80 characters per line -->

Filename                                  |Descriptions
------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------
[mlc_config.json](mlc_config.json)        |Configuration of the link checker, use `markdown-link-check --config mlc_config.json --quiet docs/**/*.md` to do link checking locally
[.spellcheck.yml](.spellcheck.yml)        |Configuration of the spell checker, use `pyspelling -c .spellcheck.yml` to do spellcheck locally
[.wordlist.txt](.wordlist.txt)            |Whitelisted words for the spell checker, use `pyspelling -c .spellcheck.yml` to do spellcheck locally
[.markdownlint.jsonc](.markdownlint.jsonc)|Configuration of the markdown linter, use `markdownlint "**/*.md"` to do markdown linting locally. The name of this file is a default name.
[.markdownlintignore](.markdownlintignore)|Files ignored by the markdown linter, use `markdownlint "**/*.md"` to do markdown linting locally. The name of this file is a default name.

<!-- markdownlint-enable MD013 -->
