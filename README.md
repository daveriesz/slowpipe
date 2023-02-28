# slowpipe

Slowpipe is a simple utility to slow the standard output from another command.  It may not be vitally useful, but it looks cool.

## Usage
```
slowpipe: print from STDIN to STDOUT slowly.

usage:

    slowpipe <interval> [-g] [-h]

    interval: sleep interval between characters in milliseconds
              e.g.: "10" or "5.6"
    -s      : silent mode (suppress curor)
    -g      : print debugging information
    -h      : print this help information
```

An example of slowpipe's usage may look like:
```
dmesg | slowpipe 10
```

## Issues

Issue?  Bug?  Feature request?  Please submit an [issue](https://github.com/daveriesz/slowpipe/issues).

## Contributing

Feel free to submit a pull request if you want to contribute.
I'm not very good at reading other people's code, so please
include a verbose description and/or explanation.

## Licensing

The script and attendant documentation are Copyright (C) 2023 David Riesz.

This project and its code are all licensed under the MIT License.
Please read the [license file](LICENSE).
