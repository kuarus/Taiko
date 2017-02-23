#include "Songs.h"
#include "DxLib.h"
#include <Windows.h>
#include <fstream>

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

std::vector< std::string > Songs::getDirectory( ) {
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

std::string Songs::getGenreName( std::string directory ) {
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

std::string Songs::getMusicFileName( int idx ) {

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

Songs::SONG Songs::getSongData( int idx ) {
	return _song_list[ idx ];
}

int Songs::getLevel( int idx ) {
	int level = 0;
	std::string level_str = "";
	std::string file_name = _song_list[ idx ].directory + "/" + _song_list[ idx ].file_name;
	std::ifstream ifs( file_name );
	if ( ifs.fail( ) ) {
		return 0;
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

std::vector< std::vector< char > > Songs::getCode( int idx ) {
	std::vector< std::vector< char > > code;
	std::string tmp_str = "";
	std::string file_name = _song_list[ idx ].directory + "/" + _song_list[ idx ].file_name;
	std::ifstream ifs( file_name );
	if ( ifs.fail( ) ) {
		return code;
	}
	bool start = false;
	while ( getline( ifs, tmp_str ) ) {
		if ( !start ) {
			if ( strstr( tmp_str.c_str( ), "#START" ) != NULL ) {
				start = true;
				continue;
			}
		}
		if ( strstr( tmp_str.c_str( ), "#END" ) != NULL ) {
			break;
		}
		if ( strstr( tmp_str.c_str( ), "#" ) != NULL ) {
			continue;
		}
		if ( start ) {
			std::string::iterator ite = tmp_str.begin( );
			int count = 0;
			std::vector< char > tmp;
			while ( ite != tmp_str.end( ) ) {
				char str = (*ite);
				if ( strstr( (const char*)&str, "," ) ) {
					ite++;
					break;
				}
				tmp.push_back( atoi( (const char*)&str ) );
				count++;
				ite++;
			}
			code.push_back( tmp );
		}

	}
	return code;
}

int Songs::getBpm( int idx ) {
	int bpm = 0;
	std::string tmp;
	std::string file_name = _song_list[ idx ].directory + "/" + _song_list[ idx ].file_name;
	std::ifstream ifs( file_name );
	if ( ifs.fail( ) ) {
		return bpm;
	}

	while ( getline( ifs, tmp ) ) {
		if ( strstr( tmp.c_str( ), "BPM:" ) != NULL ) {
			tmp.replace( 0, 4, "" );
			break;
		}
	}
	bpm = atoi( tmp.c_str( ) );
	return bpm;
}

std::vector< Songs::SONG > Songs::getSongList( ) {
	return _song_list;
}

std::string Songs::getTitle( std::string file_name ) {
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

std::string Songs::getTitle( int idx ) {
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