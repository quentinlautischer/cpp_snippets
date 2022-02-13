# C++ Snippets

A place for succinct & reusable C++ code snippets

## Arg Parser

```c++
// program.exe --key value "in/file/path" -o "out/file/path" -h -v
auto args = command_args::parse(argc, argv);

bool is_help = args.help;
bool is_verbose = args.verbose;
std::ofstream(args.outfile);
std::ifstream(args.infile);

auto value = args.get("key");
```