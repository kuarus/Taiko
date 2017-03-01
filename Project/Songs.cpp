#include "Songs.h"
#include "DxLib.h"
#include <Windows.h>
#include <fstream>
#include <assert.h>

static const std::string DIRECTORY = "Resource/Songs/";
static const std::string EXTENTION = "tja";

Songs::Songs( ) {
	sarchFile( );
}


Songs::~Songs( ) {
}

void Songs::sarchFile( ) {
	std::vector< std::string > file_names;

	//各フォルダ内検索
	std::vector< std::string > directory = getDirectory( );
	std::vector< std::string >::iterator dir_ite = directory.begin( );
	while ( dir_ite != directory.end( ) ) {
		WIN32_FIND_DATA win32fd;
		std::string sarch = DIRECTORY + (*dir_ite) + "\\*." + EXTENTION;
		HANDLE handle = FindFirstFile( sarch.c_str( ), &win32fd );
		do {
			if ( handle == INVALID_HANDLE_VALUE ) {
				continue;
			}
			if ( !( win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) {
				//ファイル名追加
				SONG song;
				song.file_name = win32fd.cFileName;
				song.directory = DIRECTORY + (*dir_ite);
				_song_list.push_back( song );
			}
		} while ( FindNextFile( handle, &win32fd ) );
		
		dir_ite++;
	}
}

std::vector< std::string > Songs::getDirectory( ) const {
	std::vector< std::string > directory;
	WIN32_FIND_DATA win32fd;
	std::string sarch = DIRECTORY + "\\*";
	HANDLE handle = FindFirstFile( sarch.c_str( ), &win32fd );
	do {
		if( strcmp( win32fd.cFileName, "." ) && strcmp( win32fd.cFileName, ".." ) ) {
			if ( handle == INVALID_HANDLE_VALUE ) {
				continue;
			}
			if ( win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
				//フォルダ名追加
				directory.push_back( win32fd.cFileName );
			}
		}
	} while ( FindNextFile( handle, &win32fd ) );
	return directory;
}

std::string Songs::getGenreName( std::string directory ) const {
	std::string genre_name = "";

	std::string file_name = directory + "/";
	std::ifstream ifs( file_name );
	if ( ifs.fail( ) ) {
		return genre_name;
	}

	while ( getline( ifs, genre_name ) ) {
		if ( strstr( genre_name.c_str( ), "GenreName=" ) != NULL ) {
			genre_name.replace( 10, 0, "" );
			break;
		}
	}
	return genre_name;
}

std::string Songs::getMusicFileName( int idx ) const {

	std::string music = "";
	std::string file_name = _song_list[ idx ].directory + "/" + _song_list[ idx ].file_name;
	std::ifstream ifs( file_name );
	if ( ifs.fail( ) ) {
		return music;
	}
	while ( getline( ifs, music ) ) {
		if ( strstr( music.c_str( ), "WAVE:" ) != NULL ) {
			music.replace( 0, 5, "" );
			break;
		}
	}
	return music;
}

Songs::SONG Songs::getSongData( int idx ) const {
	return _song_list[ idx ];
}

int Songs::getLevel( int idx, DIFF diff ) const {
	int level = 0;
	std::string level_str;
	std::string file_name = _song_list[ idx ].directory + "/" + _song_list[ idx ].file_name;
	std::ifstream ifs( file_name );
	if ( ifs.fail( ) ) {
		return 0;
	}
	std::string sarch0 = "COURSE:";
	std::string sarch1 = "COURSE:";
	std::string sarch2 = "COURSE:";
	switch ( diff ) {
	case DIFF::HARD:
		sarch0 += "hard";
		sarch1 += "Hard";
		sarch2 += "2";
		break;
	case DIFF::NORMAL:
		sarch0 += "normal";
		sarch1 += "Normal";
		sarch2 += "1";
		break;
	case DIFF::EASY:
		sarch0 += "easy";
		sarch1 += "Easy";
		sarch2 += "0";
		break;
	}
	if ( diff != DIFF::ONI ) {
		while ( std::getline( ifs, level_str ) ) {
			if ( std::strstr( level_str.c_str( ), sarch0.c_str( ) ) != NULL ) {
				break;
			}
			if ( std::strstr( level_str.c_str( ), sarch1.c_str( ) ) != NULL ) {
				break;
			}
			if ( std::strstr( level_str.c_str( ), sarch2.c_str( ) ) != NULL ) {
				break;
			}
		}
	}
	while ( getline( ifs, level_str ) ) {
		if ( strstr( level_str.c_str( ), "LEVEL:" ) != NULL ) {
			level_str.replace( 0, 6, "" );
			break;
		}
	}
	level = std::atoi( level_str.c_str( ) );
	return level;
}

std::vector< std::vector< char > > Songs::getCode( int idx, DIFF diff ) const {
	std::vector< std::vector< char > > code;
	std::string tmp_str;
	std::string file_name = _song_list[ idx ].directory + "/" + _song_list[ idx ].file_name;
	std::ifstream ifs( file_name );
	if ( ifs.fail( ) ) {
		return code;
	}
	bool start = false;
	bool through = false;
	std::string sarch0 = "COURSE:";
	std::string sarch1 = "COURSE:";
	std::string sarch2 = "COURSE:";
	switch ( diff ) {
	case DIFF::ONI:
		sarch0 += "oni";
		sarch1 += "Oni";
		sarch2 += "3";
		break;
	case DIFF::HARD:
		sarch0 += "hard";
		sarch1 += "Hard";
		sarch2 += "2";
		break;
	case DIFF::NORMAL:
		sarch0 += "normal";
		sarch1 += "Normal";
		sarch2 += "1";
		break;
	case DIFF::EASY:
		sarch0 += "easy";
		sarch1 += "Easy";
		sarch2 += "0";
		break;
	}
	while ( std::getline( ifs, tmp_str ) ) {
		if ( std::strstr( tmp_str.c_str( ), sarch0.c_str( ) ) != NULL ) {
			break;
		}
		if ( std::strstr( tmp_str.c_str( ), sarch1.c_str( ) ) != NULL ) {
			break;
		}
		if ( std::strstr( tmp_str.c_str( ), sarch2.c_str( ) ) != NULL ) {
			break;
		}
	}
	while ( std::getline( ifs, tmp_str ) ) {
		if ( !start ) {
			if ( std::strstr( tmp_str.c_str( ), "#START" ) != NULL ) {
				start = true;
				continue;
			}
		}
		if ( tmp_str.size( ) == 0 ) {
			continue;
		}
		if ( std::strstr( tmp_str.c_str( ), "#END" ) != NULL ) {
			break;
		}
		if ( std::strstr( tmp_str.c_str( ), "#N" ) != NULL ) {
			through = true;
		}
		if ( std::strstr( tmp_str.c_str( ), "#E" ) != NULL ) {
			through = true;
		}
		if ( std::strstr( tmp_str.c_str( ), "#M" ) != NULL ) {
			through = false;
		}
		if ( std::strstr( tmp_str.c_str( ), "#" ) != NULL ) {
			continue;
		}
		if ( start && !through ) {
			std::string::const_iterator ite = tmp_str.begin( );
			std::vector< char > tmp;
			int size = tmp_str.size( );
			while ( ite != tmp_str.end( ) ) {
				char str = (*ite);
				if ( strstr( (const char*)&str, "," ) ) {
					break;
				}
				tmp.push_back( atoi( (const char*)&str ) );
				ite++;
			}
			code.push_back( tmp );
		}

	}
	return code;
}

int Songs::getPitch( int idx ) const {
	int pitch = 1500;
	std::string tmp;
	std::string file_name = _song_list[ idx ].directory + "/" + _song_list[ idx ].file_name;
	std::ifstream ifs( file_name );
	if ( ifs.fail( ) ) {
		return pitch;
	}

	while ( getline( ifs, tmp ) ) {
		if ( strstr( tmp.c_str( ), "PITCH:" ) != NULL ) {
			tmp.replace( 0, 6, "" );
			double d_pitch = std::stod( tmp, 0 );
			d_pitch *= 100;
			pitch = (int)d_pitch;
			break;
		}
	}
	return pitch;
}

int Songs::getOffset( int idx ) const {
	int offset = 0;
	std::string tmp;
	std::string file_name = _song_list[ idx ].directory + "/" + _song_list[ idx ].file_name;
	std::ifstream ifs( file_name );
	if ( ifs.fail( ) ) {
		return offset;
	}
	bool is_minus = false;
	while ( getline( ifs, tmp ) ) {
		if ( strstr( tmp.c_str( ), "OFFSET:" ) != NULL ) {
			tmp.replace( 0, 7, "" );
			if ( strstr( tmp.c_str( ), "-" ) != NULL ) {
				tmp.replace( 0, 1, "" );
				is_minus = true;
			}
			break;
		}
	}
	double d_offset = std::stod( tmp, 0 );
	d_offset *= 1000;
	offset = (int)d_offset;
	if ( is_minus ) {
		offset *= -1;
	}
	return offset;
}

std::vector< Songs::SONG > Songs::getSongList( ) const {
	return _song_list;
}

std::string Songs::getTitle( std::string file_name ) const {
	std::string title;
	std::ifstream ifs( file_name );
	if ( ifs.fail( ) ) {
		return title;
	}

	while ( getline( ifs, title ) ) {
		if ( strstr( title.c_str( ), "TITLE:" ) != NULL ) {
			title.replace( 0, 6, "" );
			break;
		}
	}
	return title;
}

std::string Songs::getTitle( int idx ) const {
	std::string file_name = _song_list[ idx ].directory + "/" + _song_list[ idx ].file_name;

	std::string title;
	std::ifstream ifs( file_name );
	if ( ifs.fail( ) ) {
		return title;
	}

	while ( getline( ifs, title ) ) {
		if ( strstr( title.c_str( ), "TITLE:" ) != NULL ) {
			if ( strstr( title.c_str( ), "TITLE:" ) != NULL ) {
				title.replace( 0, 6, "" );
				break;
			}
		}
	}
	
	return title;
}