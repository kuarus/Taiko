#include "Songs.h"
#include "DxLib.h"
#include <Windows.h>
#include <fstream>
#include <assert.h>

static const std::string DIRECTORY = "Resource/Songs/";
static const std::string EXTENTION = "tja";

Songs::Songs( ) {
	loadSongInfoList( );
}


Songs::~Songs( ) {
}

std::vector< Songs::SONG_INFO > Songs::getSongInfoList( ) const {
	return _song_info_list;
}

Songs::SONG_DATA Songs::getSongData( int idx, DIFF diff ) const {
	SONG_DATA song_data = SONG_DATA( );
	std::string filename = _song_info_list[ idx ].filename;
	song_data.codes = getCode( filename, diff );
	std::string tmp;
	std::ifstream ifs( filename );
	if ( ifs.fail( ) ) {
		return song_data;
	}

	while ( getline( ifs, tmp ) ) {
		if ( strstr( tmp.c_str( ), "PITCH:" ) != NULL ) {
			tmp.replace( 0, 6, "" );
			song_data.pitch = std::stod( tmp, 0 );
			continue;
		}
		if ( strstr( tmp.c_str( ), "OFFSET:" ) != NULL ) {
			bool minus = false;
			tmp.replace( 0, 7, "" );
			if ( strstr( tmp.c_str( ), "-" ) != NULL ) {
				tmp.replace( 0, 1, "" );
				minus = true;
			}
			song_data.offset = std::stod( tmp, 0 );
			
			if ( minus ) {
				song_data.offset *= -1;
			}
			continue;
		}

		if ( song_data.offset != 0 &&
			 song_data.pitch != 0 ) {
			break;
		}
	}

	if ( song_data.pitch == 0 ) {
		song_data.pitch = 1000;
	}

	return song_data;
}

Songs::SONG_INFO Songs::getInfo( int idx ) const {
	return _song_info_list[ idx ];
}

int Songs::getLevel( int idx, DIFF diff ) const {
	int level = 0;
	std::string level_str;
	std::string filename = _song_info_list[ idx ].filename;
	std::ifstream ifs( filename );
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

void Songs::loadSongInfoList( ) {
	std::vector< std::string > file_names;

	//�e�t�H���_������
	std::vector< std::string > directory_list = getDirectoryList( );
	std::vector< std::string >::const_iterator dir_ite = directory_list.begin( );
	while ( dir_ite != directory_list.end( ) ) {
		std::string directory = DIRECTORY + (*dir_ite) + "/";
		GENRE genre = getGenre( directory );

		WIN32_FIND_DATA win32fd;
		std::string sarch = DIRECTORY + (*dir_ite) + "\\*." + EXTENTION;
		HANDLE handle = FindFirstFile( sarch.c_str( ), &win32fd );
		do {
			if ( handle == INVALID_HANDLE_VALUE ) {
				continue;
			}
			if ( !( win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) {
				//�t�@�C�����ǉ�
				std::string file_name = win32fd.cFileName;
				SONG_INFO song_info = getInfo( file_name, genre, directory ); 
				_song_info_list.push_back( song_info );
			}
		} while ( FindNextFile( handle, &win32fd ) );
		
		dir_ite++;
	}
}

Songs::SONG_INFO Songs::getInfo( std::string filename, Songs::GENRE genre, std::string directory ) const {
	SONG_INFO song_info = SONG_INFO( );
	song_info.genre = genre;
	song_info.filename = directory + filename;
	std::ifstream ifs( song_info.filename );
	std::string tmp;

	if ( ifs.fail( ) ) {
		return song_info;
	}

	while ( getline( ifs, tmp ) ) {
		if ( strstr( tmp.c_str( ), "SUBTITLE:" ) != NULL ) {
			continue;
		}
		if ( strstr( tmp.c_str( ), "TITLE:" ) != NULL ) {
			tmp.replace( 0, 6, "" );
			song_info.title = tmp;
			continue;
		}
		if ( strstr( tmp.c_str( ), "WAVE:" ) != NULL ) {
			tmp.replace( 0, 5, "" );
			song_info.music = directory + tmp;
			continue;
		}
		if ( song_info.title.size( ) != 0 &&
			 song_info.music.size( ) != 0 ) {
			break;
		}
	}
 	return song_info;
}

std::vector< std::string > Songs::getDirectoryList( ) const {
	std::vector< std::string > directory_list;
	WIN32_FIND_DATA win32fd;
	std::string sarch = DIRECTORY + "\\*";
	HANDLE handle = FindFirstFile( sarch.c_str( ), &win32fd );
	do {
		if( strcmp( win32fd.cFileName, "." ) && strcmp( win32fd.cFileName, ".." ) ) {
			if ( handle == INVALID_HANDLE_VALUE ) {
				continue;
			}
			if ( win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
				//�t�H���_���ǉ�
				directory_list.push_back( win32fd.cFileName );
			}
		}
	} while ( FindNextFile( handle, &win32fd ) );
	return directory_list;
}

Songs::GENRE Songs::getGenre( std::string directory ) const {
	std::string tmp = "";
	GENRE genre;

	std::string filename = directory + "/_info.ini";
	std::ifstream ifs( filename );
	if ( ifs.fail( ) ) {
		return genre;
	}

	while ( getline( ifs, tmp ) ) {
		if ( strstr( tmp.c_str( ), "GenreName=" ) != NULL ) {
			tmp.replace( 0, 10, "" );
			genre.name = tmp;
			continue;
		}
		if ( strstr( tmp.c_str( ), "GenreColor=" ) != NULL ) {
			tmp.replace( 0, 11, "" );
			genre.color_code = tmp;
			continue;
		}
		if ( genre.name.size( ) != 0 && genre.color_code.size( ) != 0 ) {
			break;
		}
	}
	return genre;
}

std::vector< std::vector< char > > Songs::getCode( std::string filename, DIFF diff ) const {
	std::vector< std::vector< char > > code;
	std::string tmp_str;
	std::ifstream ifs( filename );
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