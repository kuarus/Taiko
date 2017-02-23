#pragma once
class Task {
public:
	Task( );
	virtual ~Task( );
public:
	virtual void update( ) = 0;
};

