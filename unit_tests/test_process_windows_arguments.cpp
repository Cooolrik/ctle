// ctle Copyright (c) 2026 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <ctle/process_windows_arguments.inl>

#include "unit_tests.h"

TEST( process_windows_arguments, preserves_empty_arguments_and_whitespace )
{
	EXPECT_EQ( ctle::detail::windows_command_line( { "tool.exe", "", "", "a b", "a\tb", "" } ),
		"\"tool.exe\" \"\" \"\" \"a b\" \"a\tb\" \"\"" );
	EXPECT_EQ( ctle::detail::windows_command_line( { "tool.exe" } ), "\"tool.exe\"" );
}

TEST( process_windows_arguments, preserves_backslashes_and_literal_quotes )
{
	EXPECT_EQ( ctle::detail::windows_command_line( { "tool.exe", R"(C:\Temp\file.txt)" } ),
		R"("tool.exe" "C:\Temp\file.txt")" );
	EXPECT_EQ( ctle::detail::windows_command_line( { "tool.exe", R"(C:\path with spaces\)" } ),
		R"("tool.exe" "C:\path with spaces\\")" );
	EXPECT_EQ( ctle::detail::windows_command_line( { "tool.exe", R"(say "hello")" } ),
		R"("tool.exe" "say \"hello\"")" );
	EXPECT_EQ( ctle::detail::windows_command_line( { "tool.exe", R"(a\"b)" } ),
		R"("tool.exe" "a\\\"b")" );
	EXPECT_EQ( ctle::detail::windows_command_line( { "tool.exe", R"(a\\"b)" } ),
		R"("tool.exe" "a\\\\\"b")" );
	EXPECT_EQ( ctle::detail::windows_command_line( { "tool.exe", R"(\\)" } ),
		R"("tool.exe" "\\\\")" );
}

TEST( process_windows_arguments, executable_and_shell_characters_are_literal )
{
	EXPECT_EQ( ctle::detail::windows_command_line( { R"(C:\Program Files\tool.exe)", "argument" } ),
		R"("C:\Program Files\tool.exe" "argument")" );
	EXPECT_EQ( ctle::detail::windows_command_line( { "tool.exe", R"('^&|<>%PATH%$HOME)" } ),
		R"("tool.exe" "'^&|<>%PATH%$HOME")" );
	EXPECT_EQ( ctle::detail::windows_command_line( { "tool.exe", "line\r\nbreak" } ),
		"\"tool.exe\" \"line\r\nbreak\"" );
	EXPECT_EQ( ctle::detail::windows_command_line( { "tool.exe", "\xc3\xa5 \xe6\x97\xa5" } ),
		"\"tool.exe\" \"\xc3\xa5 \xe6\x97\xa5\"" );
}
