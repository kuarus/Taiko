#pragma once
#include <string>
#include <vector>

class Songs {
public:
	struct SONG {
		std::string file_name;
		std::string directory;
	};
public:
	Songs( );
	virtual ~Songs( );
public:
	std::vector< SONG > getSongList( );
	std::string getTitle( std::string file_name );
	std::string getTitle( int idx );
	std::string getMusicFileName( int idx );
	SONG getSongData( int idx );
	int getLevel( int idx );
	std::vector< std::vector< char > > getCode( int idx );
	int getBpm( int idx );
private:
	void sarchFile( );
	std::vector< std::string > getDirectory( );
	std::string getDirectory( int idx );
	std::string getGenreName( std::string directory );
private:
	std::vector< SONG > _song_list;
};