#pragma once

class file_system
{
public:
	typedef std::map<std::string, std::vector<std::string> > file_map;
	typedef file_map::const_iterator map_citer;
	typedef file_map::value_type value_type;
	
	void clear();	
	bool get( std::string& file_name );
	void add_path( const std::string& dir_path );

	void dump_dups();

	static void to_own( std::string& file_name );

	static file_system& instance();


	const file_map& get_map() const {return map_;}

private:
	void enumerate( const boost::filesystem::path& dir_path, file_map& _map );	
	file_map map_;
};


bool FindMedia( std::string& file_name );