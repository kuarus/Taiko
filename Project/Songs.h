#pragma once
#include <string>
#include <vector>

class Songs {
public:
	struct SONG {
		std::string file_name;
		std::string directory;
	};
	enum DIFF {
		ONI,
		HARD,
		NORMAL,
		EASY,
		MAX_DIFF
	};
public:
	Songs( );
	virtual ~Songs( );
public:
	std::vector< SONG > getSongList( ) const;
	std::string getTitle( std::string file_name ) const;
	std::string getTitle( int idx ) const;
	std::string getMusicFileName( int idx ) const;
	SONG getSongData( int idx ) const;
	int getLevel( int idx, DIFF diff ) const;
	std::vector< std::vector< char > > getCode( int idx, DIFF diff ) const;
	int getPitch( int idx ) const;
	int getOffset( int idx ) const;
private:
	void sarchFile( );
	std::vector< std::string > getDirectory( ) const;
	std::string getGenreName( std::string directory ) const;
private:
	std::vector< SONG > _song_list;
};