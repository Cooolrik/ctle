## process

`process::start()` owns one direct child process and its optional standard-stream
pipes. It takes an argument vector, a working directory, optional stdout/stderr
callbacks, and a flag enabling `put_stdin()`.

### Startup and streams

Only requested streams use pipes. Unconnected stdin reads EOF; unconnected stdout
and stderr are discarded through the platform null device. Callbacks run on
separate threads and return `false` to close their corresponding read pipe.

Invalid executable arguments return `status::invalid_param`. An executable or
working-directory failure returns an error from `start()`, without a process
object. All resources acquired during a failed launch are released.

Linux prepares arguments in the parent and uses `posix_spawnp()` with checked
file actions. It requires `posix_spawn_file_actions_addchdir_np()` (available in
glibc 2.29 and later) for the working directory. Pipe descriptors are close-on-exec.
Executable text files without a shebang use `/bin/sh`, as with `execvp()`.
The fallback preserves PATH lookup, the child working directory, and literal
arguments; it does not build a shell command string.
Windows restricts inherited handles to the three child standard streams.

### Windows argument encoding

Supply argument values without adding quotes or shell escapes yourself. Windows
startup encodes them according to the Microsoft C runtime's command-line rules:
empty arguments are retained, spaces and tabs stay inside their argument, and
ordinary backslashes are preserved. Backslashes before literal quotes and at the
end of a quoted argument are escaped as required by those rules.

The first argument is encoded as an executable path, using the CRT's separate
`argv[0]` rules. Double quotes in that Windows path are rejected as invalid input.
Subsequent arguments may contain literal double quotes.

This is not a shell encoder. Characters such as `&`, `|`, `%`, `$`, and `^` remain
literal when launching an ordinary executable. Programs using a custom command-line
parser, including shell command strings, may require their own argument conventions.
UTF-8 arguments are converted to UTF-16 for `CreateProcessW`; a child's narrow
`main()` encoding still depends on that child's runtime configuration.

### Exit and termination

`wait()` returns `status::ok` only after observing exit and collecting the child.
A finite timeout returns `status::not_ready` without changing the exit-code output.
Zero polls, the default waits indefinitely, and negative timeouts are invalid.
Subsequent waits and exit queries return the same cached exit code.

`terminate()` requests termination; it does not report an exit before it happens.
On Linux it sends SIGTERM, which the child may ignore. Signal termination is
reported as `INT_MIN` by `wait()`; the requested exit-code parameter only applies
on Windows. Terminating an already exited process preserves its recorded code.

Destruction stops pending stdin work, force-terminates and collects a remaining
direct child, and joins the I/O threads. On Linux this uses SIGKILL, so destruction
also collects a child that ignored an earlier SIGTERM request.

`put_stdin()` returns `invalid_param` if stdin was not enabled. It returns
`not_ready` after input shutdown, including after exit was observed or the writer
detected a closed pipe. Writes already queued are asynchronous and may be discarded
during shutdown.

### Limits

Lifecycle methods require external synchronization. Output callbacks may continue
draining after `wait()` returns; destruction joins them. Callbacks must return
normally, must not destroy their own process owner, and must not block indefinitely.

Only the direct child is managed, not its descendants. Descendants retaining pipe
handles can delay the I/O joins.
