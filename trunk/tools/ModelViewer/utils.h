#pragma once

inline std::string getCommandLineParams()
{
	std::wstring cmd_line = GetCommandLine();
	size_t first = cmd_line.find_first_of( '"' );
	size_t last = cmd_line.find_first_of( '"', first+1 );
	cmd_line.erase( first, last+2 );

	if ( cmd_line.size() > 0 )
	{
		first = cmd_line.find_first_of( '"' );
		last = cmd_line.find_first_of( '"', first+1 );
		if ( last != -1)
			cmd_line.erase( last, last+1 );
		if ( first != -1)
			cmd_line.erase( first, first+1 );
	}
	return std::string(cmd_line.begin(), cmd_line.end());
}