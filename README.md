# termcaps

TCSADRAIN  

The change occurs after all output written to the file descriptor has been transmitted. This action should be used when changing parameters that affect output.

tgetent()

function looks up the termcap entry for name. The emulation ignores the buffer pointer bp. Returns 1, if entry found.

tcflag_t c_lflag :  unsigned integer
A bit mask specifying flags for local modes;

cc_t c_cc[NCCS] : unsigned integer 

An array specifying which characters are associated with various control functions;

Macro: tcflag_t ICANON : expands to 0x00000100

This bit, if set, enables canonical input processing mode. Otherwise, input is processed in noncanonical mode.

Macro: tcflag_t ECHO expand to 00000008

If this bit is set, echoing of input characters back to the terminal is enabled.

tcsetattr()

TCSANOW, the change shall occur immediately

TCSADRAIN, the change shall occur after all output written to fildes is transmitted. This function should be used when changing parameters that affect output

`im'

String of commands to enter insert mode. If the terminal has no special insert mode, but it can insert characters with a special command, `im' should be defined with a null value, because the `vi' editor assumes that insertion of a character is impossible if `im' is not provided. New programs should not act like `vi'. They should pay attention to `im' only if it is defined.

`ei'

String of commands to leave insert mode. This capability must be present if `im' is. On a few old terminals the same string is used to enter and exit insert mode. This string turns insert mode on if it was off, and off it it was on. You can tell these terminals because the `ei' string equals the `im' string.